/* Includes ------------------------------------------------------------------*/
#include "socket_server.h"
#include "lcd_log.h"
#include "cmsis_os.h"
#include "app_ethernet.h"
#include "lwip/sockets.h"
#include "stm32746g_discovery_ts.h"
#include <string.h>
#include "stm32746g_discovery_lcd.h"
#include "projector_server.h"
#define SERVER_IP           "10.27.6.141"
#define SERVER_PORT         8003

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void projector_client_thread(void const *argument)
{
	// Connect to server
		int client_sock;
		//struct sockaddr_in server;
		client_sock = socket(AF_INET, SOCK_STREAM, 0);
		if (client_sock < 0) {
			terminate_thread();
		}
		// Creating server address
		struct sockaddr_in addr_in;
		addr_in.sin_family = AF_INET;
		addr_in.sin_port = htons(SERVER_PORT);
		addr_in.sin_addr.s_addr = inet_addr(SERVER_IP);

		// Connecting the client socket to the server
//		int connect_retval = connect(client_sock, (struct sockaddr *)&addr_in, sizeof(addr_in));
//		if (connect_retval < 0) {
//			terminate_thread();
//		}

		uint8_t buffer1 = 1;
		TS_StateTypeDef TS_State;
		int send_bytes;
		int flag = -1;
		while (1) {
			if (flag != -1) {
//				do {
					if ((connect(client_sock, (struct sockaddr *)&addr_in, sizeof(addr_in))) < 0) {
						closesocket(client_sock);
						flag = -1;
					}
					BSP_TS_GetState(&TS_State);
					if (TS_State.touchDetected > 0) {
						if (TS_State.touchX[0] >= 400 && TS_State.touchX[0] <= 450
									&& TS_State.touchY[0] >= 147 && TS_State.touchY[0] <= 197) {
							buffer1 = 3;														//down
						} else if (TS_State.touchX[0] >= 400 && TS_State.touchX[0] <= 450
									&& TS_State.touchY[0] >= 87 && TS_State.touchY[0] <= 137) {
								buffer1 = 2; 													//stop
						} else if (TS_State.touchX[0] >= 400 && TS_State.touchX[0] <= 450
									&& TS_State.touchY[0] >= 27 && TS_State.touchY[0] <= 77) {
								buffer1 = 1;													//up
						}
						//determine int to send based on touch data
						send_bytes = send(client_sock, &buffer1, sizeof(uint8_t), 0);
						if (send_bytes < 0) {
							break;
						}
						osDelay(1000);
					}//if
//				}while (send_bytes > 0);
			} else {
				if ((connect(client_sock, (struct sockaddr *)&addr_in, sizeof(addr_in))) == 0) {
					flag = 0;
				}
				HAL_Delay(3000);
			}
		}//while(1)

		closesocket(client_sock);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
