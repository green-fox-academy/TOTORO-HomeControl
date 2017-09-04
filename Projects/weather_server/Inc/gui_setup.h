/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GUI_SETUP_H
#define __GUI_SETUP_H



/* Includes ------------------------------------------------------------------*/
#include "stm32746g_discovery_ts.h"

/* Sent value defines */
#define PROJECTOR_UP		1
#define PROJECTOR_STOP		2
#define PROJECTOR_DOWN		3
#define AC_STATE_CHANGE		1
#define AC_STATE_NOCHANGE	0
#define AC_SWING_ON			1
#define AC_SWING_OFF		0
#define AC_MIN_VALUE		16
#define AC_MAX_VALUE		30
#define AC_BLADE_0			0
#define AC_BLADE_1			1
#define AC_BLADE_2			2
#define AC_BLADE_3			3
#define AC_BLADE_4			4
#define AC_BLADE_5			5
#define AC_IS_OFF			0
#define AC_IS_ON			1


//extern uint8_t ac_state;
extern uint8_t ac_swing_state;
extern uint8_t ac_lever_state;
extern int ac_temperature;
extern uint8_t ac_controls[5];
extern uint8_t proj_control;

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */





#endif /* __GUI_SETUP_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
