#ifndef _SOCKET_UDP_H_
#define _SOCKET_UDP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

uint16_t get_Udp_Packet(
        int       socket,   /*  0..3 */
        uint8_t * header,   /* 8 bytes or NULL: peer_ip[0..3], peer_port[4..5], get_size[6..7] */
        uint8_t * packet,   /* UDP payload */
        int       max_bytes /* size of packet[] buffer (this function discards excess data) */
        );

#ifdef __cplusplus
}
#endif

#endif /* _SOCKET_UDP_H_ */

