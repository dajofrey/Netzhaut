#ifndef NH_NETWORK_SOCKET_H
#define NH_NETWORK_SOCKET_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Common/Includes.h"

#include "../nh-core/Util/LinkedList.h"

#include <stdint.h>
#include <stdlib.h>

#endif

/** @addtogroup NetworkEnums Enums
 *  \ingroup Network
 *  @{
 */
    typedef enum NH_NETWORK_PROTOCOL_FAMILY {
        NH_NETWORK_PROTOCOL_FAMILY_IPV4,
        NH_NETWORK_PROTOCOL_FAMILY_IPV6,
    } NH_NETWORK_PROTOCOL_FAMILY;

    typedef enum NH_NETWORK_SOCKET {
        NH_NETWORK_SOCKET_TCP,
        NH_NETWORK_SOCKET_UDP,
        NH_NETWORK_SOCKET_SEQPACKET,
        NH_NETWORK_SOCKET_RAW,        
        NH_NETWORK_SOCKET_RDM,
    } NH_NETWORK_SOCKET;

    typedef enum NH_NETWORK_PORT {
        NH_NETWORK_PORT_HTTP = 80,
        NH_NETWORK_PORT_HTTPS = 443,
        NH_NETWORK_PORT_SMTP = 25,
        NH_NETWORK_PORT_FTP_CONNECT = 21,
        NH_NETWORK_PORT_FTP_TRANSFER = 20,
    } NH_NETWORK_PORT;

/** @} */

/** @addtogroup NetworkDefinitions Definitions
 *  \ingroup Network
 *  @{
 */

    typedef struct nh_network_ClientSocket nh_network_ClientSocket;

/** @} */

/** @addtogroup NetworkStructs Structs
 *  \ingroup Network
 *  @{
 */

    typedef struct nh_network_Data {
        NH_NETWORK_PORT port;
        void *data_p;
        size_t size;
    } nh_network_Data;

    typedef struct nh_network_Socket {
        char *hostName_p;
        NH_NETWORK_SOCKET type;
        NH_NETWORK_PROTOCOL_FAMILY family;
        char *protocolName_p;
        uint32_t address4;             /**<In network byte order.*/
        unsigned char address16_p[16]; /**<In network byte order.*/
        unsigned short port;           /**<In host byte order.*/
    } nh_network_Socket;

/** @} */

/** @addtogroup NetworkFunctions Functions
 *  \ingroup Network
 *  @{
 */

    nh_network_Data nh_network_initData(
    );

    NH_API_RESULT nh_network_getSockets(
        char *hostNameIn_p, char *hostNameOut_p, NH_NETWORK_PORT port, nh_LinkedList *List_p
    );

    nh_network_ClientSocket *nh_network_getClientSocket(
        nh_network_Socket Socket
    );

    NH_API_RESULT nh_network_closeClientSocket(
        nh_network_ClientSocket *Socket_p
    );

    NH_API_RESULT nh_network_freeSockets(
        nh_LinkedList *Sockets_p
    );

    NH_API_RESULT nh_network_connect(
        char *hostName_p, nh_network_ClientSocket *ClientSocket_p, nh_network_Socket Socket
    );

    NH_API_RESULT nh_network_send(
        nh_network_ClientSocket *ClientSocket_p, char *message_p, size_t messageSize, bool secure
    );

    ssize_t nh_network_receive(
        nh_network_ClientSocket *ClientSocket_p, char *set_p, ssize_t count, bool secure
    );

    nh_network_Data nh_network_getDataFromSocket(
        nh_network_ClientSocket *Socket_p, char *hostName_p, char *pathName_p, NH_NETWORK_PORT port
    );

    void nh_network_stringifySocketAddress(
        nh_network_Socket *Socket_p, char *set_p 
    );

    void nh_network_stringifySocket(
        nh_network_Socket *Socket_p, char *set_p
    );

    nh_network_ClientSocket *nh_network_createClientSocket(
        NH_NETWORK_PROTOCOL_FAMILY family, NH_NETWORK_SOCKET type
    );

/** @} */

#endif 
