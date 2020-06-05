#ifndef _SOCKET_TCP_H_
#define _SOCKET_TCP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "main.h"

void tcp_Socket(uint8_t socket, uint16_t sport);
uint8_t is_Tcp_Socket_Established(uint8_t socket);
uint16_t tcp_Socket_Has_Data(uint8_t socket);
void tcp_Write_u8(uint8_t socket, uint8_t val);
void tcp_Write(uint8_t socket, uint8_t * payload, uint16_t send_size);

uint16_t get_Tcp_Packet(
        int       socket,   /*  0..3 */
        uint8_t * header,   /* 8 bytes or NULL: peer_ip[0..3], peer_port[4..5], get_size[6..7] */
        uint8_t * packet,   /* TCP payload */
        int       max_bytes /* size of packet[] buffer (this function discards excess data) */
        );

#ifdef __cplusplus
}
#endif

#endif /* _SOCKET_TCP_H_ */

