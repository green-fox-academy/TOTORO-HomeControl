/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PROJECTOR_SERVER_H
#define __PROJECTOR_SERVER_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void projector_server_thread(void const *argument);
extern float received_weather_data[3];
#endif /* __PROJECTOR_SERVER_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
