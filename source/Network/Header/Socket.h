#ifndef NH_NET_SOCKET_H
#define NH_NET_SOCKET_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../API/Header/Netzhaut.h"
#include "../../Core/Header/List.h"

#include <stdint.h>
#include <stdlib.h>

#endif

/** @addtogroup NetworkEnums Enums
 *  \ingroup Network
 *  @{
 */
    typedef enum NH_NET_PROTOCOL_FAMILY {
        NH_NET_PROTOCOL_FAMILY_IPV4,
        NH_NET_PROTOCOL_FAMILY_IPV6,
    } NH_NET_PROTOCOL_FAMILY;

    typedef enum NH_NET_SOCKET {
        NH_NET_SOCKET_TCP,
        NH_NET_SOCKET_UDP,
        NH_NET_SOCKET_SEQPACKET,
        NH_NET_SOCKET_RAW,        
        NH_NET_SOCKET_RDM,
    } NH_NET_SOCKET;

    typedef enum NH_NET_PORT {
        NH_NET_PORT_HTTP = 80,
        NH_NET_PORT_HTTPS = 443,
        NH_NET_PORT_SMTP = 25,
        NH_NET_PORT_FTP_CONNECT = 21,
        NH_NET_PORT_FTP_TRANSFER = 20,
    } NH_NET_PORT;

/** @} */

/** @addtogroup NetworkDefinitions Definitions
 *  \ingroup Network
 *  @{
 */

    typedef struct Nh_Net_ClientSocket Nh_Net_ClientSocket;

/** @} */

/** @addtogroup NetworkStructs Structs
 *  \ingroup Network
 *  @{
 */

    typedef struct Nh_Net_Data {
        NH_NET_PORT port;
        void *data_p;
        size_t size;
    } Nh_Net_Data;

    typedef struct Nh_Net_Socket {
        char *hostName_p;
        NH_NET_SOCKET type;
        NH_NET_PROTOCOL_FAMILY family;
        char *protocolName_p;
        uint32_t address4;             /**<In network byte order.*/
        unsigned char address16_p[16]; /**<In network byte order.*/
        unsigned short port;           /**<In host byte order.*/
    } Nh_Net_Socket;

/** @} */

/** @addtogroup NetworkFunctions Functions
 *  \ingroup Network
 *  @{
 */

    Nh_Net_Data Nh_Net_initData(
    );

    NH_RESULT Nh_Net_getSockets(
        char *hostNameIn_p, char *hostNameOut_p, NH_NET_PORT port, Nh_List *List_p
    );

    Nh_Net_ClientSocket *Nh_Net_getClientSocket(
        Nh_Net_Socket Socket
    );

    NH_RESULT Nh_Net_closeClientSocket(
        Nh_Net_ClientSocket *Socket_p
    );

    NH_RESULT Nh_Net_freeSockets(
        Nh_List *Sockets_p
    );

    NH_RESULT Nh_Net_connect(
        Nh_Net_ClientSocket *ClientSocket_p, Nh_Net_Socket Socket
    );

    NH_RESULT Nh_Net_send(
        Nh_Net_ClientSocket *ClientSocket_p, char *message_p, size_t messageSize, NH_BOOL secure
    );

    ssize_t Nh_Net_receive(
        Nh_Net_ClientSocket *ClientSocket_p, char *set_p, ssize_t count, NH_BOOL secure
    );

    Nh_Net_Data Nh_Net_getDataFromSocket(
        Nh_Net_ClientSocket *Socket_p, char *hostName_p, char *pathName_p, NH_NET_PORT port
    );

    void Nh_Net_stringifySocketAddress(
        Nh_Net_Socket *Socket_p, char *set_p 
    );

    void Nh_Net_stringifySocket(
        Nh_Net_Socket *Socket_p, char *set_p
    );

    Nh_Net_ClientSocket *Nh_Net_createClientSocket(
        NH_NET_PROTOCOL_FAMILY family, NH_NET_SOCKET type
    );

/** @} */

#endif 
