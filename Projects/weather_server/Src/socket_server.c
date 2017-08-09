/* Includes ------------------------------------------------------------------*/
#include "socket_server.h"
#include "lcd_log.h"
#include "cmsis_os.h"
#include "app_ethernet.h"
#include "lwip/sockets.h"
#include "stm32746g_discovery_ts.h"
#include <string.h>
#include "stm32746g_discovery_lcd.h"
#include "GUI.h"
#include "DIALOG.h"
float received_weather_data[3];


#define SERVER_QUEUE_SIZE 100
#define SERVER_BUFF_LEN 100
#define PORT 9500

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void terminate_thread()
{
	while (1)
		osThreadTerminate(NULL);
}
// TODO:
// Implement this function!


void socket_server_thread(void const *argument)
{
		//BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

		LCD_UsrLog("Socket server - startup...\n");
		LCD_UsrLog("Socket server - waiting for IP address...\n");
//		GUI_SetColor(GUI_WHITE);
//		GUI_DispString("Socket server startup\n");
//		GUI_DispString("Socket server waiting for IP address...\n");


		// Wait for an IP address
		while (!is_ip_ok())
			osDelay(10);
		LCD_UsrLog("Socket server - IP address is ok\n");
//		GUI_DispString("Socket server - IP address is OK\n");

		// Create server socket
		int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
		if (server_socket < 0) {
			LCD_ErrLog("Socket server - can't create socket\n");
//			GUI_DispString("Socket server - cannot create socket\n");
			terminate_thread();
		}
		LCD_UsrLog("Socket server - socket created\n");
//		GUI_DispString("Socket server - socket created\n");

		// Create address structure and bind the socket to it
		struct sockaddr_in server_addr;
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(PORT);
		server_addr.sin_addr.s_addr = INADDR_ANY;
		if (bind(server_socket, (struct sockaddr*)&(server_addr), sizeof(server_addr)) < 0) {
			LCD_ErrLog("Socket server - can't bind socket\n");
//			GUI_DispString("Socket server - cannot bind socket\n");
			terminate_thread();
		}
		LCD_UsrLog("Socket server - socket bind ok\n");
//		GUI_DispString("Socket server - socket bind OK\n");

		// Start listening
		if (listen(server_socket, SERVER_QUEUE_SIZE) < 0) {
			LCD_ErrLog("Socket server - can't listen\n");
//			GUI_DispString("Socket server - cannot listen\n");
			terminate_thread();
		}
		LCD_UsrLog("Socket server - listening...\n");
//		GUI_DispString("Socket server - listening...\n");


		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		int client_socket;
		//TS_StateTypeDef TS_State;
		//TS_StateTypeDef *TS_State_ptr = &TS_State;
		while (1) {
			// Accept incoming connections
			client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
			LCD_UsrLog("Socket server - connection accepted\n");
//			GUI_DispString("Socket server connection accepted\n");
			BSP_LCD_Clear(LCD_COLOR_GREEN);
			// Check the client socket
			if (client_socket < 0) {
				LCD_ErrLog("Socket server - invalid client socket\n");
//				GUI_DispString("Socket server - invalid client socket\n");
			} else {
				// Define buffer for incoming message
				//char buff[SERVER_BUFF_LEN];
				float received_bytes;
				//float buffer[3];
				// Receive data
				do {
					
					received_bytes = recv(client_socket, received_weather_data, sizeof(received_weather_data), 0);
					LCD_UsrLog("Temperature: %.1f C, Humidity: %.1f%%, Pressure: %.1f Pa,\n", received_weather_data[0], received_weather_data[1], received_weather_data[2]);
//					GUI_DispStringAt("Temperature: ", 150, 80);
					GUI_GotoXY(50, 150);
					GUI_DispFloat(received_weather_data[0], 3);
//					GUI_DispString(" �C");

//					GUI_DispStringAt("Humidity: ", 150, 120);
					GUI_GotoXY(213, 150);
					GUI_DispFloat(received_weather_data[1], 4);
//					GUI_DispString(" %");

//					GUI_DispStringAt("Air pressure: ", 150, 160);
					GUI_GotoXY(376, 150);
					GUI_DispFloat(received_weather_data[2], 4);
//					GUI_DispString(" Pa");

				} while (received_bytes > 0);

				// Close the socket
				closesocket(client_socket);
				LCD_UsrLog("Socket server - connection closed\n");
//				GUI_DispString("Socket server - connection closed\n");
			}
		}

		// Close socket
		closesocket(server_socket);

		while (1) {
			osDelay(10);
		}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
