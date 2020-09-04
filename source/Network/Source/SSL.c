// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/SSL.h"
#include "../Header/Macros.h"
#include "../Header/Network.h"

#include "../../Core/Header/String.h"

#include NH_DEBUG
#include NH_CUSTOM_CHECK
#include NH_UTILS

#include <stdio.h>
#include <string.h>

#ifdef __unix__
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

// DECLARE =========================================================================================

static NH_RESULT Nh_Net_createSSLContext(
);

// EXTENSIONS ======================================================================================

static int Nh_Net_addExtension(
    SSL *s, unsigned int ext_type, const unsigned char **out, size_t *outlen, int *al, void *add_arg)
{
}

static void Nh_Net_freeExtension(
    SSL *s, unsigned int ext_type, const unsigned char *out, void *add_arg)
{
}

static int Nh_Net_parseExtension (
    SSL *s, unsigned int ext_type, unsigned int context, const unsigned char *in, size_t inlen, X509 *x,
    size_t chainidx, int *al, void *parse_arg)
{
}

// READ / WRITE ====================================================================================

size_t Nh_Net_writeViaSSL(
    void *Connection_p, char *message_p, size_t messageSize)
{
NH_BEGIN()
NH_END(SSL_write(Connection_p, message_p, messageSize))
}

size_t Nh_Net_readViaSSL(
    void *Connection_p, char *set_p, size_t count)
{
NH_BEGIN()

    size_t size = SSL_read(Connection_p, set_p, count);

NH_END(size)
}

// CONNECT =========================================================================================

void *Nh_Net_connectViaSSL(
    int fd)
{
NH_BEGIN()

    if (Nh_Net_getNetwork()->SSL_p == NULL) {NH_CHECK(NULL, Nh_Net_createSSLContext())}

    SSL *Connection_p = SSL_new(Nh_Net_getNetwork()->SSL_p);
    NH_CHECK_NULL(NULL, Connection_p)

    if (SSL_set_fd(Connection_p, fd) != 1) {
        SSL_free(Connection_p); 
        NH_END(NULL)
    }

    SSL_set_tlsext_host_name(Connection_p, "www.york.ac.uk"); // Add SNI aka Client Hello extension 'host_name'

    if (SSL_connect(Connection_p) != 1) {

        unsigned long err = ERR_get_error();
        while (err != 0) {
            char buf_p[512] = {'\0'};
            printf("%s\n", ERR_error_string(err, buf_p));
            err = ERR_get_error();
        }
        SSL_free(Connection_p);
        NH_END(NULL)
    }

NH_END(Connection_p)
}

// CONTEXT =========================================================================================

static NH_RESULT Nh_Net_createSSLContext()
{
NH_BEGIN()

    SSL_load_error_strings();
    SSL_library_init();
    Nh_Net_getNetwork()->SSL_p = SSL_CTX_new(SSLv23_client_method());

//    SSL_CTX_add_client_custom_ext(
//        Nh_Net_getNetwork()->SSL_p, SSL_EXT_CLIENT_HELLO, Nh_Net_addExtension, Nh_Net_freeExtension, 
//        NULL, Nh_Net_parseExtension, NULL
//    );

NH_END(NH_SUCCESS)
}

