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
#include "projector_client.h"
#include "time.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define SERVER_QUEUE_SIZE 100
#define SERVER_BUFF_LEN 100
#define PORT 8002

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
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
	BSP_PB_Init(2, 0);
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
	char temp[100];
	char temptime[255];
	char header[] = "Time; Temperature; Humidity; Pressure\n";

	f_open(&w_log, "W.CSV", FA_OPEN_ALWAYS | FA_WRITE);
	f_printf(&w_log, header);

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

			float received_bytes;
			do {
				received_bytes = recv(client_socket, &received_data, sizeof(received_data), 0);	//added &
//				received_bytes = recv(client_socket, received_data.sensor_values, sizeof(received_data.sensor_values), 0);
				LCD_UsrLog("Temperature: %.1f C, Humidity: %.1f%%, Pressure: %.1f Pa,\n", received_data.sensor_values[0], received_data.sensor_values[1], received_data.sensor_values[2]);
//				memcpy(temp, received_data.sensor_values, sizeof(float));
				sprintf(temp, "%.2f;%.1f;%.2f\n", received_data.sensor_values[0], received_data.sensor_values[1], received_data.sensor_values[2]);
				strftime(temptime,100,"%c;", (struct tm*)&(received_data.hq_time));
//				sprintf(temptime, "%d.%d.%d. %d:%d:%d;", received_data.hq_time.tm_year, received_data.hq_time.tm_mon, received_data.hq_time.tm_mday, received_data.hq_time.tm_hour, received_data.hq_time.tm_min, received_data.hq_time.tm_sec);
				strcat(temptime, temp);

				f_printf(&w_log, temptime);
				f_sync(&w_log);

//				received_bytes = recv(client_socket, &received_data, sizeof(received_data), 0);	//added &

				gui_update_temp(received_data.sensor_values[0]);
				gui_update_hum(received_data.sensor_values[1]);
				gui_update_press(received_data.sensor_values[2]);
				gui_update_time(received_data.hq_time.tm_hour, received_data.hq_time.tm_min, received_data.hq_time.tm_sec);



			} while (received_bytes > 0);
			// Close the socket
			closesocket(client_socket);
			LCD_UsrLog("Socket server - connection closed\n");
			//GUI_DispString("Socket server - connection closed\n");
		}//else
	}//while(1)

	f_close(&w_log);
	// Close socket
	closesocket(server_socket);

	while (1) {
		osDelay(10);
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
