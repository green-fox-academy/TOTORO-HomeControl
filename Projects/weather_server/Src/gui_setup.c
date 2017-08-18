/* Includes ------------------------------------------------------------------*/
#include "socket_client.h"
#include "socket_server.h"
#include "lcd_log.h"
#include "cmsis_os.h"
#include "app_ethernet.h"
#include "lwip/sockets.h"
#include "stm32746g_discovery_ts.h"
#include <string.h>
#include "gui_setup.h"

 #include "GUI.h"
 #include "DIALOG.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//const GUI_POINT tri_up[] = {
//{ 417, 60},
//{ 433, 60},
//{ 425, 44}
//};
//
//const GUI_POINT tri_down[] = {
//{ 417, 164},
//{ 433, 164},
//{ 425, 180}
//};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void GUI_Startup()
{
//    GUI_Init();
//	WM_MULTIBUF_Enable(1);
//	GUI_SetLayerVisEx (1, 0);
//	GUI_SelectLayer(0);

	//split screen for weather station and projector canvas control
	GUI_SetBkColor(GUI_BLUE);
	GUI_Clear();
	GUI_SetColor(GUI_DARKBLUE);
	GUI_FillRect(0, 0, 100, 272);		//controls
	GUI_FillRect(105, 0, 275, 170);		//temperature
	GUI_FillRect(280, 0, 365, 85);		//humidity
	GUI_FillRect(280, 90, 365, 170);	//air pressure
	GUI_FillRect(105, 175, 365, 272);	//AC control
	GUI_FillRect(370, 0, 480, 217);		//projector canvas control
	GUI_FillRect(370, 222, 480, 272);	//empty
//	GUI_SetColor(GUI_LIGHTGRAY);
//	GUI_SetFont(GUI_FONT_16_1);
//	GUI_SetBkColor(GUI_DARKBLUE);
//	GUI_DispStringAt("HomeControl", 5, 5);
//	GUI_SetFont(GUI_FONT_13_1);
//	GUI_DispStringAt("Temperature (°C)", 110, 5);
//	GUI_DispStringAt("Humidity (%)", 285, 5);
//	GUI_DispStringAt("Pressure (Pa)", 285, 95);
//	GUI_DispStringAt("Projector", 375, 5);
//	GUI_SetColor(GUI_BLUE);
//	GUI_FillRect(400, 27, 450, 77);		//up
//	GUI_FillRect(400, 87, 450, 137);	//stop
//	GUI_FillRect(400, 147, 450, 197);	//down
//	GUI_SetColor(GUI_DARKBLUE);
//	GUI_FillRect(402, 29, 448, 75);		//up
//	GUI_FillRect(402, 89, 448, 135);	//stop
//	GUI_FillRect(402, 149, 448, 195);	//down

//	GUI_SetColor(GUI_BLUE);
//	GUI_FillPolygon(tri_up, 3, 0, 0);	//up sign
//	GUI_FillRect(417, 104, 433, 120);	//stop sign
//	GUI_FillPolygon(tri_down, 3, 0, 0);	//down sign
}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
