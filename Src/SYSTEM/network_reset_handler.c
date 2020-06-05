#include "network_reset_handler.h"
#include "net_reg.h"
#include "w5100_reg.h"
#include "iwdg.h"
#include "string.h"
#include "socket_udp.h"

void network_Reset_Handler_Init(void)
{
    /* from sockets[0..3] we use socket[3] so
     * app can use max three socket[0..2]
     */
     do{
        W5100_Write_Two_Bytes(REG_S3_PORT0,NETWORK_RESET_HANDLER_PORT); //Set Socket3's source port, register 0x0704 set 0x0A,register 0x0705 set 0xD9
        W5100_Write_A_Byte(REG_S3_MR,MR_UDP);  //Set Socket3 UDP protocol
        W5100_Write_A_Byte(REG_S3_CR,CR_OPEN); //Socket3 initialization
        if(W5100_Read_A_Byte(REG_S3_SR)!= SOCK_UDP) //check Socket3 status register , is equal to SOCK_UDP??
        {
           W5100_Write_A_Byte(REG_S3_CR,CR_CLOSE);  //Not equal to SOCK_UDP, Close the Socket3
        }
     }while(W5100_Read_A_Byte(REG_S3_SR) != SOCK_UDP);  //Not equal to SOCK_UDP, need to run set again.
}

void network_Reset_Handler(void)
{
    HAL_IWDG_Refresh(&hiwdg);  //feed watchdog
    if(W5100_Read_Two_Bytes(REG_S3_RX_RSR0)) 
    {
        /* see whether we got NETWORK_RESET_MAGIC */
        uint8_t udp_payload[4];
        if(4 == get_udp_packet(3,NULL,udp_payload,4))
        {
            uint32_t magic =
                ((uint32_t)udp_payload[3] << 24) |
                ((uint32_t)udp_payload[2] << 16) |
                ((uint32_t)udp_payload[1] << 8)  |
                ((uint32_t)udp_payload[0] << 0);
            if(magic == NETWORK_RESET_MAGIC)
            {
               /* PE6 set high level */
               HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
               /* Just wait for the watchdog timeout */
               while(1);
            }                
        }    
    }
}













