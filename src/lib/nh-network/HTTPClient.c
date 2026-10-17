// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "HTTPClient.h"

#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#if defined(__ANDROID__)
    #include <dirent.h>
    #include <openssl/pem.h>
#endif

#ifndef MSG_NOSIGNAL
    #define MSG_NOSIGNAL 0
#endif

// DECLARE =========================================================================================

#define NH_NETWORK_HTTP_BUFFER_SIZE 16384
#define NH_NETWORK_HTTP_MAX_HEADER_SIZE (64 * 1024)

typedef struct nh_network_HTTPConnection {
    int fd;
    SSL *SSL_p;
    char buffer_p[NH_NETWORK_HTTP_BUFFER_SIZE];
    size_t position;
    size_t length;
} nh_network_HTTPConnection;

typedef struct nh_network_HTTPBuffer {
    char *data_p;
    size_t length;
    size_t capacity;
} nh_network_HTTPBuffer;

static pthread_once_t NH_NETWORK_SSL_ONCE = PTHREAD_ONCE_INIT;
static SSL_CTX *NH_NETWORK_SSL_CONTEXT_P = NULL;

// HELPERS =========================================================================================

static void nh_network_setError(
    nh_api_HTTPResponse *Response_p, const char *format_p, ...)
{
    va_list args;
    va_start(args, format_p);
    vsnprintf(Response_p->error_p, sizeof(Response_p->error_p), format_p, args);
    va_end(args);
}

static bool nh_network_appendToBuffer(
    nh_network_HTTPBuffer *Buffer_p, const char *data_p, size_t size)
{
    if (Buffer_p->length + size + 1 > Buffer_p->capacity) {
        size_t capacity = Buffer_p->capacity ? Buffer_p->capacity : 1024;
        while (Buffer_p->length + size + 1 > capacity) {capacity *= 2;}
        char *data2_p = realloc(Buffer_p->data_p, capacity);
        if (!data2_p) {return false;}
        Buffer_p->data_p = data2_p;
        Buffer_p->capacity = capacity;
    }
    memcpy(Buffer_p->data_p + Buffer_p->length, data_p, size);
    Buffer_p->length += size;
    Buffer_p->data_p[Buffer_p->length] = '\0';
    return true;
}

#if defined(__ANDROID__)
// Android keeps its trust store as one PEM file per certificate, named by the legacy OpenSSL
// subject hash, so OpenSSL's directory lookup cannot find them. Load every file instead.
static int nh_network_loadAndroidCertificates(
    SSL_CTX *Context_p)
{
    const char *dirs_pp[] = {
        "/apex/com.android.conscrypt/cacerts",
        "/system/etc/security/cacerts",
    };
    X509_STORE *Store_p = SSL_CTX_get_cert_store(Context_p);
    int loaded = 0;
    for (size_t i = 0; i < sizeof(dirs_pp) / sizeof(dirs_pp[0]) && loaded == 0; ++i) {
        DIR *Dir_p = opendir(dirs_pp[i]);
        if (!Dir_p) {continue;}
        struct dirent *Entry_p;
        while ((Entry_p = readdir(Dir_p))) {
            if (Entry_p->d_name[0] == '.') {continue;}
            char path_p[512];
            snprintf(path_p, sizeof(path_p), "%s/%s", dirs_pp[i], Entry_p->d_name);
            FILE *File_p = fopen(path_p, "r");
            if (!File_p) {continue;}
            X509 *Certificate_p = PEM_read_X509(File_p, NULL, NULL, NULL);
            fclose(File_p);
            if (!Certificate_p) {continue;}
            if (X509_STORE_add_cert(Store_p, Certificate_p) == 1) {++loaded;}
            X509_free(Certificate_p);
        }
        closedir(Dir_p);
    }
    return loaded;
}
#endif

static void nh_network_createSSLContextOnce()
{
    SSL_CTX *Context_p = SSL_CTX_new(TLS_client_method());
    if (!Context_p) {return;}

    SSL_CTX_set_min_proto_version(Context_p, TLS1_2_VERSION);
    SSL_CTX_set_verify(Context_p, SSL_VERIFY_PEER, NULL);
    SSL_CTX_set_default_verify_paths(Context_p);

    // OpenSSL builds do not always know the platform trust store, so also try common bundles.
    const char *bundles_pp[] = {
        getenv("SSL_CERT_FILE"),
        "/etc/ssl/cert.pem",
        "/etc/ssl/certs/ca-certificates.crt",
        "/etc/pki/tls/certs/ca-bundle.crt",
        "/opt/homebrew/etc/openssl@3/cert.pem",
        "/usr/local/etc/openssl@3/cert.pem",
        "/opt/homebrew/etc/ca-certificates/cert.pem",
    };
    for (size_t i = 0; i < sizeof(bundles_pp) / sizeof(bundles_pp[0]); ++i) {
        if (bundles_pp[i] && bundles_pp[i][0] && access(bundles_pp[i], R_OK) == 0) {
            SSL_CTX_load_verify_locations(Context_p, bundles_pp[i], NULL);
        }
    }
#if defined(__ANDROID__)
    nh_network_loadAndroidCertificates(Context_p);
#endif
    ERR_clear_error();

    NH_NETWORK_SSL_CONTEXT_P = Context_p;
}

// URL =============================================================================================

static bool nh_network_parseHTTPURL(
    const char *url_p, bool *tls_p, char *host_p, size_t hostSize, char *port_p, size_t portSize,
    const char **target_pp)
{
    const char *rest_p = NULL;
    if (strncasecmp(url_p, "https://", 8) == 0) {
        *tls_p = true;
        rest_p = url_p + 8;
        snprintf(port_p, portSize, "443");
    } else if (strncasecmp(url_p, "http://", 7) == 0) {
        *tls_p = false;
        rest_p = url_p + 7;
        snprintf(port_p, portSize, "80");
    } else {
        return false;
    }

    const char *hostEnd_p = rest_p;
    if (*rest_p == '[') {
        hostEnd_p = strchr(rest_p, ']');
        if (!hostEnd_p) {return false;}
        size_t length = (size_t)(hostEnd_p - rest_p - 1);
        if (length == 0 || length >= hostSize) {return false;}
        memcpy(host_p, rest_p + 1, length);
        host_p[length] = '\0';
        hostEnd_p++;
    } else {
        while (*hostEnd_p && *hostEnd_p != ':' && *hostEnd_p != '/' && *hostEnd_p != '?') {hostEnd_p++;}
        size_t length = (size_t)(hostEnd_p - rest_p);
        if (length == 0 || length >= hostSize) {return false;}
        memcpy(host_p, rest_p, length);
        host_p[length] = '\0';
    }

    if (*hostEnd_p == ':') {
        const char *portStart_p = hostEnd_p + 1;
        const char *portEnd_p = portStart_p;
        while (*portEnd_p >= '0' && *portEnd_p <= '9') {portEnd_p++;}
        size_t length = (size_t)(portEnd_p - portStart_p);
        if (length == 0 || length >= portSize) {return false;}
        memcpy(port_p, portStart_p, length);
        port_p[length] = '\0';
        hostEnd_p = portEnd_p;
    }

    *target_pp = *hostEnd_p ? hostEnd_p : "/";
    return true;
}

// CONNECTION ======================================================================================

static void nh_network_setSocketTimeouts(
    int fd, int timeoutSeconds)
{
    struct timeval Timeout;
    Timeout.tv_sec = timeoutSeconds;
    Timeout.tv_usec = 0;
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &Timeout, sizeof(Timeout));
    setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &Timeout, sizeof(Timeout));
#ifdef SO_NOSIGPIPE
    int one = 1;
    setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &one, sizeof(one));
#endif
}

static int nh_network_connectTCP(
    const char *host_p, const char *port_p, int timeoutSeconds, nh_api_HTTPResponse *Response_p)
{
    struct addrinfo Hints;
    memset(&Hints, 0, sizeof(Hints));
    Hints.ai_family = AF_UNSPEC;
    Hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *Info_p = NULL;
    int result = getaddrinfo(host_p, port_p, &Hints, &Info_p);
    if (result != 0) {
        nh_network_setError(Response_p, "Could not resolve %s: %s", host_p, gai_strerror(result));
        return -1;
    }

    int fd = -1;
    for (struct addrinfo *Current_p = Info_p; Current_p; Current_p = Current_p->ai_next) {
        fd = socket(Current_p->ai_family, Current_p->ai_socktype, Current_p->ai_protocol);
        if (fd < 0) {continue;}

        int flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);

        int connected = connect(fd, Current_p->ai_addr, Current_p->ai_addrlen);
        if (connected != 0 && errno == EINPROGRESS) {
            struct pollfd Poll = {.fd = fd, .events = POLLOUT};
            int ready = poll(&Poll, 1, timeoutSeconds * 1000);
            int error = 0;
            socklen_t length = sizeof(error);
            if (ready == 1 && getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &length) == 0 && error == 0) {
                connected = 0;
            }
        }

        if (connected == 0) {
            fcntl(fd, F_SETFL, flags);
            nh_network_setSocketTimeouts(fd, timeoutSeconds);
            break;
        }

        close(fd);
        fd = -1;
    }

    freeaddrinfo(Info_p);
    if (fd < 0) {nh_network_setError(Response_p, "Could not connect to %s:%s", host_p, port_p);}
    return fd;
}

static bool nh_network_startTLS(
    nh_network_HTTPConnection *Connection_p, const char *host_p, nh_api_HTTPResponse *Response_p)
{
    pthread_once(&NH_NETWORK_SSL_ONCE, nh_network_createSSLContextOnce);
    if (!NH_NETWORK_SSL_CONTEXT_P) {
        nh_network_setError(Response_p, "Could not create TLS context");
        return false;
    }

    SSL *SSL_p = SSL_new(NH_NETWORK_SSL_CONTEXT_P);
    if (!SSL_p) {
        nh_network_setError(Response_p, "Could not create TLS session");
        return false;
    }

    SSL_set_fd(SSL_p, Connection_p->fd);
    SSL_set_tlsext_host_name(SSL_p, host_p);
    SSL_set1_host(SSL_p, host_p);

    if (SSL_connect(SSL_p) != 1) {
        long verify = SSL_get_verify_result(SSL_p);
        char error_p[160] = {0};
        unsigned long code = ERR_get_error();
        if (code) {ERR_error_string_n(code, error_p, sizeof(error_p));}
        ERR_clear_error();
        if (verify != X509_V_OK) {
            nh_network_setError(Response_p, "TLS certificate verification failed for %s: %s",
                host_p, X509_verify_cert_error_string(verify));
        } else {
            nh_network_setError(Response_p, "TLS handshake with %s failed %s", host_p, error_p);
        }
        SSL_free(SSL_p);
        return false;
    }

    Connection_p->SSL_p = SSL_p;
    return true;
}

static void nh_network_closeConnection(
    nh_network_HTTPConnection *Connection_p)
{
    if (Connection_p->SSL_p) {
        SSL_shutdown(Connection_p->SSL_p);
        SSL_free(Connection_p->SSL_p);
        Connection_p->SSL_p = NULL;
    }
    if (Connection_p->fd >= 0) {
        close(Connection_p->fd);
        Connection_p->fd = -1;
    }
}

static bool nh_network_writeAll(
    nh_network_HTTPConnection *Connection_p, const char *data_p, size_t size)
{
    while (size > 0) {
        long written = 0;
        if (Connection_p->SSL_p) {
            int chunk = size > (1 << 30) ? (1 << 30) : (int)size;
            written = SSL_write(Connection_p->SSL_p, data_p, chunk);
            if (written <= 0) {ERR_clear_error(); return false;}
        } else {
            written = send(Connection_p->fd, data_p, size, MSG_NOSIGNAL);
            if (written < 0 && errno == EINTR) {continue;}
            if (written <= 0) {return false;}
        }
        data_p += written;
        size -= (size_t)written;
    }
    return true;
}

/**
 * Returns > 0 bytes read, 0 at EOF, -1 on error.
 */
static long nh_network_readRaw(
    nh_network_HTTPConnection *Connection_p, char *buffer_p, size_t size)
{
    if (Connection_p->SSL_p) {
        int chunk = size > (1 << 30) ? (1 << 30) : (int)size;
        int read = SSL_read(Connection_p->SSL_p, buffer_p, chunk);
        if (read > 0) {return read;}
        int error = SSL_get_error(Connection_p->SSL_p, read);
        ERR_clear_error();
        // Many servers close without close_notify; treat a clean socket EOF as end of stream.
        if (error == SSL_ERROR_ZERO_RETURN || (error == SSL_ERROR_SYSCALL && errno == 0)) {return 0;}
        return -1;
    }

    while (true) {
        long read = recv(Connection_p->fd, buffer_p, size, 0);
        if (read < 0 && errno == EINTR) {continue;}
        return read;
    }
}

static bool nh_network_fillBuffer(
    nh_network_HTTPConnection *Connection_p, bool *eof_p)
{
    *eof_p = false;
    if (Connection_p->position < Connection_p->length) {return true;}
    long read = nh_network_readRaw(Connection_p, Connection_p->buffer_p, sizeof(Connection_p->buffer_p));
    if (read < 0) {return false;}
    if (read == 0) {*eof_p = true;}
    Connection_p->position = 0;
    Connection_p->length = (size_t)read;
    return true;
}

/**
 * Reads one CRLF terminated line without the line break.
 */
static bool nh_network_readLine(
    nh_network_HTTPConnection *Connection_p, char *line_p, size_t size)
{
    size_t length = 0;
    while (true) {
        bool eof = false;
        if (!nh_network_fillBuffer(Connection_p, &eof) || eof) {return false;}
        char c = Connection_p->buffer_p[Connection_p->position++];
        if (c == '\n') {break;}
        if (c == '\r') {continue;}
        if (length + 1 >= size) {return false;}
        line_p[length++] = c;
    }
    line_p[length] = '\0';
    return true;
}

static bool nh_network_deliverBody(
    const nh_api_HTTPRequest *Request_p, nh_network_HTTPBuffer *Body_p, const char *data_p, size_t size)
{
    if (size == 0) {return true;}
    if (Request_p->write_f) {
        return Request_p->write_f(Request_p->writeUser_p, data_p, size) == size;
    }
    return nh_network_appendToBuffer(Body_p, data_p, size);
}

/**
 * Reads exactly size bytes (or until EOF if size is (size_t)-1) and forwards them.
 */
static bool nh_network_readBody(
    nh_network_HTTPConnection *Connection_p, const nh_api_HTTPRequest *Request_p,
    nh_network_HTTPBuffer *Body_p, size_t size)
{
    bool untilEOF = size == (size_t)-1;
    while (untilEOF || size > 0) {
        bool eof = false;
        if (!nh_network_fillBuffer(Connection_p, &eof)) {return false;}
        if (eof) {return untilEOF;}
        size_t available = Connection_p->length - Connection_p->position;
        size_t take = untilEOF || available < size ? available : size;
        if (!nh_network_deliverBody(Request_p, Body_p, Connection_p->buffer_p + Connection_p->position, take)) {
            return false;
        }
        Connection_p->position += take;
        if (!untilEOF) {size -= take;}
    }
    return true;
}

static bool nh_network_readChunkedBody(
    nh_network_HTTPConnection *Connection_p, const nh_api_HTTPRequest *Request_p,
    nh_network_HTTPBuffer *Body_p)
{
    char line_p[1024];
    while (true) {
        if (!nh_network_readLine(Connection_p, line_p, sizeof(line_p))) {return false;}
        char *end_p = NULL;
        unsigned long long size = strtoull(line_p, &end_p, 16);
        if (end_p == line_p) {return false;}
        if (size == 0) {
            // Skip trailers.
            do {
                if (!nh_network_readLine(Connection_p, line_p, sizeof(line_p))) {return false;}
            } while (line_p[0] != '\0');
            return true;
        }
        if (!nh_network_readBody(Connection_p, Request_p, Body_p, (size_t)size)) {return false;}
        if (!nh_network_readLine(Connection_p, line_p, sizeof(line_p))) {return false;}
    }
}

// REQUEST =========================================================================================

static bool nh_network_hasHeader(
    const nh_api_HTTPRequest *Request_p, const char *name_p)
{
    size_t length = strlen(name_p);
    for (int i = 0; i < Request_p->headerCount; ++i) {
        const char *header_p = Request_p->headers_pp[i];
        if (header_p && strncasecmp(header_p, name_p, length) == 0 && header_p[length] == ':') {return true;}
    }
    return false;
}

static bool nh_network_sendRequest(
    nh_network_HTTPConnection *Connection_p, const nh_api_HTTPRequest *Request_p,
    const char *method_p, const char *host_p, const char *port_p, bool tls, const char *target_p,
    nh_api_HTTPResponse *Response_p)
{
    nh_network_HTTPBuffer Head = {0};
    char line_p[2048];
    bool defaultPort = (tls && strcmp(port_p, "443") == 0) || (!tls && strcmp(port_p, "80") == 0);
    bool ok = true;

    ok = ok && nh_network_appendToBuffer(&Head, method_p, strlen(method_p));
    ok = ok && nh_network_appendToBuffer(&Head, " ", 1);
    ok = ok && nh_network_appendToBuffer(&Head, target_p, strlen(target_p));
    ok = ok && nh_network_appendToBuffer(&Head, " HTTP/1.1\r\n", 11);

    snprintf(line_p, sizeof(line_p), defaultPort ? "Host: %s\r\n" : "Host: %s:%s\r\n", host_p, port_p);
    ok = ok && nh_network_appendToBuffer(&Head, line_p, strlen(line_p));

    const char *fixed_p = "Connection: close\r\nAccept-Encoding: identity\r\n";
    ok = ok && nh_network_appendToBuffer(&Head, fixed_p, strlen(fixed_p));
    if (!nh_network_hasHeader(Request_p, "User-Agent")) {
        const char *agent_p = "User-Agent: Netzhaut\r\n";
        ok = ok && nh_network_appendToBuffer(&Head, agent_p, strlen(agent_p));
    }

    for (int i = 0; ok && i < Request_p->headerCount; ++i) {
        const char *header_p = Request_p->headers_pp[i];
        if (!header_p || !header_p[0] || strpbrk(header_p, "\r\n")) {continue;}
        ok = nh_network_appendToBuffer(&Head, header_p, strlen(header_p));
        ok = ok && nh_network_appendToBuffer(&Head, "\r\n", 2);
    }

    bool hasBody = Request_p->body_p || Request_p->read_f;
    bool needsLength = hasBody || strcasecmp(method_p, "POST") == 0 || strcasecmp(method_p, "PUT") == 0 ||
        strcasecmp(method_p, "PATCH") == 0;
    if (needsLength && !nh_network_hasHeader(Request_p, "Content-Length")) {
        snprintf(line_p, sizeof(line_p), "Content-Length: %zu\r\n", hasBody ? Request_p->bodySize : (size_t)0);
        ok = ok && nh_network_appendToBuffer(&Head, line_p, strlen(line_p));
    }
    ok = ok && nh_network_appendToBuffer(&Head, "\r\n", 2);

    if (!ok) {
        free(Head.data_p);
        nh_network_setError(Response_p, "Out of memory");
        return false;
    }

    ok = nh_network_writeAll(Connection_p, Head.data_p, Head.length);
    free(Head.data_p);
    if (!ok) {
        nh_network_setError(Response_p, "Failed to send request headers");
        return false;
    }

    if (Request_p->body_p) {
        if (!nh_network_writeAll(Connection_p, Request_p->body_p, Request_p->bodySize)) {
            nh_network_setError(Response_p, "Failed to send request body");
            return false;
        }
    } else if (Request_p->read_f) {
        char *chunk_p = malloc(NH_NETWORK_HTTP_BUFFER_SIZE);
        if (!chunk_p) {
            nh_network_setError(Response_p, "Out of memory");
            return false;
        }
        size_t remaining = Request_p->bodySize;
        while (remaining > 0) {
            size_t want = remaining < NH_NETWORK_HTTP_BUFFER_SIZE ? remaining : NH_NETWORK_HTTP_BUFFER_SIZE;
            size_t got = Request_p->read_f(Request_p->readUser_p, chunk_p, want);
            if (got == (size_t)-1 || got == 0 || got > want) {
                free(chunk_p);
                nh_network_setError(Response_p, "Request body source failed");
                return false;
            }
            if (!nh_network_writeAll(Connection_p, chunk_p, got)) {
                free(chunk_p);
                nh_network_setError(Response_p, "Failed to send request body");
                return false;
            }
            remaining -= got;
        }
        free(chunk_p);
    }

    return true;
}

static bool nh_network_receiveResponse(
    nh_network_HTTPConnection *Connection_p, const nh_api_HTTPRequest *Request_p, const char *method_p,
    nh_api_HTTPResponse *Response_p)
{
    char line_p[8192];
    nh_network_HTTPBuffer Headers = {0};

    // Skip interim 1xx responses.
    do {
        free(Headers.data_p);
        Headers = (nh_network_HTTPBuffer){0};
        if (!nh_network_readLine(Connection_p, line_p, sizeof(line_p))) {
            nh_network_setError(Response_p, "No response received");
            return false;
        }
        int major = 0, minor = 0, status = 0;
        if (sscanf(line_p, "HTTP/%d.%d %d", &major, &minor, &status) != 3) {
            nh_network_setError(Response_p, "Malformed status line");
            return false;
        }
        Response_p->status = status;

        while (true) {
            if (!nh_network_readLine(Connection_p, line_p, sizeof(line_p))) {
                free(Headers.data_p);
                nh_network_setError(Response_p, "Malformed response headers");
                return false;
            }
            if (line_p[0] == '\0') {break;}
            if (Headers.length + strlen(line_p) + 2 > NH_NETWORK_HTTP_MAX_HEADER_SIZE ||
                !nh_network_appendToBuffer(&Headers, line_p, strlen(line_p)) ||
                !nh_network_appendToBuffer(&Headers, "\r\n", 2)) {
                free(Headers.data_p);
                nh_network_setError(Response_p, "Response headers too large");
                return false;
            }
        }
    } while (Response_p->status >= 100 && Response_p->status < 200);

    Response_p->headers_p = Headers.data_p ? Headers.data_p : calloc(1, 1);

    nh_network_HTTPBuffer Body = {0};
    bool ok = true;
    char value_p[256];
    bool noBody = strcasecmp(method_p, "HEAD") == 0 || Response_p->status == 204 || Response_p->status == 304;

    if (!noBody) {
        if (nh_network_getHTTPHeader(Response_p, "Transfer-Encoding", value_p, sizeof(value_p)) &&
            strcasestr(value_p, "chunked")) {
            ok = nh_network_readChunkedBody(Connection_p, Request_p, &Body);
        } else if (nh_network_getHTTPHeader(Response_p, "Content-Length", value_p, sizeof(value_p))) {
            char *end_p = NULL;
            unsigned long long length = strtoull(value_p, &end_p, 10);
            ok = end_p != value_p && nh_network_readBody(Connection_p, Request_p, &Body, (size_t)length);
        } else {
            ok = nh_network_readBody(Connection_p, Request_p, &Body, (size_t)-1);
        }
    }

    Response_p->body_p = Body.data_p ? Body.data_p : calloc(1, 1);
    Response_p->bodySize = Body.length;

    if (!ok) {
        nh_network_setError(Response_p, "Failed to read response body");
        return false;
    }
    return true;
}

int nh_network_performHTTPRequest(
    const nh_api_HTTPRequest *Request_p, nh_api_HTTPResponse *Response_p)
{
    if (!Response_p) {return -1;}
    memset(Response_p, 0, sizeof(*Response_p));
    if (!Request_p || !Request_p->url_p) {
        nh_network_setError(Response_p, "Missing URL");
        return -1;
    }

    bool tls = false;
    char host_p[256];
    char port_p[8];
    const char *target_p = NULL;
    if (!nh_network_parseHTTPURL(Request_p->url_p, &tls, host_p, sizeof(host_p), port_p, sizeof(port_p), &target_p)) {
        nh_network_setError(Response_p, "Unsupported URL");
        return -1;
    }

    const char *method_p = Request_p->method_p ? Request_p->method_p : "GET";
    int timeoutSeconds = Request_p->timeoutSeconds > 0 ? Request_p->timeoutSeconds : 30;

    nh_network_HTTPConnection *Connection_p = calloc(1, sizeof(nh_network_HTTPConnection));
    if (!Connection_p) {
        nh_network_setError(Response_p, "Out of memory");
        return -1;
    }
    Connection_p->fd = nh_network_connectTCP(host_p, port_p, timeoutSeconds, Response_p);

    bool ok = Connection_p->fd >= 0;
    if (ok && tls) {ok = nh_network_startTLS(Connection_p, host_p, Response_p);}
    if (ok) {ok = nh_network_sendRequest(Connection_p, Request_p, method_p, host_p, port_p, tls, target_p, Response_p);}
    if (ok) {ok = nh_network_receiveResponse(Connection_p, Request_p, method_p, Response_p);}

    nh_network_closeConnection(Connection_p);
    free(Connection_p);

    return ok ? 0 : -1;
}

void nh_network_freeHTTPResponse(
    nh_api_HTTPResponse *Response_p)
{
    if (!Response_p) {return;}
    free(Response_p->headers_p);
    free(Response_p->body_p);
    Response_p->headers_p = NULL;
    Response_p->body_p = NULL;
    Response_p->bodySize = 0;
}

int nh_network_getHTTPHeader(
    const nh_api_HTTPResponse *Response_p, const char *name_p, char *value_p, size_t size)
{
    if (!Response_p || !Response_p->headers_p || !name_p || !value_p || size == 0) {return 0;}
    size_t nameLength = strlen(name_p);
    const char *line_p = Response_p->headers_p;

    while (*line_p) {
        const char *end_p = strstr(line_p, "\r\n");
        if (!end_p) {end_p = line_p + strlen(line_p);}
        if ((size_t)(end_p - line_p) > nameLength && strncasecmp(line_p, name_p, nameLength) == 0 &&
            line_p[nameLength] == ':') {
            const char *start_p = line_p + nameLength + 1;
            while (start_p < end_p && (*start_p == ' ' || *start_p == '\t')) {start_p++;}
            size_t length = (size_t)(end_p - start_p);
            while (length > 0 && (start_p[length - 1] == ' ' || start_p[length - 1] == '\t')) {length--;}
            if (length >= size) {length = size - 1;}
            memcpy(value_p, start_p, length);
            value_p[length] = '\0';
            return 1;
        }
        line_p = *end_p ? end_p + 2 : end_p;
    }
    return 0;
}

// LOOPBACK ========================================================================================

int nh_network_openLoopbackListener(
    int *port_p)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {return -1;}

    int one = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

    struct sockaddr_in Address;
    memset(&Address, 0, sizeof(Address));
    Address.sin_family = AF_INET;
    Address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    Address.sin_port = 0;

    socklen_t length = sizeof(Address);
    if (bind(fd, (struct sockaddr *)&Address, sizeof(Address)) != 0 || listen(fd, 4) != 0 ||
        getsockname(fd, (struct sockaddr *)&Address, &length) != 0) {
        close(fd);
        return -1;
    }

    if (port_p) {*port_p = ntohs(Address.sin_port);}
    return fd;
}

int nh_network_acceptLoopbackRequest(
    int listener, int timeoutSeconds, char *target_p, size_t targetSize, const char *responseHtml_p)
{
    if (listener < 0 || !target_p || targetSize == 0) {return -1;}
    target_p[0] = '\0';

    struct pollfd Poll = {.fd = listener, .events = POLLIN};
    int ready = poll(&Poll, 1, timeoutSeconds > 0 ? timeoutSeconds * 1000 : -1);
    if (ready == 0) {return 0;}
    if (ready < 0) {return errno == EINTR ? 0 : -1;}

    int fd = accept(listener, NULL, NULL);
    if (fd < 0) {return -1;}
    nh_network_setSocketTimeouts(fd, 10);

    char request_p[8192];
    size_t length = 0;
    while (length + 1 < sizeof(request_p)) {
        long read = recv(fd, request_p + length, sizeof(request_p) - length - 1, 0);
        if (read < 0 && errno == EINTR) {continue;}
        if (read <= 0) {break;}
        length += (size_t)read;
        request_p[length] = '\0';
        if (strstr(request_p, "\r\n\r\n")) {break;}
    }
    request_p[length] = '\0';

    int result = -1;
    char *space1_p = strchr(request_p, ' ');
    char *space2_p = space1_p ? strchr(space1_p + 1, ' ') : NULL;
    if (space1_p && space2_p) {
        size_t targetLength = (size_t)(space2_p - space1_p - 1);
        if (targetLength >= targetSize) {targetLength = targetSize - 1;}
        memcpy(target_p, space1_p + 1, targetLength);
        target_p[targetLength] = '\0';
        result = 1;
    }

    const char *html_p = responseHtml_p ? responseHtml_p : "";
    char head_p[256];
    snprintf(head_p, sizeof(head_p),
        "HTTP/1.1 %s\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: %zu\r\n"
        "Cache-Control: no-store\r\nConnection: close\r\n\r\n",
        result == 1 ? "200 OK" : "400 Bad Request", strlen(html_p));
    nh_network_HTTPConnection Connection = {.fd = fd};
    nh_network_writeAll(&Connection, head_p, strlen(head_p));
    nh_network_writeAll(&Connection, html_p, strlen(html_p));
    close(fd);

    return result;
}

void nh_network_closeLoopbackListener(
    int listener)
{
    if (listener >= 0) {close(listener);}
}
