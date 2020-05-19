#ifndef _SHELL_H
#define _SHELL_H


#include "main.h"
#include "usart.h"
#include "eeprom.h"
#include "net_reg.h"
#include "stdlib.h"


typedef struct format_network_settings
{
    /* ip address */
    uint8_t ip_Address_1[3];
    uint8_t ip_Address_2[3];
    uint8_t ip_Address_3[3];
    uint8_t ip_Address_4[3];
    /* MAC address */
    uint8_t MAC_Address_1[3];
    uint8_t MAC_Address_2[3];
    uint8_t MAC_Address_3[3];
    uint8_t MAC_Address_4[3];
    uint8_t MAC_Address_5[3];
    uint8_t MAC_Address_6[3];
    /* subnet mask */
    uint8_t subnet_Mask_1[3];
    uint8_t subnet_Mask_2[3];
    uint8_t subnet_Mask_3[3];
    uint8_t subnet_Mask_4[3];
    /* gate way */
    uint8_t gate_Way_1[3];
    uint8_t gate_Way_2[3];
    uint8_t gate_Way_3[3];
    uint8_t gate_Way_4[3];

}_FORMAT_NETWORK_SETTINGS;


int8_t* int_To_String(int32_t num, int8_t *str);
void show_Shell_Title(void);
void show_W5100_Default_Network_Settings(void);

#endif

