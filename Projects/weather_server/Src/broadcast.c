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
#include "broadcast.h"
#include <string.h>

extern uint8_t ip_address[20];

#define PORT 					12345
#define WEATHER_SERVER_PORT		8002
#define BROADCAST_IP        	"255.255.255.255"
#define BC_UNIQUE_STR        	"SMARTHOME_HQ"

void socket_broadcast_thread(void const *argument)
{
	while (!is_ip_ok()) {
		osDelay(100);
	}
	struct bcst {
		char uniq_str[20];
		uint16_t port;
		uint8_t ip[20];
	}broadcast_msg;

	strcpy(broadcast_msg.uniq_str, BC_UNIQUE_STR);
	broadcast_msg.port = WEATHER_SERVER_PORT;
	memcpy(broadcast_msg.ip, ip_address, strlen(ip_address)+1);
//	memcpy(broadcast_msg.ip, "Szia", strlen("Szia")+1);

	// Create broadcast socket
	    int broadcast_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	    if (broadcast_socket < 0) {
	        printf("Error: send_broadcast_msg - socket()\n");
	        terminate_thread();
	    }
	    int brodadc = 1;
	    // Set the socket as broadcasting socket
	    int setsockopt_retval = setsockopt(broadcast_socket, SOL_SOCKET, SO_BROADCAST, (void *)&brodadc, sizeof(brodadc));
	    if (setsockopt_retval < 0) {
	        printf("Error: send_broadcast_msg - setsockopt()\n");
	        terminate_thread();
	    }

	    // Set up destination address
	    struct sockaddr_in broadcast_addr;
	    broadcast_addr.sin_family = AF_INET;
	    broadcast_addr.sin_port = htons(PORT);
	    broadcast_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);

	    // Send broadcast message
	    while (1) {
			sendto(broadcast_socket, &broadcast_msg, sizeof(broadcast_msg), 0,
				  (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr));
			printf("SENT\n");

			osDelay(1000);
	    }

	    // Close the broadcast socket
	    closesocket(broadcast_socket);
}
