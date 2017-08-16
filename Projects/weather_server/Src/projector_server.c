/* Includes ------------------------------------------------------------------*/
#include "socket_server.h"
#include "lcd_log.h"
#include "cmsis_os.h"
#include "app_ethernet.h"
#include "lwip/sockets.h"
#include "stm32746g_discovery_ts.h"
#include <string.h>
#include "stm32746g_discovery_lcd.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SERVER_QUEUE_SIZE 100
#define SERVER_BUFF_LEN 100
#define PORT 8003
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t ctrl;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void projector_server_thread(void const *argument)
{
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
	LCD_UsrLog("PROJECTOR Socket server - startup...\n");
	LCD_UsrLog("PROJECTOR Socket server - waiting for IP address...\n");

	// Wait for an IP address
	while (!is_ip_ok())
		osDelay(10);
	LCD_UsrLog("PROJECTOR Socket server - IP address is ok\n");

	// Create server socket
	int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (server_socket < 0) {
		LCD_ErrLog("PROJECTOR Socket server - can't create socket\n");
		terminate_thread();
	}
	LCD_UsrLog("PROJECTOR Socket server - socket created\n");

	// Create address structure and bind the socket to it
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(server_socket, (struct sockaddr*)&(server_addr), sizeof(server_addr)) < 0) {
		LCD_ErrLog("PROJECTOR Socket server - can't bind socket\n");
		terminate_thread();
	}
	LCD_UsrLog("PROJECTOR Socket server - socket bind ok\n");

	// Start listening
	if (listen(server_socket, SERVER_QUEUE_SIZE) < 0) {
		LCD_ErrLog("PROJECTOR Socket server - can't listen\n");
		terminate_thread();
	}
	LCD_UsrLog("PROJECTOR Socket server - listening...\n");

	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	int client_socket;
	int send_bytes;
	while (1) {
		// Accept incoming connections
		client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
		LCD_UsrLog("PROJECTOR Socket server - connection accepted\n");
		BSP_LCD_Clear(LCD_COLOR_GREEN);
		// Check the client socket
		if (client_socket < 0) {
			LCD_ErrLog("PROJECTOR Socket server - invalid client socket\n");
		} else {
			uint8_t buffer;
			while (1) {
					//determine int to send based on touch data
					buffer = ctrl;
					send_bytes = send(client_socket, &buffer, sizeof(uint8_t), 0);

					if (send_bytes < 0) {
						break;
					}
					osDelay(1000);
			}//while(1)

			// Close the socket
			closesocket(client_socket);
			LCD_UsrLog("PROJECTOR Socket server - connection closed\n");
		}//else
	}//while(1)

	// Close socket
	closesocket(server_socket);

	while (1) {
		LCD_UsrLog("PROJECTOR Socket server - server socket closed\n");
		osDelay(1000);
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
