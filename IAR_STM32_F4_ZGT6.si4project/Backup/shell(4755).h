#ifndef _SHELL_H
#define _SHELL_H


#include "main.h"
#include "usart.h"
#include "eeprom.h"
#include "net_reg.h"
#include "stdlib.h"


typedef struct format_network_settings
{
    /* gate way */
    int8_t gate_Way_1[5];
    int8_t gate_Way_2[5];
    int8_t gate_Way_3[5];
    int8_t gate_Way_4[5];
    /* subnet mask */
    int8_t subnet_Mask_1[5];
    int8_t subnet_Mask_2[5];
    int8_t subnet_Mask_3[5];
    int8_t subnet_Mask_4[5];
    /* MAC address */
    int8_t MAC_Address_1[5];
    int8_t MAC_Address_2[5];
    int8_t MAC_Address_3[5];
    int8_t MAC_Address_4[5];
    int8_t MAC_Address_5[5];
    int8_t MAC_Address_6[5];
    /* ip address */
    int8_t ip_Address_1[5];
    int8_t ip_Address_2[5];
    int8_t ip_Address_3[5];
    int8_t ip_Address_4[5];
}_FORMAT_NETWORK_SETTINGS;


int8_t* int_To_String(int32_t num, int8_t *str);
void show_Shell_Title(void);
void show_W5100_Default_Network_Settings(void);
void do_something(void);



#endif

