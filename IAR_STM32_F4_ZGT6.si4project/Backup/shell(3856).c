/*
 * 用于与串口助手交互
 *
 */


#include "shell.h"

extern uint8_t w5100_Default_Network_Settings[];
int32_t number1 = 123456;
int8_t string[16] = {0};


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
    int32_t i = 0;
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
 * 通过DMA方式发送帮助信息到串口
 *
 */
void show_Shell_Title(void)
{
    //通过DMA发送
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)title_Of_Shell,sizeof(title_Of_Shell));
}

void show_W5100_Default_Network_Settings(void)
{
  //HAL_UART_Transmit_DMA(&huart1, (uint8_t *)w5100_Default_Network_Settings, W5100_NETWORK_SETTING_SIZE);
  printf("\r\n Hello world! \r\n");
  int_To_String(number1, string);
  printf("\r\n %s \r\n",string); 
}








