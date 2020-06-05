#include "net_reg.h"
#include "f4_flash.h"
#include "delay.h"

/* Ä¬ÈÏµÄÍøÂçÅäÖÃ */
uint8_t w5100_Default_Network_Settings[W5100_NETWORK_SETTING_SIZE] = 
{
    10,10,10,10,        //GWR Gateway IP Address Register
    // invalid, wo don't want GW in BL by default
    255,255,240,0,      //SUBR Subnet Mask Register
    0x70,0xB3,0xD5,0x46,0xFF,0x00,  //MAC address
    192,168,178,88,     //Ip Address
};

/* 
 * ÍøÂçÅäÖÃ
 * eepromµÄÍøÂçÅäÖÃ»á´æÔÚÕâÀï
 *
 */
uint8_t w5100_Network_Settings[W5100_NETWORK_SETTING_SIZE] =
{
    0,0,0,0,   //Gateway IP
    0,0,0,0,   //Subnet Mask
    0,0,0,0,0,0,   //MAC address
    0,0,0,0,   //Ip Address
};

uint8_t *p_w5100_Default_Network_Settings = w5100_Default_Network_Settings;

/*
function: 
    Write a single byte inside a given W5100 memory register
    
note: 
*/
HAL_StatusTypeDef W5100_Write_A_Byte(uint16_t regaddr, uint8_t data)
{
    HAL_StatusTypeDef status = HAL_OK;

    //Every w5100 write command starts with 0xF0 byte,followed by the register address(2 bytes) and data(1 byte)
    uint8_t buf[] = {SPI_WRITE, regaddr >> 8, regaddr, data);

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);   //select w5100, Set PA3 to low level.
    status = HAL_SPI_Transmit(hspi1,buf,4,0xFFFFFFFF);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);     //deselect w5100, Set PA3 to high level.  

    return status;
}

/*
function: 
    Read a single byte inside a given W5100 memory register
        
note: 
*/
uint8_t W5100_Read_A_Byte(uint16_t regaddr)
{
    HAL_StatusTypeDef status = HAL_OK;

    //Every W5100 read command starts with 0x0F byte, followed by the register address (2 bytes) and data (1 byte)
    uint8_t wbuf[] = {SPI_READ, regaddr >> 8, regaddr, data);
    uint8_t rbuf[4];
    
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);   //select w5100, Set PA3 to low level.
    status = HAL_SPI_TransmitReceive(hspi1,wbuf,rbuf,4,0xFFFFFFFF);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);     //deselect w5100, Set PA3 to high level.     

    return rbuf[3];
}

/*
function: 
    Write 2 bytes inside a given W5100 memory register
            
note: 
*/
void W5100_Write_Two_Bytes(uint16_t regaddr, uint16_t data)
{
    W5100_Write_A_Byte(regaddr+0,data >>8);
    W5100_Write_A_Byte(regaddr+1,data & 0xFF);
}

/*
function: 
    Read 2 bytes inside a given W5100 memory register
            
note: 
*/
uint16_t W5100_Read_Two_Bytes(uint16_t regaddr)
{
    uint16_t data;
    data = W5100_Read_A_Byte(regaddr+0);
    data <<=8 ;
    data |= W5100_Read_A_Byte(regaddr+1);
    return data;
}


/*
function: 
    
            
note: 
*/
void W5100_Memcpy(uint16_t saddr, uint8_t *dst, int size)
{
    int i;

    for(i=0; i<size; i++)
    {
        dst[i] = W5100_Read_A_Byte(saddr+i);
    }
}

/*
function: 
    
            
note: 
*/
void W5100_Memcpy_To_Chip(uint8_t *src, uint16_t daddr, int size)
{
    int i;
    for(i=0 ; i<size; i++)
    {
        W5100_Write_A_Byte(daddr + i, src[i]);
    }

}

/*
function: 
     Make W5100 reset
            
note: 
*/
void reset_W5100(void)
{
    /* SW reset */
    delay_ms(3);
    //Address(0x0000)is Mode Register, set bit7 to 1 can initialize W5100 and bit7 will back to 0 automatically.
    W5100_Write_A_Byte(0,0x80);
    
    int retries = 1000;

    //Wait for initialization to complete, maximum 1000ms
    while((0x80 & W5100_Read_A_Byte(0)) && (retries--))
    {
        delay_ms(1);
    }

    delay_ms(250);
}













