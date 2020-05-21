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



/* 用于存放字符串形式的网络配置 */
typedef struct format_network_settings
{
    /* gate way */
    char part1_Gateway[4];
    char part2_Gateway[4];
    char part3_Gateway[4];
    char part4_Gateway[4];
    /* subnet mask */
    char part1_Subnet_Mask[4];
    char part2_Subnet_Mask[4];
    char part3_Subnet_Mask[4];
    char part4_Subnet_Mask[4];
    /* MAC address */
    char part1_MAC_Address[6];
    char part2_MAC_Address[6];
    char part3_MAC_Address[6];
    char part4_MAC_Address[6];
    char part5_MAC_Address[6];
    char part6_MAC_Address[6];
    /* ip address */
    char part1_IP_Address[4];
    char part2_IP_Address[4];
    char part3_IP_Address[4];
    char part4_IP_Address[4];

}_STRING_FORMAT_NETWORK_SETTINGS;


int8_t* int_To_String(int32_t num, int8_t *str);
void show_Shell_Title(void);
void show_W5100_Default_Network_Settings(void);
void do_something(void);
void analyze_User_Command(char const *Command);
//int8_t analyze_IP_Subnet_Gateway_Command(char const *Command,char *part1,char *part2,char *part3,char *part4);
int8_t analyze_IP_Subnet_Gateway_Command(char const *Command, _STRING_FORMAT_NETWORK_SETTINGS *network_Setting);
int8_t IP_Address_Command_Turn_To_Value(_STRING_FORMAT_NETWORK_SETTINGS *string_Network_Setting ,uint8_t *value_Network_Setting);
int8_t subnet_Mask_Command_Turn_To_Value(_STRING_FORMAT_NETWORK_SETTINGS *string_Network_Setting,uint8_t *value_Network_Setting);
int8_t gateway_IP_Command_Turn_To_Value(_STRING_FORMAT_NETWORK_SETTINGS *string_Network_Setting,uint8_t *value_Network_Setting);





#endif

