#include "socket_udp.h"
#include "w5100_reg.h"
#include "net_reg.h"

#define gSn_RX_MASK 0x7ff
#define gS0_RX_BASE 0x6000

uint16_t get_Udp_Packet(
        int       socket,   /*  0..3 */
        uint8_t * header,   /* 8 bytes or NULL: peer_ip[0..3], peer_port[4..5], get_size[6..7] */
        uint8_t * packet,   /* UDP payload */
        int       max_bytes /* size of packet[] buffer (this function discards excess data) */
        )
{
    uint16_t s = 0x100 * socket; /* register offset from socket 0 register */

    uint16_t srx = 0x800 * socket; /* register offset from socket 0 Rx register */

    uint16_t bytes_copied = 0;
    uint8_t lheader[8]; /* we need it angway */

    if(!header)
    {
        header = lheader;
    }

    /* first, get the received size */
    int get_size = W5100_Read_Two_Bytes(s + REG_S0_RX_RSR0);

    if(!get_size) return 0;

    /* calculate offset address */
    uint16_t get_offset = W5100_Read_Two_Bytes(s + REG_S0_RX_RD0) & gSn_RX_MASK;

    /* calculate start address(physica address) */
    uint16_t get_start_address = srx + gS0_RX_BASE + get_offset;

    /* read head information(8 bytes) */
    int header_size = 8;

    /* if overflow socket RX memory */
    if((get_offset + header_size) > (gSn_RX_MASK + 1))
    {
        /* copy upper_size bytes of get_start_address to header */
        int upper_size = (gSn_RX_MASK + 1) - get_offset;
        W5100_Memcpy(get_start_address,header,upper_size);
        /* update header */
        header += upper_size;
        /* copy left_size bytes of gS0_RX_BASE to header */
        int left_size = header_size - upper_size;
        W5100_Memcpy(srx + gS0_RX_BASE,header,left_size);
        /* update get_offset */
        get_offset = left_size;
    }
    else
    {
        /*copy header_size bytes of get_start_address to header */
        W5100_Memcpy(get_start_address, header, header_size);
        /* update get_offset */
        get_offset += header_size;
    }
    /* update get_start_address */
    get_start_address = srx + gS0_RX_BASE + get_offset;


    /* save remote peer information & received data size */
    // peer_ip = header[0 to 3];
    // peer_port = header[4 to 5];
    // get_size  = (header[6] << 8 ) | header[7];   
    get_size -= 8;

    /* if overflow socket RX memory */
    if((get_offset + get_size) > (gSn_RX_MASK + 1))
    {
        /*copy upper_size bytes of get_start_address to packet */
        int upper_size = (gSn_RX_MASK + 1) - get_offset;

        if(max_bytes - bytes_copied >= upper_size)
        {
            W5100_Memcpy(get_start_address, packet, upper_size);
            bytes_copied += upper_size;
        }
        else
        {
            W5100_Memcpy(get_start_address, packet, max_bytes - bytes_copied);
            bytes_copied += max_bytes - bytes_copied;
        }

        /* update packet */
        packet += upper_size;
        /* copy left_size bytes of gS0_RX_BASE to packet */
        int left_size = get_size - upper_size;
        if(max_bytes - bytes_copied >= left_size)
        {
            W5100_Memcpy(srx + gS0_RX_BASE, packet, left_size);
            bytes_copied += left_size;
        }
        else
        {
            W5100_Memcpy(srx + gS0_RX_BASE, packet, max_bytes - bytes_copied);
            bytes_copied += max_bytes - bytes_copied;
        }
    }
    else
    {
        /* copy get_size bytes of get_start_address to packet */
        if( max_bytes - bytes_copied >= get_size)
        {
            W5100_Memcpy(get_start_address, packet, get_size);
            bytes_copied += get_size;
        }
        else
        {
            W5100_Memcpy(get_start_address, packet, max_bytes - bytes_copied);
            bytes_copied += max_bytes - bytes_copied;  
        }
    }
    
    /* increase Sn_RX_RD as length of get_size + header_size */
    W5100_Write_Two_Bytes(s + REG_S0_RX_RD0, W5100_Read_Two_Bytes(s + REG_S0_RX_RD0) + get_size + header_size);

    /* set RECV command */
    W5100_Write_A_Byte(s + REG_S0_CR, CR_RECV);
    while(W5100_Read_A_Byte(s + REG_S0_CR));
    return bytes_copied;
}



