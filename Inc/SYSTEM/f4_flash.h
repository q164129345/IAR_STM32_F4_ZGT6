#ifndef __F4_FLASH_H__
#define __F4_FLASH_H__

#include "main.h"

//FLASH��ʼ��ַ
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000)   //����0��ʼ��ַ��16KB
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000)   //����1��ʼ��ַ��16KB
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000)   //����2��ʼ��ַ��16KB
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800c000)   //����3��ʼ��ַ��16KB
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000)   //����4��ʼ��ַ��64KB
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000)   //����5��ʼ��ַ��128KB
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000)   //����6��ʼ��ַ��128KB
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000)   //����7��ʼ��ַ��128KB
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000)   //����8��ʼ��ַ��128KB
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000)   //����1��ʼ��ַ��128KB
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000)   //����1��ʼ��ַ��128KB
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000)   //����1��ʼ��ַ��128KB

//FLASH������
#define STM32_FALSH_BASE 0x08000000 //STM32F4 FLASH����ʼ��ַ
#define FLASH_WAITE_TIME 50000      //FLASH�ȴ���ʱʱ��


//�������õĴ�С
#define W5100_NETWORK_SETTING_SIZE 4+4+6+4


//��̬���������غ�����
static uint8_t STMFLASH_GetFlashSector(uint32_t addr);
static uint32_t STMFLASH_ReadWord(uint32_t faddr);

//���⺯��
void STMFLASH_Write(uint32_t WriteAddr, uint32_t *pBuffer, uint32_t NumToWrite); //��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(uint32_t ReadAddr, uint32_t *pBuffer, uint32_t NumToRead);    //��ָ����ַ��ʼ����ָ�����ȵ�����







#endif
