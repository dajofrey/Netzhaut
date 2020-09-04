// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Socket.h"
#include "../Header/Macros.h"
#include "../Header/HTTP.h"
#include "../Header/SSL.h"
#include "../Header/Network.h"
#include "../Header/Log.h"

#include "../../Core/Header/Config.h"
#include "../../Core/Header/Memory.h"
#include "../../Core/Header/String.h"

#include NH_DEBUG
#include NH_UTILS
#include NH_DEFAULT_CHECK

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#ifdef __unix__
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <netdb.h>
#endif

// DECLARE =========================================================================================

typedef struct Nh_Net_ClientSocket {
    int count;
    int fd;
    void *SSL_p;
    Nh_Net_Socket Socket;
} Nh_Net_ClientSocket;

static NH_NET_PROTOCOL_FAMILY Nh_Net_convertProtocolFamily(
    int type
);
static int Nh_Net_revertProtocolFamily(
    NH_NET_PROTOCOL_FAMILY type
);

static NH_NET_SOCKET Nh_Net_convertSocketType(
    int type
);
static int Nh_Net_revertSocketType(
    NH_NET_SOCKET type
);

// INIT ============================================================================================

Nh_Net_Data Nh_Net_initData()
{
NH_BEGIN()

    Nh_Net_Data Data;
    Data.port = 0;
    Data.data_p = NULL;
    Data.size = 0;

NH_END(Data)
}

static Nh_Net_Socket Nh_Net_initSocket()
{
NH_BEGIN()

    Nh_Net_Socket Socket;
    
    Socket.hostName_p = NULL;

    Socket.type           = -1;
    Socket.family         = -1;
    Socket.protocolName_p = NULL;

    Socket.address4 = 0;
    Socket.port     = 0;    
    memset(Socket.address16_p, 0, sizeof(unsigned char) * 16);

NH_END(Socket)
}

// SERVER ==========================================================================================

NH_RESULT Nh_Net_getSockets(
    char *hostNameIn_p, char *hostNameOut_p, NH_NET_PORT port, Nh_List *List_p)
{
NH_BEGIN()

#ifdef __unix__

    char service_p[8] = {'\0'};
    sprintf(service_p, "%d", port);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family   = AF_UNSPEC;  /* Allow IPv4 or IPv6 */
    hints.ai_socktype = 0; /* Any socket */
    hints.ai_flags    = AI_CANONNAME; /* Get host-name */
    hints.ai_protocol = 0; /* Any protocol */

    struct addrinfo *Info_p = NULL;
    int s = getaddrinfo(hostNameIn_p, service_p, &hints, &Info_p);    
    if (s != 0) {NH_END(NH_ERROR_BAD_STATE)}

    for (int i = 0; Info_p != NULL; Info_p = Info_p->ai_next, ++i)
    {
        if (i == 0 && Info_p->ai_canonname != NULL && hostNameOut_p != NULL) {
            strcpy(hostNameOut_p, Info_p->ai_canonname);
        }

        if (Info_p->ai_addr == NULL) {continue;}
        
        Nh_Net_Socket *Socket_p = Nh_allocate(sizeof(Nh_Net_Socket));
        NH_CHECK_MEM(Socket_p)

        *Socket_p = Nh_Net_initSocket();

        Socket_p->family = Nh_Net_convertProtocolFamily(Info_p->ai_family);
        Socket_p->type   = Nh_Net_convertSocketType(Info_p->ai_socktype);

        struct protoent *Proto_p = getprotobynumber(Info_p->ai_protocol);
        Socket_p->protocolName_p = Nh_allocateChars(Proto_p->p_name);
        endprotoent();

        switch (Socket_p->family) 
        {
            case NH_NET_PROTOCOL_FAMILY_IPV4 :
            {
                struct sockaddr_in *Addr_p = (struct sockaddr_in*)Info_p->ai_addr;
                memset(Addr_p->sin_zero, 0, sizeof(char) * 8);
    
                Socket_p->port = ntohs(Addr_p->sin_port);
                Socket_p->address4 = Addr_p->sin_addr.s_addr;

                char hbuf[NI_MAXHOST] = {'\0'};
                if (getnameinfo((struct sockaddr *) Addr_p, sizeof(struct sockaddr_in), hbuf, sizeof(hbuf), NULL, 0, NI_NAMEREQD) == 0) {
                    Socket_p->hostName_p = Nh_allocateChars(hbuf);
                }
            
                break;
            }
            case NH_NET_PROTOCOL_FAMILY_IPV6 :
            {
                struct sockaddr_in6 *Addr_p = (struct sockaddr_in6*)Info_p->ai_addr;
    
                Socket_p->port = ntohs(Addr_p->sin6_port);
                memcpy(Socket_p->address16_p, Addr_p->sin6_addr.s6_addr, sizeof(unsigned char) * 16);

                char hbuf[NI_MAXHOST] = {'\0'};
                if (getnameinfo((struct sockaddr *) Addr_p, sizeof(struct sockaddr_in6), hbuf, sizeof(hbuf), NULL, 0, NI_NAMEREQD) == 0) {
                    Socket_p->hostName_p = Nh_allocateChars(hbuf);
                }

                break;
            }
        }
        NH_CHECK(Nh_addListItem(List_p, Socket_p))
    } 

    freeaddrinfo(Info_p);

#endif

NH_END(NH_SUCCESS)
}

// CLIENT ==========================================================================================

Nh_Net_ClientSocket *Nh_Net_getClientSocket(
    Nh_Net_Socket Socket)
{
NH_BEGIN()

    for (int i = 0; i < Nh_Net_getNetwork()->Sockets.count; ++i) // TODO multi threading
    {
        Nh_Net_ClientSocket *Tmp_p = Nh_getListItem(&Nh_Net_getNetwork()->Sockets, i);
        if (Tmp_p->Socket.family == Socket.family && Tmp_p->Socket.type == Socket.type) {
            Tmp_p->count++;
            NH_END(Tmp_p)
            break;
        }
    }

NH_END(NULL)
}

Nh_Net_ClientSocket *Nh_Net_createClientSocket(
    NH_NET_PROTOCOL_FAMILY family, NH_NET_SOCKET type)
{
NH_BEGIN()

#ifdef __unix__    

    char *protocolName_p = NULL;

    switch (type)
    {
        case NH_NET_SOCKET_TCP : protocolName_p = "tcp"; break;
        case NH_NET_SOCKET_UDP : protocolName_p = "udp"; break;
        case NH_NET_SOCKET_RAW : protocolName_p = "hopopt"; break;
    }

    struct protoent *Proto_p = getprotobyname(protocolName_p);
    int fd = socket(Nh_Net_revertProtocolFamily(family), Nh_Net_revertSocketType(type), Proto_p->p_proto);
    endprotoent();

#endif

#include NH_CUSTOM_CHECK

    Nh_Net_ClientSocket *Socket_p = Nh_allocate(sizeof(Nh_Net_ClientSocket));
    NH_CHECK_NULL(NULL, Socket_p)

    Socket_p->count = 0;
    Socket_p->fd = fd;
    Socket_p->SSL_p = NULL;

    Nh_Net_Socket Socket = Nh_Net_initSocket();
    Socket.family = family;
    Socket.type = type;

    memcpy(&Socket_p->Socket, &Socket, sizeof(Nh_Net_Socket));

#include NH_DEFAULT_CHECK

NH_END(Socket_p)
}

// FREE / CLOSE ====================================================================================

NH_RESULT Nh_Net_closeClientSocket(
    Nh_Net_ClientSocket *Socket_p)
{
NH_BEGIN()

//    Socket_p->count--;
//    if (Socket_p->count > 0) {NH_END(NH_SUCCESS)} // socket is still in use
//
//    Nh_free(Socket_p->Socket.protocolName_p);
//    Nh_free(Socket_p->Socket.hostName_p);
//
//#ifdef __unix__
//
//    int result = close(Socket_p->fd);
//    if (result != 0) {NH_END(NH_ERROR_BAD_STATE)}
//
//#endif
//
//    Nh_destroyListItemFromPointer(&Nh_Net_getNetwork()->Sockets, Socket_p, true);

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_Net_freeSockets(
    Nh_List *Sockets_p)
{
NH_BEGIN()

    for (int i = 0; i < Sockets_p->count; ++i) {
        Nh_Net_Socket *Socket_p = Nh_getListItem(Sockets_p, i);
        Nh_free(Socket_p->protocolName_p);
        Nh_free(Socket_p->hostName_p);
    }

    Nh_destroyList(Sockets_p, true);

NH_END(NH_SUCCESS)
}

// CONNECT =========================================================================================

NH_RESULT Nh_Net_connect(
    Nh_Net_ClientSocket *ClientSocket_p, Nh_Net_Socket Socket)
{
NH_BEGIN()

    if (ClientSocket_p == NULL || ClientSocket_p->Socket.family != Socket.family || ClientSocket_p->Socket.type != Socket.type) {
        NH_END(NH_ERROR_BAD_STATE)
    }

#ifdef __unix__

    switch (Socket.family)
    {
        case NH_NET_PROTOCOL_FAMILY_IPV4 :
        {
            struct sockaddr_in Addr = {0};
            Addr.sin_family = Nh_Net_revertProtocolFamily(Socket.family); 
            Addr.sin_port = htons(Socket.port);
            Addr.sin_addr.s_addr = Socket.address4;
            memset(Addr.sin_zero, 0, sizeof(char) * 8);

            struct sockaddr Reset;
            Reset.sa_family = AF_UNSPEC;

            if (connect(ClientSocket_p->fd, (const struct sockaddr*) &Reset, sizeof(struct sockaddr)) != 0) {
                NH_END(NH_ERROR_BAD_STATE)
            }
            if (connect(ClientSocket_p->fd, (const struct sockaddr*) &Addr, sizeof(struct sockaddr_in)) != 0) {
                NH_END(NH_ERROR_BAD_STATE)
            }

            break;
        }
        case NH_NET_PROTOCOL_FAMILY_IPV6 :
        {
        }
        default : NH_END(NH_ERROR_BAD_STATE)
    }

    // get client port    
    struct sockaddr_in ClientInfo;
    socklen_t len = sizeof(ClientInfo);
    if (getsockname(ClientSocket_p->fd, (struct sockaddr *)&ClientInfo, &len) == 0) {
        ClientSocket_p->Socket.port = ntohs(ClientInfo.sin_port);
    }

#endif

    if (Socket.type == NH_NET_SOCKET_TCP && Socket.port == NH_NET_PORT_HTTPS) {
        ClientSocket_p->SSL_p = Nh_Net_connectViaSSL(ClientSocket_p->fd);
        if (ClientSocket_p->SSL_p == NULL) {NH_END(NH_ERROR_BAD_STATE)}
    }

NH_END(NH_SUCCESS)
}

// SEND / RECEIVE ==================================================================================

NH_RESULT Nh_Net_send(
    Nh_Net_ClientSocket *ClientSocket_p, char *message_p, size_t messageSize, NH_BOOL secure)
{
NH_BEGIN()

    size_t result = 0;

    if (secure == NH_FALSE) {
#ifdef __unix__
        result = write(ClientSocket_p->fd, message_p, messageSize);
#endif
    }
    else {
        result = Nh_Net_writeViaSSL(ClientSocket_p->SSL_p, message_p, messageSize); 
    }

NH_END(messageSize == result ? NH_SUCCESS : NH_ERROR_BAD_STATE)
}

ssize_t Nh_Net_receive(
    Nh_Net_ClientSocket *ClientSocket_p, char *set_p, ssize_t count, NH_BOOL secure)
{
NH_BEGIN()

    ssize_t size = 0;

    if (secure == NH_FALSE) {
#ifdef __unix__
        size = read(ClientSocket_p->fd, set_p, count);
#endif
    }
    else {
        size = Nh_Net_readViaSSL(ClientSocket_p->SSL_p, set_p, count);
    }

    if (size > 0) {Nh_Net_logReceive(set_p);}

NH_END(size)
}

// DATA ============================================================================================

Nh_Net_Data Nh_Net_getDataFromSocket(
    Nh_Net_ClientSocket *Socket_p, char *hostName_p, char *pathName_p, NH_NET_PORT port)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    Nh_Net_Data Data;
    Data.port = port;
    Data.data_p = NULL;
    Data.size = 0;

    NH_BOOL secure = NH_FALSE;
    if (port == NH_NET_PORT_HTTPS) {secure = NH_TRUE;}

    switch (port)
    {
        case NH_NET_PORT_HTTP  :  
        case NH_NET_PORT_HTTPS : NH_CHECK(Data, Nh_Net_sendDataRequestViaHTTP(Socket_p, hostName_p, pathName_p, secure)) break; 
        default : NH_END(Data)
    }

    Nh_String *String_p = Nh_allocateString(NULL);
    NH_CHECK_NULL(Data, String_p)

    ssize_t size = 0;
    do {
        char set_p[2049] = {'\0'};
        size = Nh_Net_receive(Socket_p, set_p, 2048, secure);
        if (size > 0) {
            NH_CHECK(Data, Nh_appendToString(String_p, set_p))
            Data.size += size; 
        }
    }
    while (size > 0);

    Data.data_p = Nh_getChars(String_p);
    Nh_freeString(String_p, false);

#include NH_DEFAULT_CHECK

NH_END(Data)
}

// STRINGIFY =======================================================================================

void Nh_Net_stringifySocketAddress(
    Nh_Net_Socket *Socket_p, char *set_p)
{
NH_BEGIN()
    
    Nh_String *String_p = Nh_allocateString(NULL);

    switch (Socket_p->family)
    {
        case NH_NET_PROTOCOL_FAMILY_IPV4 :
        {
            for (int i = 0; i < 4; ++i) {
                if (i < 3) {Nh_appendFormatToString(String_p, "%d.", (Socket_p->address4 >> (8*i)) & 0xff);}
                else {Nh_appendFormatToString(String_p, "%d", (Socket_p->address4 >> (8*i)) & 0xff);}
            }
            break;
        }
        case NH_NET_PROTOCOL_FAMILY_IPV6 :
        {
            for (int i = 0; i < 16; ++i) {
                if ((i % 2) == 0 || i == 0 || i == 15) {Nh_appendFormatToString(String_p, "%02x", (int)Socket_p->address16_p[i]);}
                else if ((i % 2) == 1 && i != 0) {Nh_appendFormatToString(String_p, "%02x:", (int)Socket_p->address16_p[i]);}
            }
            break;
        }
    }

    strcpy(set_p, Nh_getChars(String_p));
    Nh_freeString(String_p, true);

NH_SILENT_END()
}

void Nh_Net_stringifySocket(
    Nh_Net_Socket *Socket_p, char *set_p)
{
NH_BEGIN()

    char addr_p[255] = {'\0'};
    Nh_Net_stringifySocketAddress(Socket_p, addr_p);

    Nh_String *String_p = Nh_allocateString(NULL);
    Nh_appendFormatToString(String_p, "%s %s %s %d", Socket_p->hostName_p, Socket_p->protocolName_p, addr_p, Socket_p->port);

    strcpy(set_p, Nh_getChars(String_p));
    Nh_freeString(String_p, true);

NH_SILENT_END()
}

// HELPER ==========================================================================================

static NH_NET_PROTOCOL_FAMILY Nh_Net_convertProtocolFamily(
    int type)
{
NH_BEGIN()

#ifdef __unix__

    switch (type)
    {
        case AF_INET  : NH_END(NH_NET_PROTOCOL_FAMILY_IPV4)
        case AF_INET6 : NH_END(NH_NET_PROTOCOL_FAMILY_IPV6) 
    }

#endif

NH_END(-1)
}

static int Nh_Net_revertProtocolFamily(
    NH_NET_PROTOCOL_FAMILY type)
{
NH_BEGIN()

#ifdef __unix__

    switch (type)
    {
        case NH_NET_PROTOCOL_FAMILY_IPV4 : NH_END(AF_INET)
        case NH_NET_PROTOCOL_FAMILY_IPV6 : NH_END(AF_INET6) 
    }

#endif

NH_END(-1)
}

static NH_NET_SOCKET Nh_Net_convertSocketType(
    int type)
{
NH_BEGIN()

#ifdef __unix__

    switch (type)
    {
        case SOCK_STREAM    : NH_END(NH_NET_SOCKET_TCP)
        case SOCK_DGRAM     : NH_END(NH_NET_SOCKET_UDP) 
        case SOCK_SEQPACKET : NH_END(NH_NET_SOCKET_SEQPACKET)
        case SOCK_RAW       : NH_END(NH_NET_SOCKET_RAW)
        case SOCK_RDM       : NH_END(NH_NET_SOCKET_RDM)
    }

#endif

NH_END(-1)
}

static int Nh_Net_revertSocketType(
    NH_NET_SOCKET type)
{
NH_BEGIN()

#ifdef __unix__

    switch (type)
    {
        case NH_NET_SOCKET_TCP       : NH_END(SOCK_STREAM)
        case NH_NET_SOCKET_UDP       : NH_END(SOCK_DGRAM) 
        case NH_NET_SOCKET_SEQPACKET : NH_END(SOCK_SEQPACKET)
        case NH_NET_SOCKET_RAW       : NH_END(SOCK_RAW)
        case NH_NET_SOCKET_RDM       : NH_END(SOCK_RDM)
    }

#endif

NH_END(-1)
}

