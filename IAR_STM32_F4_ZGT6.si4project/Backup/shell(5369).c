/*
 * 用于与串口助手交互
 *
 */


#include "shell.h"




/* 提示帮助信息 */
uint8_t titleOfShell[] = "\r\n************* Easy shell *************\r\n"
                           "Help Information: \r\n"
                           "-g    Get the detail about network configuration. \r\n"
                           "-i    Set the network configuration. \r\n";


/*
 * 通过DMA方式发送帮助信息到串口
 *
 */
void show_Shell_Title(void)
{
    //通过DMA发送
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)titleOfShell,sizeof(titleOfShell));
}










