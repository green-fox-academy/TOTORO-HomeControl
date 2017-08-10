#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include <conio.h>
#include <windows.h>

#define SERVER_IP           "10.27.99.26"
#define SERVER_PORT         9500
//#define DATA_BUFFER_SIZE    1024

float buffer[3] = {23.0, 50.0, 101300};

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
	// Get the message from the user
	//float buffer[3] = {1.1, 2.2, 3.3};

	//printf("\nEnter the message to send: ");
	//gets(msg);
	// Send the message to the servers


	int sent_bytes = send(*socket, buffer, sizeof(buffer), 0);
	if (sent_bytes < 0)
		handle_error("send() ");

	buffer[0]++;
	buffer[1]++;
	buffer[2] += 100;

	return sent_bytes;
}

int main()
{
	// Initialize the WSA
	wsa_init();

	// Connect to server
	SOCKET client_socket;
	connect_to_server(&client_socket, SERVER_PORT, SERVER_IP);

	// Local variables used in the do-while loop
	float sent_bytes;
	//int received_bytes;
	//char recv_buff[DATA_BUFFER_SIZE];
	//do {
		// Send data to the server
    for(int i = 0; i < 100; i++) {
		sent_bytes = send_message(&client_socket);
		Sleep(10000);
    }
		// Receive the answer if message was sent
		/*if (sent_bytes > 0) {
			received_bytes = recv(client_socket, recv_buff, DATA_BUFFER_SIZE, 0);
			// Error handling
			if (received_bytes < 0) {
				handle_error("recv() ");
			} else {
				// Printing out received string
				recv_buff[received_bytes] = '\0';
				printf("Received string: %s\n", recv_buff);
			}
		}*/
	//} while (sent_bytes > 0);

	printf("Closing the socket...\n");
	closesocket(client_socket);
	printf("Cleaning up memory...\n");
	WSACleanup();
	return 0;

}
