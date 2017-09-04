/**************************
WEATHER STATION TEST CLIENT
***************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include <conio.h>
#include <windows.h>

#define SERVER_IP           "10.27.99.64"
#define SERVER_PORT         8002
/*Time structure */
typedef struct  {
   int tm_sec;         /* seconds,  range 0 to 59          */
   int tm_min;         /* minutes, range o 59           */
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

hq_data_t buffer;

//float buffer[3] = {23.0, 50.0, 101300};

void handle_error(const char *error_string)
{
	printf("Error: %s\nError code: %d\n", error_string, WSAGetLastError());
	WSACleanup();
	printf("Press any key to exit from the program...");
	while (!kbhit());
	exit(EXIT_FAILURE);
}

void wsa_init()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
		handle_error("WSAStartup() ");
}

void connect_to_server(SOCKET *client_sock, unsigned int server_port, char *server_ip)
{
	// Creating client socket
	(*client_sock) = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
	if (client_sock < 0)
		handle_error("socket() ");

	// Creating server address
	struct sockaddr_in addr_in;
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(server_port);
	addr_in.sin_addr.s_addr = inet_addr(server_ip);

	// Connecting the client socket to the server
	int connect_retval = connect(*client_sock, (struct sockaddr *)&addr_in, sizeof(addr_in));
	if (connect_retval < 0)
		handle_error("connect() ");

	printf("Connected to %s:%d\n", SERVER_IP, SERVER_PORT);
}

float send_message(SOCKET *socket)
{
	int sent_bytes = send(*socket, &buffer, sizeof(buffer), 0);     //added &
	if (sent_bytes < 0)
		handle_error("send() ");

    int flag_0 = 0;
if (flag_0 == 0) {
    buffer.sensor_values[0]++;
} else if (buffer.sensor_values[0]== 50) {
    flag_0 == 1;
    buffer.sensor_values[0]--;
} else if (flag_0 == 1) {
    buffer.sensor_values[0]--;
} else if (buffer.sensor_values[0] == 23 && flag_0 == 1) {
    buffer.sensor_values[0]--;
    flag_0 == 0;
}



// buffer.sensor_values[0]++;
// if ( buffer.sensor_values[0] == 50)
//     buffer.sensor_values[0] = 23;
// buffer.sensor_values[1]++;
// if(buffer.sensor_values[1] == 101)
//    buffer.sensor_values[1] = 50;
// buffer.sensor_values[2]++;
// if (buffer.sensor_values[2] == 101900)
//    buffer.sensor_values[2] = 101300;

buffer.hq_time.tm_sec+=60;
if (buffer.hq_time.tm_sec == 60) {
   buffer.hq_time.tm_min++;
   buffer.hq_time.tm_sec = 0;
   if (buffer.hq_time.tm_min == 60) {
    buffer.hq_time.tm_hour++;
    buffer.hq_time.tm_min = 0;
    if (buffer.hq_time.tm_hour == 24) {
        buffer.hq_time.tm_yday++;
        buffer.hq_time.tm_hour = 0;
        if (buffer.hq_time.tm_yday == 366) {
           buffer.hq_time.tm_year++;
           buffer.hq_time.tm_yday = 0;
        }
    }
   }
 }
 int res_day = buffer.hq_time.tm_yday % 7;
 switch(res_day) {
 case 0:
    buffer.hq_time.tm_wday = 0;
    break;
 case 1:
    buffer.hq_time.tm_wday = 1;
    break;
 case 2:
    buffer.hq_time.tm_wday = 2;
    break;
 case 3:
    buffer.hq_time.tm_wday = 3;
    break;
 case 4:
    buffer.hq_time.tm_wday = 4;
    break;
 case 5:
    buffer.hq_time.tm_wday = 5;
    break;
 case 6:
    buffer.hq_time.tm_wday = 6;
    break;
 }

 int res_month = buffer.hq_time.tm_yday / 30;
 switch(res_month) {
 case 0:
     buffer.hq_time.tm_mon = 0;
     break;
 case 1:
    buffer.hq_time.tm_mon = 1;
    break;
 case 2:
    buffer.hq_time.tm_mon  = 2;
    break;
 case 3:
    buffer.hq_time.tm_mon = 3;
    break;
 case 4:
    buffer.hq_time.tm_mon  = 4;
    break;
case 5:
    buffer.hq_time.tm_mon = 5;
    break;
case 6:
    buffer.hq_time.tm_mon = 6;
    break;
case 7:
    buffer.hq_time.tm_mon  = 7;
    break;
case 8:
    buffer.hq_time.tm_mon  = 8;
    break;
case 9:
    buffer.hq_time.tm_mon  = 9;
    break;
case 10:
    buffer.hq_time.tm_mon  = 10;
    break;
default:
    buffer.hq_time.tm_mon = 11;
    break;
     }
	return sent_bytes;
}

int main()
{

    //print startup message
    printf("****WEATHER STATION TEST CLIENT****\n");
    printf("\n");

    //Add test data to structure
    buffer.sensor_values[0] = 23;
    buffer.sensor_values[1] = 50;
    buffer.sensor_values[2] = 1013;

    buffer.hq_time.tm_hour = 12;
    buffer.hq_time.tm_min = 20;
    buffer.hq_time.tm_sec = 0;

    buffer.hq_time.tm_wday = 2;
    buffer.hq_time.tm_year = 117;
    buffer.hq_time.tm_mon = 0;
    buffer.hq_time.tm_mday = 2;

    buffer.hq_time.tm_yday = 2;
    buffer.hq_time.tm_isdst = 0;  //will not change

	// Initialize the WSA
	wsa_init();

	// Connect to server
	SOCKET client_socket;
	connect_to_server(&client_socket, SERVER_PORT, SERVER_IP);

	// Local variables used in the do-while loop
	float sent_bytes;
		// Send data to the server
    //for(int i = 0; i < 1000; i++) {
    while(1) {
		sent_bytes = send_message(&client_socket);
		printf("Date: %d - %d - %d || Time: %d : %d : %d || temp: %.2f humidity: %.2f pressure: %.2f\n", buffer.hq_time.tm_year + 1900, buffer.hq_time.tm_mon + 1 , buffer.hq_time.tm_mday + 1,
         buffer.hq_time.tm_hour, buffer.hq_time.tm_min, buffer.hq_time.tm_sec, buffer.sensor_values[0], buffer.sensor_values[1], buffer.sensor_values[2]);

		Sleep(1000);
    }

	printf("Closing the socket...\n");
	closesocket(client_socket);
	printf("Cleaning up memory...\n");
	WSACleanup();
	return 0;
}
