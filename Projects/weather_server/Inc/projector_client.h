/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PROJECTOR_CLIENT_H
#define __PROJECTOR_CLIENT_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

//void projector_server_thread(void const *argument);
int8_t send_command_to_projector_screen(uint8_t comm);
void projector_client_thread(void const *argument);


#endif /* __PROJECTOR_CLIENT_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
