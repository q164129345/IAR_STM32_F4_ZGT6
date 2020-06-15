#ifndef __NETWORK_RESET_HANDLER_H__
#define __NETWORK_RESET_HANDLER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define NETWORK_RESET_HANDLER_PORT 2777

#define NETWORK_RESET_MAGIC 0x696F6352 /* "iocR" in some weired byteorder */

/*******************************************************************
 * you can trigger a reset from remote like so:
 *   echo -n Rcoi | nc -u 192.168.178.202 2777
 * or
 *   use the Serva IOControl Flasher tool
 *******************************************************************/

void network_Reset_Handler_Init(void);
void network_Reset_Handler(void);


#ifdef __cplusplus
}
#endif

#endif /* _NETWORK_RESET_HANDLER_H_ */

