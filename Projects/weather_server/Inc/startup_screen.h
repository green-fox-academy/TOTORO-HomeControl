/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STARTUP_SCREEN_H
#define __STARTUP_SCREEN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32746g_discovery_ts.h"
#include "socket_server.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
static void _cbDialog_start(WM_MESSAGE * pMsg);
WM_HWIN CreateWindow_start(void);

#endif /* __STARTUP_SCREEN_H*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
