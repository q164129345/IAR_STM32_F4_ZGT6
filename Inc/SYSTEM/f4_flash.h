#ifndef __F4_FLASH_H__
#define __F4_FLASH_H__

#include "main.h"

//FLASH起始地址
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000)   //扇区0起始地址，16KB
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000)   //扇区1起始地址，16KB
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000)   //扇区2起始地址，16KB
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800c000)   //扇区3起始地址，16KB
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000)   //扇区4起始地址，64KB
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000)   //扇区5起始地址，128KB
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000)   //扇区6起始地址，128KB
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000)   //扇区7起始地址，128KB
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000)   //扇区8起始地址，128KB
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000)   //扇区1起始地址，128KB
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000)   //扇区1起始地址，128KB
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000)   //扇区1起始地址，128KB

//FLASH的配置
#define STM32_FALSH_BASE 0x08000000 //STM32F4 FLASH的起始地址
#define FLASH_WAITE_TIME 50000      //FLASH等待超时时间


//网络配置的大小
#define W5100_NETWORK_SETTING_SIZE 4+4+6+4


//静态函数（本地函数）
static uint8_t STMFLASH_GetFlashSector(uint32_t addr);
static uint32_t STMFLASH_ReadWord(uint32_t faddr);

//对外函数
void STMFLASH_Write(uint32_t WriteAddr, uint32_t *pBuffer, uint32_t NumToWrite); //从指定地址开始写入指定长度的数据
void STMFLASH_Read(uint32_t ReadAddr, uint32_t *pBuffer, uint32_t NumToRead);    //从指定地址开始读出指定长度的数据







#endif
