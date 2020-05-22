#include "f4_flash.h"
#include "delay.h"



//const uint8_t TEXT_BUFFER[] = "STM32 FALSH TEST";


/*
//读取指定地址的字(32位数据)
//faddr:读地址
//返回值
uint32_t STMFLASH_ReadWord(uint32_t faddr)
{
    return *(__IO uint32_t *)faddr;
}
*/
//获取某个地址所在的flash扇区
//addr:flash地址
//返回值:0 ~ 11,即addr所在的扇区
uint8_t STMFLASH_GetFlashSector(uint32_t addr)
{
    
    if(addr<ADDR_FLASH_SECTOR_1) 
    {
        return FLASH_SECTOR_0;
    }
    if(addr<ADDR_FLASH_SECTOR_2) 
    {
        return FLASH_SECTOR_1;
    }
    else if(addr<ADDR_FLASH_SECTOR_3) 
    {
        return FLASH_SECTOR_2;
    }
    else if(addr<ADDR_FLASH_SECTOR_4)
    {
        return FLASH_SECTOR_3;
    }
    else if(addr<ADDR_FLASH_SECTOR_5)
    {
        return FLASH_SECTOR_4;
    }
    else if(addr<ADDR_FLASH_SECTOR_6) 
    {
        return FLASH_SECTOR_5;
    }
    else if(addr<ADDR_FLASH_SECTOR_7) 
    {
        return FLASH_SECTOR_6;
    }
    else if(addr<ADDR_FLASH_SECTOR_8)
    {
        return FLASH_SECTOR_7;
    }
    else if(addr<ADDR_FLASH_SECTOR_9)
    {
        return FLASH_SECTOR_8;
    }
    else if(addr<ADDR_FLASH_SECTOR_10)
    {
        return FLASH_SECTOR_9;
    }
    else if(addr<ADDR_FLASH_SECTOR_11)
    {
        return FLASH_SECTOR_10;
    }
    else
    {
        return FLASH_SECTOR_11;
    }
    
}


void STMFLASH_Write(uint32_t WriteAddr, uint32_t *pBuffer, uint32_t NumToWrite)
{
    FLASH_EraseInitTypeDef FlashEraseInit;
    HAL_StatusTypeDef FlashStatus = HAL_OK;
    uint32_t SectorError = 0;
    uint32_t addrx = 0;
    uint32_t endaddr = 0;
    if(WriteAddr < STM32_FALSH_BASE || WriteAddr%4) return;  //非法地址

    HAL_FLASH_Unlock(); //解锁
    addrx = WriteAddr;  //写入的起始地址
    endaddr = WriteAddr + NumToWrite*4 ; //写入的结束地址

    if(addrx < 0x1FFF0000)
    {
        while(addrx < endaddr)   //扫清一切障碍。（对非FFFFFFFF的地方，先擦除)
        {
            if(STMFLASH_ReadWord(addrx) != 0xFFFFFFFF) //有非0XFFFFFFFF的地方，要擦除这个扇区
            {
                FlashEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;         //擦除类型，扇区擦除
                FlashEraseInit.Sector    = STMFLASH_GetFlashSector(addrx);  //获取要擦除的扇区
                FlashEraseInit.NbSectors = 1;                               //一次擦一个扇区
                FlashEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;        //电压范围，VCC = 2.7~3.6V之间!！
                if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError) != HAL_OK)
                {
                    break; //发生错误
                }
                
            }else addrx+=4;
            FLASH_WaitForLastOperation(FLASH_WAITE_TIME); //等待上次操作完成
            
        }
    }

    //等待上次操作完成
    FlashStatus = FLASH_WaitForLastOperation(FLASH_WAITE_TIME);
    if(FlashStatus == HAL_OK)
    {
        while(WriteAddr < endaddr) //写数据
        {
            if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,WriteAddr,*pBuffer) != HAL_OK) //写入数据   
            {
                break; //写入异常
            }
            WriteAddr +=4;
            pBuffer ++;
        }    
    }

    HAL_FLASH_Lock();
}



void STMFLASH_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead)
{
    uint32_t i;
    for(i=0;i<NumToRead;i++)
    {
        pBuffer[i] = STMFLASH_ReadWord(ReadAddr); //读取4个字节
        ReadAddr += 4;     //偏移4个字节
    }
    
}


