/* Includes ------------------------------------------------------------------*/
#include "socket_server.h"
#include "lcd_log.h"
#include "cmsis_os.h"
#include "app_ethernet.h"
#include "lwip/sockets.h"
#include "stm32746g_discovery_ts.h"
#include <string.h>
#include "stm32746g_discovery_lcd.h"
#include "ac_client.h"


#define DATA_BUFFER_SIZE    100
#define SERVER_IP           "10.27.99.106"
#define SERVER_PORT         8004
#define CONN_RETRY_COUNT	10

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


int8_t send_command_to_ac(uint8_t *comm)
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
			if (send(client_sock, comm, 5, 0) > 0) {
				closesocket(client_sock);
				//break;
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

void ac_client_thread(void const *argument)
{
	send_command_to_ac((uint8_t*)argument);
	while (1) {
		osThreadTerminate(NULL);
	}
}

void ac_client_init()
{

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
