#ifndef __NET_REG_H__
#define __NET_REG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "main.h"

#define SPI_WRITE 0xF0
#define SPI_READ  0x0F



extern uint8_t * g_w5100_Default_Network_Settings;


void W5100_Write_A_Byte(uint16_t regaddr, uint8_t data);
uint8_t W5100_Read_A_Byte(uint16_t regaddr);
void W5100_Write_Two_Bytes(uint16_t regaddr, uint16_t data);
uint16_t W5100_Read_Two_Bytes(uint16_t regaddr);
void W5100_Memcpy(uint16_t saddr, uint8_t *dst, int size);
void W5100_Memcpy_To_Chip(uint8_t *src, uint16_t daddr, int size);
void reset_W5100(void);
void set_Ip_Address_W5100(void);
void init_W5100(void);
void enable_W5100_Interrupt(void);



#ifdef __cplusplus
}
#endif

#endif /*_NET_REG_H_ */

