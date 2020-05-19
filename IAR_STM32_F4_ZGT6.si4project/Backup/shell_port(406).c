#include "shell.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "sheel_port.h"

Shell shell;

char shell_buffer[512];


void User_Shell_Write(const char ch)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)& ch, 1, 0xFFFF);
}

void User_Shell_Init(void)
{
    shell.write = User_Shell_Write;

    shellInit(&shell,shell_buffer,512);
}
