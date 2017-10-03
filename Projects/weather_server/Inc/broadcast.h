/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BROADCAST_H
#define __BROADCAST_H

/* Includes ------------------------------------------------------------------*/
#include "stm32746g_discovery_ts.h"
#include "socket_server.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#define BROADCAST_UNIQUE_STR_LEN    (strlen(BROADCAST_UNIQUE_STR)+1)
#define BROADCAST_MSG_LEN           1024
#define TCP_MSG_LEN                 1024
#define BROADCAST_LISTENING_PORT    12345    // Same for every device

void socket_broadcast_thread(void const *argument);

#endif /* __BROADCAST_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
