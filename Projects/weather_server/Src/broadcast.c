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

#define PORT 					12345
#define WEATHER_SERVER_PORT		8002
#define BROADCAST_IP        	"255.255.255.255"
#define BC_UNIQUE_STR        	"SMARTHOME_HQ"

void socket_broadcast_thread(void const *argument)
{
	struct bcst {
		char uniq_str[15];
		uint16_t port;
		uint32_t ip;
	}broadcast_msg;
	strcpy(broadcast_msg.uniq_str, BC_UNIQUE_STR);
	broadcast_msg.port = WEATHER_SERVER_PORT;
	broadcast_msg.ip = ip_address[20];
	// Create broadcast socket
	    int broadcast_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	    if (broadcast_socket < 0) {
	        printf("Error: send_broadcast_msg - socket()\n");
	        terminate_thread();
	    }

	    // Set the socket as broadcasting socket
	    int setsockopt_retval = setsockopt(broadcast_socket, SOL_SOCKET, SO_BROADCAST, "1", 1);
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
			sendto(broadcast_socket, &broadcast_msg, strlen(BC_UNIQUE_STR), 0,
				  (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr));
			osDelay(10000);
	    }

//	    if (sendto_retval < strlen(BC_UNIQUE_STR)) {
//	        printf("Error: send_broadcast_msg - sendto()\n");
//	        terminate_thread();
//	    }

	    // Close the broadcast socket
	    closesocket(broadcast_socket);
}
