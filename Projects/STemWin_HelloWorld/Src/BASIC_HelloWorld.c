/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2004  SEGGER Microcontroller Systeme GmbH        *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

***** emWin - Graphical user interface for embedded applications *****
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : BASIC_HelloWorld.c
Purpose     : Simple demo drawing "Hello world"
----------------------------------------------------------------------
*/

#include "GUI.h"

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/
void MainTask(void) {

	float temp = 32.0;
	float humid = 10.0;
	float press = 1000;

	GUI_Clear();
	GUI_SetFont(&GUI_Font20_1);
	GUI_DispStringAt("Temperature: ", 150, 50);
	GUI_DispFloat(temp, 4);
	GUI_DispStringAt("Humidity: ", 150, 100);
	GUI_DispFloat(humid, 4);
	GUI_DispStringAt("Air pressure: ", 150, 150);
	GUI_DispFloat(press, 4);




  while(1);
}

/*************************** End of file ****************************/
