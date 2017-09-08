/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STARTUP_SCREEN_H
#define __STARTUP_SCREEN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32746g_discovery_ts.h"
#include "socket_server.h"

/* Exported types ------------------------------------------------------------*/
typedef struct user_access {
	uint8_t ac_access;
	uint8_t proj_access;
}user_acc_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
//static void _cbDialog_start(WM_MESSAGE * pMsg);
WM_HWIN CreateWindow_start(void);
uint8_t check_user_access(uint8_t select, user_acc_t user_db[3]);


#endif /* __STARTUP_SCREEN_H*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
