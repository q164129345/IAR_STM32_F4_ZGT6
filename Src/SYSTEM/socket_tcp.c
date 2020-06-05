#include "socket_tcp.h"
#include "w5100_reg.h"
#include "net_reg.h"

#define gSn_RX_MASK 0x7ff
#define gSn_TX_MASK gSn_RX_MASK
#define gS0_RX_BASE 0x6000
#define gS0_TX_BASE 0x4000

void tcp_Socket(uint8_t socket, uint16_t sport)
{
    uint16_t s = 0x100 * socket; /* register offset from socket 0 register */

    do{
        do{
            W5100_Write_A_Byte(s + REG_S0_MR, MR_TCP); //set socket n to TCP protocol
            W5100_Write_Two_Bytes(s + REG_S0_PORT0, sport); //set the source port of socket n
            W5100_Write_A_Byte(s + REG_S0_CR, CR_OPEN); //Socket n initialization
            if(W5100_Read_A_Byte(s + REG_S0_SR) != SOCK_INIT) //check the SR register, is equal to SOCK_INIT?
            {
                W5100_Write_A_Byte(s + REG_S0_CR, CR_CLOSE);  //close the sokcet n
            }
        }while(W5100_Read_A_Byte(s + REG_S0_SR) != SOCK_INIT); //if the SR register is not equal to SOCK_INIT, these code need to run again

        W5100_Write_A_Byte(s + REG_S0_CR, CR_LISTEN); //Set socket n to LISTEN status

        if(W5100_Read_A_Byte(s + REG_S0_SR) != SOCK_LISTEN) //check the SR register, is equal to SOCK_LISTEN?
        {
            W5100_Write_A_Byte(s + REG_S0_SR, CR_CLOSE);  //close the socket n
        }
    }while(W5100_Read_A_Byte(s + REG_S0_SR) != SOCK_LISTEN); //if Socket n isn't on LISTEN status, these code need to run again
    
}

uint8_t is_Tcp_Socket_Established(uint8_t socket)
{
    uint16_t s = 0x100 * socket; /* register offset from socket 0 registers */
    if(W5100_Read_A_Byte(s + REG_S0_SR) == SOCK_ESTABLISHED)
    {
        return 1;
    }
    return 0;
}

uint16_t tcp_Socket_Has_Data(uint8_t socket)
{
    uint16_t s = 0x100 * socket; /* register offset from socket 0 registers */
    return W5100_Read_Two_Bytes(s + REG_S0_RX_RSR0);
}

uint16_t get_Tcp_Packet(
        int       socket,   /*  0..3 */
        uint8_t * header,   /* 8 bytes or NULL: peer_ip[0..3], peer_port[4..5], get_size[6..7] */
        uint8_t * packet,   /* TCP payload */
        int       max_bytes /* size of packet[] buffer (this function discards excess data) */
        )
{
    uint16_t s    = 0x100 * socket; /* register offset from socket 0 registers */
    uint16_t strx = 0x800 * socket; /* register offset from socket 0 TX or RX register */

    uint16_t bytes_copied = 0;
    uint8_t lheader[8];  /* we need it anyway */
    if(!header)
    {
        header = lheader;
    }

    /* first, get the received size */
    int get_size = W5100_Read_Two_Bytes(s + REG_S0_RX_RSR0);
    /* calculate start address*/
    uint16_t get_offset = W5100_Read_Two_Bytes(s + REG_S0_RX_RD0) & gSn_RX_MASK;
    /* calculate start address(physical address) */
    uint16_t get_start_address = strx + gS0_RX_BASE + get_offset;

    /* if overflow socket RX memory */
    if((get_offset + get_size) > (gSn_RX_MASK + 1))
    {
        /* copy upper_size bytes of get_start_address to header */
        int upper_size = (gSn_RX_MASK + 1) - get_offset;
        W5100_Memcpy(get_start_address, packet, upper_size);
        /* update header */
        packet += upper_size;
        bytes_copied += upper_size;
        
        /* copy left_size bytes of gS0_RX_BASE to header */
        int left_size = get_size - upper_size;
        W5100_Memcpy(strx + gS0_RX_BASE, packet, left_size);
        /* update get_offset */
        bytes_copied += left_size;
        get_offset = left_size;
    }
    else
    {
        /*copy get_size bytes of get_start_address to header*/
        W5100_Memcpy(get_start_address, packet, get_size);
        bytes_copied += get_size;    
    }
    /* increase Sn_RX_RD as length of get_size */
    W5100_Write_Two_Bytes(s + REG_S0_RX_RD0, W5100_Read_Two_Bytes(s + REG_S0_RX_RD0) + get_size);
    /* set RECV command */
    W5100_Write_A_Byte(s + REG_S0_CR,CR_RECV);
    while(W5100_Read_A_Byte(s + REG_S0_CR));
    return bytes_copied;
}


void tcp_Write(uint8_t socket, uint8_t * payload, uint16_t send_size)
{
    uint16_t s    = 0x100 * socket; /* register offset from socket 0 registers */
    uint16_t strx = 0x800 * socket; /* register offset from socket 0 TX or RX register */

    uint16_t get_free_size = 0;
    while(!get_free_size)
    {
        get_free_size = W5100_Read_Two_Bytes(s + REG_S0_TX_FSR0);
    }

    uint16_t get_offset = W5100_Read_Two_Bytes(s + REG_S0_TX_WR0) & gSn_TX_MASK;
    uint16_t get_start_address = strx + gS0_TX_BASE + get_offset;

    if((get_offset + send_size) > (gSn_TX_MASK +1 ))
    {
        uint16_t upper_size = (gSn_TX_MASK + 1) - get_offset;
        W5100_Memcpy_To_Chip(payload, get_start_address, upper_size);

        payload += upper_size;
        uint16_t left_size = send_size - upper_size;
        W5100_Memcpy_To_Chip(payload, strx + gS0_TX_BASE, left_size);
    }
    else
    {
        W5100_Memcpy_To_Chip(payload, get_start_address, send_size);
    }

    W5100_Write_Two_Bytes(s + REG_S0_TX_WR0, W5100_Read_Two_Bytes(s + REG_S0_TX_WR0) + send_size);

    W5100_Write_A_Byte(s + REG_S0_CR, CR_SEND);     
    
}

void tcp_Write_u8(uint8_t socket, uint8_t val)
{
    tcp_Write(socket, &val, 1);
}





























