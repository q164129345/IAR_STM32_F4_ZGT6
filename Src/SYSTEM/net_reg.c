#include "net_reg.h"
#include "f4_flash.h"
/* 默认的网络配置 */
uint8_t w5100_Default_Network_Settings[W5100_NETWORK_SETTING_SIZE] = 
{
    10,10,10,10,        //GWR Gateway IP Address Register
    // invalid, wo don't want GW in BL by default
    255,255,240,0,      //SUBR Subnet Mask Register
    0x70,0xB3,0xD5,0x46,0xFF,0x00,  //MAC address
    192,168,178,88,     //Ip Address
};

/* 
 * 网络配置
 * eeprom的网络配置会存在这里
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






