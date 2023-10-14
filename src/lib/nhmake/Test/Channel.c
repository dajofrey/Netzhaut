// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Channel.h"

#include "../Common/Macros.h"

#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// INIT ============================================================================================

NH_MAKE_RESULT nh_make_initChannel(
    nh_make_Channel *Channel_p)
{
NH_MAKE_BEGIN()

#if defined(__linux__) || defined(__APPLE__)
    Channel_p->rw_p[0] = 0;
    Channel_p->rw_p[1] = 0;
#endif

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

NH_MAKE_RESULT nh_make_openChannel(
    nh_make_Channel *Channel_p)
{
NH_MAKE_BEGIN()

#if defined(__linux__) || defined(__APPLE__)
    pipe(Channel_p->rw_p);
#endif

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

void nh_make_closeChannelReadAccess(
    nh_make_Channel *Channel_p)
{
NH_MAKE_BEGIN()

#if defined(__linux__) || defined(__APPLE__)
    close(Channel_p->rw_p[0]);
#endif

NH_MAKE_SILENT_END()
}

void nh_make_closeChannelWriteAccess(
    nh_make_Channel *Channel_p)
{
NH_MAKE_BEGIN()

#if defined(__linux__) || defined(__APPLE__)
    close(Channel_p->rw_p[1]);
#endif

NH_MAKE_SILENT_END()
}

int nh_make_writeToChannel(
    nh_make_Channel *Channel_p, char *bytes_p, int byteCount)
{
NH_MAKE_BEGIN()

#if defined(__linux__) || defined(__APPLE__)
    int result = write(Channel_p->rw_p[1], bytes_p, byteCount);
#endif

NH_MAKE_END(result)
}

char *nh_make_readFromChannel(
    nh_make_Channel *Channel_p, size_t *size_p)
{
NH_MAKE_BEGIN()

#if defined(__linux__) || defined(__APPLE__)

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1;

    int r = Channel_p->rw_p[0];

    fd_set set;
    FD_ZERO(&set);
    FD_SET(r, &set);

    int rv = select(r + 1, &set, NULL, NULL, &timeout);
    if (rv > 0)
    {
        int offset = 0;
        char *buff_p = malloc(128);
        memset(buff_p, '\0', 128);
        int result;
        while (result = read(r, buff_p + offset, 128)) {
            if (size_p != NULL) {*size_p += result;}
            if (result < 128) {break;}
            offset += 128;
            buff_p = realloc(buff_p, offset + 128);
            memset(buff_p + offset, '\0', 128);
        }
        NH_MAKE_END(buff_p);
    }

#endif

NH_MAKE_END(NULL)
}

