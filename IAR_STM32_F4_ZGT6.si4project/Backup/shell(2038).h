#ifndef _SHELL_H
#define _SHELL_H


#include "main.h"
#include "usart.h"
#include "eeprom.h"
#include "net_reg.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "ctype.h"



/* 用于存放字符串形式的网络配置          */
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
void analyze_User_Command(char const *Command);
int8_t analyze_IP_Subnet_Gateway_Command(char const *Command,char *part1,char *part2,char *part3,char *part4);
int8_t IP_Address_Command_Turn_To_Value(char const *part1,char const *part2,char const *part3,char const *part4,uint8_t *network_Setting);




#endif

