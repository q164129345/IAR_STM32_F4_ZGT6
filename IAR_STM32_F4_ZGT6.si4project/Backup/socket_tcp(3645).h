#ifndef _SOCKET_TCP_H_
#define _SOCKET_TCP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "main.h"

#define gSn_RX_MASK 0x7ff
#define gSn_TX_MASK gSn_RX_MASK
#define gS0_RX_BASE 0x6000
#define gS0_TX_BASE 0x4000


//Packet_Buffer,the maximum length
#define PACKET_BUFFER_LEN 0x800


typedef enum _socket_NumberTypeDef
{
    SOCKET_0 = 0 ,   /* only use socket0 and socket1 */
    SOCKET_1 = 1 
}socket_NumberTypeDef;


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
void analyze_Tcp_Message(socket_NumberTypeDef socket_Number , uint8_t *packet_Buffer);




#ifdef __cplusplus
}
#endif

#endif /* _SOCKET_TCP_H_ */

