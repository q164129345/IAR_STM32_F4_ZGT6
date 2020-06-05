#ifndef _NET_REG_H_
#define _NET_REG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "main.h"

#define SPI_WRITE 0xF0
#define SPI_READ  0x0F

extern uint8_t * p_w5100_Default_Network_Settings;

void     net_wr_reg(uint16_t address, uint8_t value);
uint8_t  net_rd_reg(uint16_t address);
void     net_wr_reg16(uint16_t address, uint16_t value);
uint16_t net_rd_reg16(uint16_t address);

uint16_t net_rx_size(int socket);

void w5100_memcpy(uint16_t saddr, uint8_t * dst, int size);
void w5100_memcpy_to_chip(uint8_t * src, uint16_t daddr, int size);

void reset_switch(void); /* schwenkscanner only! */
void reset_w5100(void);
void set_ip_address_w5100(void);
void init_w5100(void);
void enable_w5100_interrupt(void);

#ifdef __cplusplus
}
#endif

#endif /*_NET_REG_H_ */

