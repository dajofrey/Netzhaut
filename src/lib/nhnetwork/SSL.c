// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "SSL.h"
#include "Network.h"

#include "../nhcore/Util/String.h"

#include "Common/Macros.h"

#include <stdio.h>
#include <string.h>

#ifdef __unix__
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

// DECLARE =========================================================================================

static NH_NETWORK_RESULT nh_network_createSSLContext(
);

// EXTENSIONS ======================================================================================

static int nh_network_addExtension(
    SSL *s, unsigned int ext_type, const unsigned char **out, size_t *outlen, int *al, void *add_arg)
{
}

static void nh_network_freeExtension(
    SSL *s, unsigned int ext_type, const unsigned char *out, void *add_arg)
{
}

static int nh_network_parseExtension (
    SSL *s, unsigned int ext_type, unsigned int context, const unsigned char *in, size_t inlen, X509 *x,
    size_t chainidx, int *al, void *parse_arg)
{
}

// READ / WRITE ====================================================================================

size_t nh_network_writeViaSSL(
    void *Connection_p, char *message_p, size_t messageSize)
{
NH_NETWORK_BEGIN()
NH_NETWORK_END(SSL_write(Connection_p, message_p, messageSize))
}

size_t nh_network_readViaSSL(
    void *Connection_p, char *set_p, size_t count)
{
NH_NETWORK_BEGIN()

    size_t size = SSL_read(Connection_p, set_p, count);

NH_NETWORK_END(size)
}

// CONNECT =========================================================================================

void *nh_network_connectViaSSL(
    char *hostName_p, int fd)
{
NH_NETWORK_BEGIN()

    if (nh_network_getNetwork()->SSL_p == NULL) {NH_NETWORK_CHECK(NULL, nh_network_createSSLContext())}

    SSL *Connection_p = SSL_new(nh_network_getNetwork()->SSL_p);
    NH_NETWORK_CHECK_NULL(NULL, Connection_p)

    if (SSL_set_fd(Connection_p, fd) != 1) {
        SSL_free(Connection_p); 
        NH_NETWORK_END(NULL)
    }

    SSL_set_tlsext_host_name(Connection_p, hostName_p); // Add SNI aka Client Hello extension 'host_name'

    if (SSL_connect(Connection_p) != 1) {

        unsigned long err = ERR_get_error();
        while (err != 0) {
            char buf_p[512] = {'\0'};
            printf("%s\n", ERR_error_string(err, buf_p));
            err = ERR_get_error();
        }
        SSL_free(Connection_p);
        NH_NETWORK_END(NULL)
    }

NH_NETWORK_END(Connection_p)
}

// CONTEXT =========================================================================================

static NH_NETWORK_RESULT nh_network_createSSLContext()
{
NH_NETWORK_BEGIN()

    SSL_load_error_strings();
    SSL_library_init();
    nh_network_getNetwork()->SSL_p = SSL_CTX_new(SSLv23_client_method());

//    SSL_CTX_add_client_custom_ext(
//        nh_network_getNetwork()->SSL_p, SSL_EXT_CLIENT_HELLO, nh_network_addExtension, nh_network_freeExtension, 
//        NULL, nh_network_parseExtension, NULL
//    );

NH_NETWORK_DIAGNOSTIC_END(NH_NETWORK_SUCCESS)
}

