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
#include "WindowDLG.h"
#include "ff.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SERVER_QUEUE_SIZE 100
#define SERVER_BUFF_LEN 100
#define PORT 8002
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
float received_weather_data[3];
FIL w_log;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void terminate_thread()
{
	while (1)
		osThreadTerminate(NULL);
}

void socket_server_thread(void const *argument)
{
	LCD_UsrLog("Socket server - startup...\n");
	LCD_UsrLog("Socket server - waiting for IP address...\n");

	// Wait for an IP address
	while (!is_ip_ok())
		osDelay(10);
	LCD_UsrLog("Socket server - IP address is ok\n");
	//GUI_DispString("Socket server - IP address is OK\n");

	// Create server socket
	int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (server_socket < 0) {
		LCD_ErrLog("Socket server - can't create socket\n");
	//GUI_DispString("Socket server - cannot create socket\n");
		terminate_thread();
	}
	LCD_UsrLog("Socket server - socket created\n");
	//GUI_DispString("Socket server - socket created\n");

	// Create address structure and bind the socket to it
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(server_socket, (struct sockaddr*)&(server_addr), sizeof(server_addr)) < 0) {
		LCD_ErrLog("Socket server - can't bind socket\n");
	//GUI_DispString("Socket server - cannot bind socket\n");
		terminate_thread();
	}
	LCD_UsrLog("Socket server - socket bind ok\n");
	//GUI_DispString("Socket server - socket bind OK\n");

	// Start listening
	if (listen(server_socket, SERVER_QUEUE_SIZE) < 0) {
		LCD_ErrLog("Socket server - can't listen\n");
	//GUI_DispString("Socket server - cannot listen\n");
		terminate_thread();
	}
	LCD_UsrLog("Socket server - listening...\n");
	//GUI_DispString("Socket server - listening...\n");

	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	int client_socket;
	char temp[128];

	while (1) {
		// Accept incoming connections
		client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
		LCD_UsrLog("Socket server - connection accepted\n");
		//GUI_DispString("Socket server connection accepted\n");
		BSP_LCD_Clear(LCD_COLOR_GREEN);
		// Check the client socket
		if (client_socket < 0) {
			LCD_ErrLog("Socket server - invalid client socket\n");
			//GUI_DispString("Socket server - invalid client socket\n");
		} else {
			// Receive data
			f_open(&w_log, "W.CSV", FA_OPEN_ALWAYS | FA_WRITE);
			float received_bytes;
			do {
				received_bytes = recv(client_socket, received_weather_data, sizeof(received_weather_data), 0);
				LCD_UsrLog("Temperature: %.1f C, Humidity: %.1f%%, Pressure: %.1f Pa,\n", received_weather_data[0], received_weather_data[1], received_weather_data[2]);
				memcpy(temp, &received_weather_data, sizeof(float));
				sprintf(temp, "%.2f,%.1f,%.2f\n", received_weather_data[0], received_weather_data[1], received_weather_data[2]);
				f_printf(&w_log, temp);

				gui_update_temp(received_weather_data[0]);
				gui_update_hum(received_weather_data[1]);
				gui_update_press(received_weather_data[2]);
			} while (received_bytes > 0);
			f_close(&w_log);
			// Close the socket
			closesocket(client_socket);
			LCD_UsrLog("Socket server - connection closed\n");
			//GUI_DispString("Socket server - connection closed\n");
		}//else
	}//while(1)

	// Close socket
	closesocket(server_socket);

	while (1) {
		osDelay(10);
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
