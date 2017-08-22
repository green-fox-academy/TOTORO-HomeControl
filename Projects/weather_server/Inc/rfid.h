/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __rfid_H
#define __rfid_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Exported functions ------------------------------------------------------- */
void spi_init(void);
void transmit_spi(void);

#endif /* __rfid_H*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
