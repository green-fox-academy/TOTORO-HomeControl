/* Includes ------------------------------------------------------------------*/
#include "socket_server.h"
#include "lcd_log.h"
#include "cmsis_os.h"
#include "app_ethernet.h"
#include "lwip/sockets.h"
#include "stm32746g_discovery_ts.h"
#include <string.h>
#include "stm32746g_discovery_lcd.h"
#include "projector_client.h"
#define SERVER_IP           "10.27.99.78"
#define SERVER_PORT         8003

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#define CONN_RETRY_COUNT	10

int8_t send_command_to_projector_screen(uint8_t comm)
{
	int client_sock;
	client_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (client_sock < 0) {
		return -1;
	}

	struct sockaddr_in addr_in;
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(SERVER_PORT);
	addr_in.sin_addr.s_addr = inet_addr(SERVER_IP);

	uint8_t i;
	for (i = 0; i < CONN_RETRY_COUNT; i++) {
		if (connect(client_sock, (struct sockaddr *)&addr_in, sizeof(addr_in)) == 0) {
			if (send(client_sock, &comm, sizeof(comm), 0) > 0) {
				closesocket(client_sock);
			} else {
				closesocket(client_sock);
			}
		}
		osDelay(100);
	}

	if (i == CONN_RETRY_COUNT)
		return -1;

	return 0;
}



void projector_client_thread(void const *argument)
{
	send_command_to_projector_screen(argument);
	while (1) {
//		osDelay(10);
		osThreadTerminate(NULL);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
