// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Socket.h"
#include "HTTP.h"
#include "SSL.h"
#include "Network.h"

#include "../nh-core/System/Memory.h"
#include "../nh-core/Util/String.h"

#include "../nh-core/Common/Macros.h"
#include NH_FLOW
#include NH_CUSTOM_CHECK

#include "Common/Log.h"
#include "Common/Macros.h"

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

typedef struct nh_network_ClientSocket {
    int count;
    int fd;
    void *SSL_p;
    nh_network_Socket Socket;
} nh_network_ClientSocket;

static NH_NETWORK_PROTOCOL_FAMILY nh_network_convertProtocolFamily(
    int type
);
static int nh_network_revertProtocolFamily(
    NH_NETWORK_PROTOCOL_FAMILY type
);

static NH_NETWORK_SOCKET nh_network_convertSocketType(
    int type
);
static int nh_network_revertSocketType(
    NH_NETWORK_SOCKET type
);

// INIT ============================================================================================

nh_network_Data nh_network_initData()
{
NH_NETWORK_BEGIN()

    nh_network_Data Data;
    Data.port = 0;
    Data.data_p = NULL;
    Data.size = 0;

NH_NETWORK_END(Data)
}

static nh_network_Socket nh_network_initSocket()
{
NH_NETWORK_BEGIN()

    nh_network_Socket Socket;
    
    Socket.hostName_p = NULL;

    Socket.type           = -1;
    Socket.family         = -1;
    Socket.protocolName_p = NULL;

    Socket.address4 = 0;
    Socket.port     = 0;    
    memset(Socket.address16_p, 0, sizeof(unsigned char) * 16);

NH_NETWORK_END(Socket)
}

// SERVER ==========================================================================================

NH_API_RESULT nh_network_getSockets(
    char *hostNameIn_p, char *hostNameOut_p, NH_NETWORK_PORT port, nh_LinkedList *List_p)
{
NH_NETWORK_BEGIN()

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
    if (s != 0) {NH_NETWORK_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)}

    for (int i = 0; Info_p != NULL; Info_p = Info_p->ai_next, ++i)
    {
        if (i == 0 && Info_p->ai_canonname != NULL && hostNameOut_p != NULL) {
            strcpy(hostNameOut_p, Info_p->ai_canonname);
        }

        if (Info_p->ai_addr == NULL) {continue;}
        
        nh_network_Socket *Socket_p = nh_core_allocate(sizeof(nh_network_Socket));
        NH_CORE_CHECK_MEM(Socket_p)

        *Socket_p = nh_network_initSocket();

        Socket_p->family = nh_network_convertProtocolFamily(Info_p->ai_family);
        Socket_p->type   = nh_network_convertSocketType(Info_p->ai_socktype);

        struct protoent *Proto_p = getprotobynumber(Info_p->ai_protocol);
        Socket_p->protocolName_p = nh_core_allocateBytes(Proto_p->p_name);
        endprotoent();

        switch (Socket_p->family) 
        {
            case NH_NETWORK_PROTOCOL_FAMILY_IPV4 :
            {
                struct sockaddr_in *Addr_p = (struct sockaddr_in*)Info_p->ai_addr;
                memset(Addr_p->sin_zero, 0, sizeof(char) * 8);
    
                Socket_p->port = ntohs(Addr_p->sin_port);
                Socket_p->address4 = Addr_p->sin_addr.s_addr;

                char hbuf[NI_MAXHOST] = {'\0'};
                if (getnameinfo((struct sockaddr *) Addr_p, sizeof(struct sockaddr_in), hbuf, sizeof(hbuf), NULL, 0, NI_NAMEREQD) == 0) {
                    Socket_p->hostName_p = nh_core_allocateBytes(hbuf);
                }
            
                break;
            }
            case NH_NETWORK_PROTOCOL_FAMILY_IPV6 :
            {
                struct sockaddr_in6 *Addr_p = (struct sockaddr_in6*)Info_p->ai_addr;
    
                Socket_p->port = ntohs(Addr_p->sin6_port);
                memcpy(Socket_p->address16_p, Addr_p->sin6_addr.s6_addr, sizeof(unsigned char) * 16);

                char hbuf[NI_MAXHOST] = {'\0'};
                if (getnameinfo((struct sockaddr *) Addr_p, sizeof(struct sockaddr_in6), hbuf, sizeof(hbuf), NULL, 0, NI_NAMEREQD) == 0) {
                    Socket_p->hostName_p = nh_core_allocateBytes(hbuf);
                }

                break;
            }
        }
        NH_CHECK(NH_API_ERROR_BAD_STATE, nh_core_appendToLinkedList(List_p, Socket_p))
    } 

    freeaddrinfo(Info_p);

#endif

NH_NETWORK_DIAGNOSTIC_END(NH_API_SUCCESS)
}

// CLIENT ==========================================================================================

nh_network_ClientSocket *nh_network_getClientSocket(
    nh_network_Socket Socket)
{
NH_NETWORK_BEGIN()

    for (int i = 0; i < nh_network_getNetwork()->Sockets.count; ++i) // TODO multi threading
    {
        nh_network_ClientSocket *Tmp_p = nh_core_getFromLinkedList(&nh_network_getNetwork()->Sockets, i);
        if (Tmp_p->Socket.family == Socket.family && Tmp_p->Socket.type == Socket.type) {
            Tmp_p->count++;
            NH_NETWORK_END(Tmp_p)
            break;
        }
    }

NH_NETWORK_END(NULL)
}

nh_network_ClientSocket *nh_network_createClientSocket(
    NH_NETWORK_PROTOCOL_FAMILY family, NH_NETWORK_SOCKET type)
{
NH_NETWORK_BEGIN()

#ifdef __unix__    

    char *protocolName_p = NULL;

    switch (type)
    {
        case NH_NETWORK_SOCKET_TCP : protocolName_p = "tcp"; break;
        case NH_NETWORK_SOCKET_UDP : protocolName_p = "udp"; break;
        case NH_NETWORK_SOCKET_RAW : protocolName_p = "hopopt"; break;
    }

    struct protoent *Proto_p = getprotobyname(protocolName_p);
    int fd = socket(nh_network_revertProtocolFamily(family), nh_network_revertSocketType(type), Proto_p->p_proto);
    endprotoent();

#endif

    nh_network_ClientSocket *Socket_p = nh_core_allocate(sizeof(nh_network_ClientSocket));
    NH_CORE_CHECK_NULL_2(NULL, Socket_p)

    Socket_p->count = 0;
    Socket_p->fd = fd;
    Socket_p->SSL_p = NULL;

    nh_network_Socket Socket = nh_network_initSocket();
    Socket.family = family;
    Socket.type = type;

    memcpy(&Socket_p->Socket, &Socket, sizeof(nh_network_Socket));

NH_NETWORK_END(Socket_p)
}

// FREE / CLOSE ====================================================================================

NH_API_RESULT nh_network_closeClientSocket(
    nh_network_ClientSocket *Socket_p)
{
NH_NETWORK_BEGIN()

//    Socket_p->count--;
//    if (Socket_p->count > 0) {NH_NETWORK_DIAGNOSTIC_END(NH_API_SUCCESS)} // socket is still in use
//
//    nh_core_free(Socket_p->Socket.protocolName_p);
//    nh_core_free(Socket_p->Socket.hostName_p);
//
//#ifdef __unix__
//
//    int result = close(Socket_p->fd);
//    if (result != 0) {NH_NETWORK_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)}
//
//#endif
//
//    nh_core_removeFromLinkedList2(&nh_network_getNetwork()->Sockets, Socket_p, true);

NH_NETWORK_DIAGNOSTIC_END(NH_API_SUCCESS)
}

NH_API_RESULT nh_network_freeSockets(
    nh_LinkedList *Sockets_p)
{
NH_NETWORK_BEGIN()

    for (int i = 0; i < Sockets_p->count; ++i) {
        nh_network_Socket *Socket_p = nh_core_getFromLinkedList(Sockets_p, i);
        nh_core_free(Socket_p->protocolName_p);
        nh_core_free(Socket_p->hostName_p);
    }

    nh_core_destroyLinkedList(Sockets_p, true);

NH_NETWORK_DIAGNOSTIC_END(NH_API_SUCCESS)
}

// CONNECT =========================================================================================

NH_API_RESULT nh_network_connect(
    char *hostName_p, nh_network_ClientSocket *ClientSocket_p, nh_network_Socket Socket)
{
NH_NETWORK_BEGIN()

    if (ClientSocket_p == NULL || ClientSocket_p->Socket.family != Socket.family || ClientSocket_p->Socket.type != Socket.type) {
        NH_NETWORK_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)
    }

#ifdef __unix__

    switch (Socket.family)
    {
        case NH_NETWORK_PROTOCOL_FAMILY_IPV4 :
        {
            struct sockaddr_in Addr = {0};
            Addr.sin_family = nh_network_revertProtocolFamily(Socket.family); 
            Addr.sin_port = htons(Socket.port);
            Addr.sin_addr.s_addr = Socket.address4;
            memset(Addr.sin_zero, 0, sizeof(char) * 8);

            struct sockaddr Reset;
            Reset.sa_family = AF_UNSPEC;

            if (connect(ClientSocket_p->fd, (const struct sockaddr*) &Reset, sizeof(struct sockaddr)) != 0) {
                NH_NETWORK_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)
            }
            if (connect(ClientSocket_p->fd, (const struct sockaddr*) &Addr, sizeof(struct sockaddr_in)) != 0) {
                NH_NETWORK_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)
            }

            break;
        }
        case NH_NETWORK_PROTOCOL_FAMILY_IPV6 :
        {
        }
        default : NH_NETWORK_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)
    }

    // get client port    
    struct sockaddr_in ClientInfo;
    socklen_t len = sizeof(ClientInfo);
    if (getsockname(ClientSocket_p->fd, (struct sockaddr *)&ClientInfo, &len) == 0) {
        ClientSocket_p->Socket.port = ntohs(ClientInfo.sin_port);
    }

#endif

    if (Socket.type == NH_NETWORK_SOCKET_TCP && Socket.port == NH_NETWORK_PORT_HTTPS) {
        ClientSocket_p->SSL_p = nh_network_connectViaSSL(hostName_p, ClientSocket_p->fd);
        if (ClientSocket_p->SSL_p == NULL) {NH_NETWORK_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)}
    }

NH_NETWORK_DIAGNOSTIC_END(NH_API_SUCCESS)
}

// SEND / RECEIVE ==================================================================================

NH_API_RESULT nh_network_send(
    nh_network_ClientSocket *ClientSocket_p, char *message_p, size_t messageSize, bool secure)
{
NH_NETWORK_BEGIN()

    size_t result = 0;

    if (messageSize > 0) {nh_network_logSend(message_p);}

    if (secure == false) {
#ifdef __unix__
        result = write(ClientSocket_p->fd, message_p, messageSize);
#endif
    }
    else {
        result = nh_network_writeViaSSL(ClientSocket_p->SSL_p, message_p, messageSize); 
    }

NH_NETWORK_END(messageSize == result ? NH_API_SUCCESS : NH_API_ERROR_BAD_STATE)
}

ssize_t nh_network_receive(
    nh_network_ClientSocket *ClientSocket_p, char *set_p, ssize_t count, bool secure)
{
NH_NETWORK_BEGIN()

    ssize_t size = 0;

    if (secure == false) {
#ifdef __unix__
        size = read(ClientSocket_p->fd, set_p, count);
#endif
    }
    else {
        size = nh_network_readViaSSL(ClientSocket_p->SSL_p, set_p, count);
    }

    if (size > 0) {nh_network_logReceive(set_p);}

NH_NETWORK_END(size)
}

// DATA ============================================================================================

nh_network_Data nh_network_getDataFromSocket(
    nh_network_ClientSocket *Socket_p, char *hostName_p, char *pathName_p, NH_NETWORK_PORT port)
{
NH_NETWORK_BEGIN()

    nh_network_Data Data;
    Data.port = port;
    Data.data_p = NULL;
    Data.size = 0;

    bool secure = false;
    if (port == NH_NETWORK_PORT_HTTPS) {secure = true;}

    switch (port)
    {
        case NH_NETWORK_PORT_HTTP  :  
        case NH_NETWORK_PORT_HTTPS : NH_CORE_CHECK_2(Data, nh_network_sendDataRequestViaHTTP(Socket_p, hostName_p, pathName_p, secure)) break; 
        default : NH_NETWORK_END(Data)
    }

    nh_core_String String = nh_core_initString(512); 

    ssize_t size = 0;
    do {
        char set_p[2049] = {'\0'};
        size = nh_network_receive(Socket_p, set_p, 2048, secure);
        if (size > 0) {
            NH_CHECK(Data, nh_core_appendFormatToString(&String, set_p))
            Data.size += size; 
        }
    }
    while (size > 0);

    Data.data_p = String.bytes_p;

NH_NETWORK_END(Data)
}

// STRINGIFY =======================================================================================

void nh_network_stringifySocketAddress(
    nh_network_Socket *Socket_p, char *set_p)
{
NH_NETWORK_BEGIN()
    
    nh_core_String String = nh_core_initString(128); 

    switch (Socket_p->family)
    {
        case NH_NETWORK_PROTOCOL_FAMILY_IPV4 :
        {
            for (int i = 0; i < 4; ++i) {
                if (i < 3) {nh_core_appendFormatToString(&String, "%d.", (Socket_p->address4 >> (8*i)) & 0xff);}
                else {nh_core_appendFormatToString(&String, "%d", (Socket_p->address4 >> (8*i)) & 0xff);}
            }
            break;
        }
        case NH_NETWORK_PROTOCOL_FAMILY_IPV6 :
        {
            for (int i = 0; i < 16; ++i) {
                if ((i % 2) == 0 || i == 0 || i == 15) {nh_core_appendFormatToString(&String, "%02x", (int)Socket_p->address16_p[i]);}
                else if ((i % 2) == 1 && i != 0) {nh_core_appendFormatToString(&String, "%02x:", (int)Socket_p->address16_p[i]);}
            }
            break;
        }
    }

    strcpy(set_p, String.bytes_p);
    nh_core_freeString(&String);

NH_NETWORK_SILENT_END()
}

void nh_network_stringifySocket(
    nh_network_Socket *Socket_p, char *set_p)
{
NH_NETWORK_BEGIN()

    nh_core_String String = nh_core_initString(128); 

    char addr_p[255] = {'\0'};
    nh_network_stringifySocketAddress(Socket_p, addr_p);

    nh_core_appendFormatToString(&String, "%s %s %s %d", Socket_p->hostName_p, Socket_p->protocolName_p, addr_p, Socket_p->port);

    strcpy(set_p, String.bytes_p);
    nh_core_freeString(&String);

NH_NETWORK_SILENT_END()
}

// HELPER ==========================================================================================

static NH_NETWORK_PROTOCOL_FAMILY nh_network_convertProtocolFamily(
    int type)
{
NH_NETWORK_BEGIN()

#ifdef __unix__

    switch (type)
    {
        case AF_INET  : NH_NETWORK_DIAGNOSTIC_END(NH_NETWORK_PROTOCOL_FAMILY_IPV4)
        case AF_INET6 : NH_NETWORK_DIAGNOSTIC_END(NH_NETWORK_PROTOCOL_FAMILY_IPV6) 
    }

#endif

NH_NETWORK_END(-1)
}

static int nh_network_revertProtocolFamily(
    NH_NETWORK_PROTOCOL_FAMILY type)
{
NH_NETWORK_BEGIN()

#ifdef __unix__

    switch (type)
    {
        case NH_NETWORK_PROTOCOL_FAMILY_IPV4 : NH_NETWORK_END(AF_INET)
        case NH_NETWORK_PROTOCOL_FAMILY_IPV6 : NH_NETWORK_END(AF_INET6) 
    }

#endif

NH_NETWORK_END(-1)
}

static NH_NETWORK_SOCKET nh_network_convertSocketType(
    int type)
{
NH_NETWORK_BEGIN()

#ifdef __unix__

    switch (type)
    {
        case SOCK_STREAM    : NH_NETWORK_DIAGNOSTIC_END(NH_NETWORK_SOCKET_TCP)
        case SOCK_DGRAM     : NH_NETWORK_DIAGNOSTIC_END(NH_NETWORK_SOCKET_UDP) 
        case SOCK_SEQPACKET : NH_NETWORK_DIAGNOSTIC_END(NH_NETWORK_SOCKET_SEQPACKET)
        case SOCK_RAW       : NH_NETWORK_DIAGNOSTIC_END(NH_NETWORK_SOCKET_RAW)
        case SOCK_RDM       : NH_NETWORK_DIAGNOSTIC_END(NH_NETWORK_SOCKET_RDM)
    }

#endif

NH_NETWORK_END(-1)
}

static int nh_network_revertSocketType(
    NH_NETWORK_SOCKET type)
{
NH_NETWORK_BEGIN()

#ifdef __unix__

    switch (type)
    {
        case NH_NETWORK_SOCKET_TCP       : NH_NETWORK_END(SOCK_STREAM)
        case NH_NETWORK_SOCKET_UDP       : NH_NETWORK_END(SOCK_DGRAM) 
        case NH_NETWORK_SOCKET_SEQPACKET : NH_NETWORK_END(SOCK_SEQPACKET)
        case NH_NETWORK_SOCKET_RAW       : NH_NETWORK_END(SOCK_RAW)
        case NH_NETWORK_SOCKET_RDM       : NH_NETWORK_END(SOCK_RDM)
    }

#endif

NH_NETWORK_END(-1)
}

