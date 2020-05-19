#include "net_reg.h"


uint8_t w5100_Default_Network_Settings[W5100_NETWORK_SETTING_SIZE] = 
{
    10,10,10,10,      //GWR Gateway IP Address Register
    // invalid, wo don't want GW in BL by default
    255,255,240,0,     //SUBR Subnet Mask Register
    0x70,0xB3,0xD5,0x46,0xFF,0x00,
    192,168,178,88,
};

uint8_t *p_w5100_Default_Network_Settings = w5100_Default_Network_Settings;






