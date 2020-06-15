#ifndef _SHELL_H
#define _SHELL_H


#include "main.h"
#include "usart.h"
#include "net_reg.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "ctype.h"

//宏定义
#define ANALYSIS_SUCCESS 0x01U
#define ANALYSIS_ERROR   0x00U
//用于excute_Command与analyze_User_Command函数的return
#define SHELL_OK         0x01U
#define SHELL_NO_OK      0x00U


/* 用于存放字符串形式的网络配置 */
typedef struct format_network_settings
{
    //以下的数组都申请多1byte,用于保存'\0'
    //最后1byte存'\0',可以防止strcpy,strlen等C标准函数出问题.
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
    char part1_MAC_Address[3];
    char part2_MAC_Address[3];
    char part3_MAC_Address[3];
    char part4_MAC_Address[3];
    char part5_MAC_Address[3];
    char part6_MAC_Address[3];
    /* ip address */
    char part1_IP_Address[4];
    char part2_IP_Address[4];
    char part3_IP_Address[4];
    char part4_IP_Address[4];

}_STRING_FORMAT_NETWORK_SETTINGS;

//静态函数（本地函数）
static void show_W5100_Default_Network_Settings(void);
static int8_t excute_Command(char const *Command, _STRING_FORMAT_NETWORK_SETTINGS *network_Setting);
static int8_t Command_Turn_To_Value(char const *Command, _STRING_FORMAT_NETWORK_SETTINGS *string_Network_Setting ,uint8_t *value_Network_Setting);

//对外函数
int HexStr2Integer( char * HexStr );
void analyze_User_Command(char const *Command);






#endif

