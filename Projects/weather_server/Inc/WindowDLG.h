/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WINDOWDLG_H
#define __WINDOWDLG_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
static void _cbDialog(WM_MESSAGE * pMsg);
WM_HWIN CreateWindow(void);
void MainTask(void);

void gui_update_temp(float temp);
void gui_update_hum(float temp);
void gui_update_press(float temp);


extern uint8_t ctrl;
#endif /* __WINDOWDLG_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
