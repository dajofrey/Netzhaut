// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Channel.h"

#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// INIT ============================================================================================

NH_API_RESULT nh_core_initChannel(
    nh_Channel *Channel_p)
{
#ifdef __unix__
    Channel_p->rw_p[0] = 0;
    Channel_p->rw_p[1] = 0;
#endif
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_openChannel(
    nh_Channel *Channel_p)
{
#ifdef __unix__
    pipe(Channel_p->rw_p);
#endif
    return NH_API_SUCCESS;
}

void nh_closeChannelReadAccess(
    nh_Channel *Channel_p)
{
#ifdef __unix__
    close(Channel_p->rw_p[0]);
#endif

    return;
}

void nh_closeChannelWriteAccess(
    nh_Channel *Channel_p)
{
#ifdef __unix__
    close(Channel_p->rw_p[1]);
#endif
    return;
}

int nh_core_writeToChannel(
    nh_Channel *Channel_p, char *bytes_p, int byteCount)
{
#ifdef __unix__
    int result = write(Channel_p->rw_p[1], bytes_p, byteCount);
#endif
    return result;
}

char *nh_readFromChannel(
    nh_Channel *Channel_p, size_t *size_p)
{
#ifdef __unix__

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
        return buff_p;;
    }

#endif

    return NULL;
}
