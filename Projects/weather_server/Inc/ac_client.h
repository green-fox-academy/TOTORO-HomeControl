/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AC_CLIENT_H
#define __AC_CLIENT_H

/* Includes ------------------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
//int8_t send_command_to_ac(uint8_t comm);
int8_t send_command_to_ac(uint8_t *comm);
void ac_client_thread(void const *argument);
void ac_client_init();

#endif /* __AC_CLIENT_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
