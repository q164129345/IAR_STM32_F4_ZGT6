/*
 * 用于与串口助手交互
 *
 */


#include "shell.h"

extern uint8_t w5100_Default_Network_Settings[];
/* 定义一个结构体以“字符串”的形式存储网络设置 */
_FORMAT_NETWORK_SETTINGS buffer_For_Setting;
/* 定义一个结构体指针 */
_FORMAT_NETWORK_SETTINGS *p_buffer_For_Setting = &buffer_For_Setting;

int32_t number = 250;
int8_t string[3] = {0};

/* 提示帮助信息 */
uint8_t title_Of_Shell[] = "\r\n************* Easy shell *************\r\n"
                           "Help Information: \r\n"
                           "-g    Get the detail about network configuration. \r\n"
                           "-i    Set the network configuration. \r\n";








/*
function: 
    整数转字符串

input: 
    入口参数1    32位有符号整数
    入口参数2    8位有符号字符串的首地址
    
output:
    入口参数2的8位有符号字符串的首地址
    
note: 参考网址：https://www.runoob.com/w3cnote/c-int2str.html
*/
int8_t* int_To_String(int32_t num, int8_t *str)
{
    int8_t i = 0;
    if(num < 0)
    {
        num = - num;
        str[i++] = '-';
    }

    //转换
    do
    {
        str[i++] = num%10+48;
        num /= 10;
            
    }
    while (num); //num不为0继续循环

    str[i] = '\0';


    //确定开始调整的位置
    int j = 0;
    if(str[0] == '-')  //如果有负号，负号不用调整
    {
        j = 1;
        ++i;
    }

    //对称交换
    for(;j<i/2;j++)
    {
        //对称交换两端的值
        str[j] =  str[j] + str[i-1-j];
        str[i-1-j] = str[j] - str[i-1-j];
        str[j] = str[j] - str[i-1-j];
    }

    return str;

}



/*
function: 
    发送提示帮助信息到串口

input: 
    
output:
    
note: 
*/

void show_Shell_Title(void)
{
    //通过DMA发送
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)title_Of_Shell,sizeof(title_Of_Shell));
}



/*
function: 
    发送网络信息到串口

input: 
    
output:
    
note: 
*/
void show_W5100_Default_Network_Settings(void)
{
  HAL_UART_Transmit_DMA(&huart1, (uint8_t *)w5100_Default_Network_Settings, W5100_NETWORK_SETTING_SIZE);
}

void do_something(void)
{
    /* 转换Gateway IP */
    int_To_String(w5100_Default_Network_Settings[0],p_buffer_For_Setting->gate_Way_1);
    int_To_String(w5100_Default_Network_Settings[1],p_buffer_For_Setting->gate_Way_2);
    int_To_String(w5100_Default_Network_Settings[2],p_buffer_For_Setting->gate_Way_3);
    int_To_String(w5100_Default_Network_Settings[3],p_buffer_For_Setting->gate_Way_4);

    /* 转换Subnet Mask */
    int_To_String(w5100_Default_Network_Settings[4],p_buffer_For_Setting->subnet_Mask_1);
    int_To_String(w5100_Default_Network_Settings[5],p_buffer_For_Setting->subnet_Mask_2);
    int_To_String(w5100_Default_Network_Settings[6],p_buffer_For_Setting->subnet_Mask_3);
    int_To_String(w5100_Default_Network_Settings[7],p_buffer_For_Setting->subnet_Mask_4);

    /* 转换MAC Address */
    int_To_String(w5100_Default_Network_Settings[8],p_buffer_For_Setting->MAC_Address_1);
    int_To_String(w5100_Default_Network_Settings[9],p_buffer_For_Setting->MAC_Address_2);
    int_To_String(w5100_Default_Network_Settings[10],p_buffer_For_Setting->MAC_Address_3);
    int_To_String(w5100_Default_Network_Settings[11],p_buffer_For_Setting->MAC_Address_4);
    int_To_String(w5100_Default_Network_Settings[12],p_buffer_For_Setting->MAC_Address_5);
    int_To_String(w5100_Default_Network_Settings[13],p_buffer_For_Setting->MAC_Address_6);

    /* 转换IP Address */
    int_To_String(w5100_Default_Network_Settings[14],p_buffer_For_Setting->ip_Address_1);
    int_To_String(w5100_Default_Network_Settings[15],p_buffer_For_Setting->ip_Address_2);
    int_To_String(w5100_Default_Network_Settings[16],p_buffer_For_Setting->ip_Address_3);
    int_To_String(w5100_Default_Network_Settings[17],p_buffer_For_Setting->ip_Address_4);


    /* 打印出来 */
    printf("\r\n Gateway is : %s \r\n", p_buffer_For_Setting->gate_Way_1);
    printf("]r]n Subnet Mask is: %s \r\n",p_buffer_For_Setting->subnet_Mask_1);
    printf("\r\n Mack address is : %s \r\n",p_buffer_For_Setting->MAC_Address_1);
    printf("\r\n Ip address is : %s \r\n",p_buffer_For_Setting->ip_Address_1);

}






