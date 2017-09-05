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
#include "projector_client.h"

/* Private typedef -----------------------------------------------------------*/
/*Time structure */
typedef struct  {
   int tm_sec;         /* seconds,  range 0 to 59          */
   int tm_min;         /* minutes, range 0 to 59           */
   int tm_hour;        /* hours, range 0 to 23             */
   int tm_mday;        /* day of the month, range 1 to 31  */
   int tm_mon;         /* month, range 0 to 11             */
   int tm_year;        /* The number of years since 1900   */
   int tm_wday;        /* day of the week, range 0 to 6    */
   int tm_yday;        /* day in the year, range 0 to 365  */
   int tm_isdst;       /* daylight saving time             */
}rtc_time;

/*Structure for sending data to HQ */
typedef struct {
	float sensor_values[3];	// Storing Temperature, Humidity and Pressure values
	rtc_time hq_time;
}hq_data_t;

hq_data_t received_data;

/* Private define ------------------------------------------------------------*/
#define SERVER_QUEUE_SIZE 100
#define SERVER_BUFF_LEN 100
#define PORT 8002

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
float received_weather_data[3];


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

	// Create server socket
	int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (server_socket < 0) {
		LCD_ErrLog("Socket server - can't create socket\n");
		terminate_thread();
	}
	LCD_UsrLog("Socket server - socket created\n");

	// Create address structure and bind the socket to it
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(server_socket, (struct sockaddr*)&(server_addr), sizeof(server_addr)) < 0) {
		LCD_ErrLog("Socket server - can't bind socket\n");
		terminate_thread();
	}
	LCD_UsrLog("Socket server - socket bind ok\n");

	// Start listening
	if (listen(server_socket, SERVER_QUEUE_SIZE) < 0) {
		LCD_ErrLog("Socket server - can't listen\n");
		terminate_thread();
	}
	LCD_UsrLog("Socket server - listening...\n");

	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	int client_socket;

	while (1) {
		// Accept incoming connections
		client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
		LCD_UsrLog("Socket server - connection accepted\n");
		BSP_LCD_Clear(LCD_COLOR_GREEN);
		// Check the client socket
		if (client_socket < 0) {
			LCD_ErrLog("Socket server - invalid client socket\n");
		} else {
			// Receive data
			float received_bytes;
			do {
//				received_bytes = recv(client_socket, received_weather_data, sizeof(received_weather_data), 0);
//				LCD_UsrLog("Temperature: %.1f C, Humidity: %.1f%%, Pressure: %.1f Pa,\n", received_weather_data[0], received_weather_data[1], received_weather_data[2]);

				received_bytes = recv(client_socket, &received_data, sizeof(received_data), 0);	//added &

				received_weather_data[0] = received_data.sensor_values[0];
				received_weather_data[1] = received_data.sensor_values[1];
				received_weather_data[2] = received_data.sensor_values[2];

				gui_update_temp(received_data.sensor_values[0]);
				gui_update_hum(received_data.sensor_values[1]);
				gui_update_press(received_data.sensor_values[2]);
				gui_update_time(received_data.hq_time.tm_hour, received_data.hq_time.tm_min, received_data.hq_time.tm_sec);
				gui_update_date(received_data.hq_time.tm_year, received_data.hq_time.tm_mon, received_data.hq_time.tm_mday, received_data.hq_time.tm_wday);
			} while (received_bytes > 0);

			// Close the socket
			closesocket(client_socket);
			LCD_UsrLog("Socket server - connection closed\n");
		}
	}

	// Close socket
	closesocket(server_socket);

	while (1) {
		osDelay(10);
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
