#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <stdint.h>
int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET s , new_socket;
    struct sockaddr_in server , client;
    int c;
    char *message;

//    printf("\nInitialising Winsock...");
    printf("****AC TEST SERVER****\n");
    printf("\n");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }

//    printf("Initialised.\n");

    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8004 );

    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    //Listen to incoming connections
    listen(s , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");

    c = sizeof(struct sockaddr_in);
    uint8_t buffer[5];

    char onoff[2][5] = {"OFF", "ON"};

    while( (new_socket = accept(s , (struct sockaddr *)&client, &c)) != INVALID_SOCKET )
    {
        //puts("Connection accepted");

        //Reply to the client
        int received_bytes = 0;
       while (1) {
             received_bytes = recv(new_socket, &buffer, sizeof(buffer), 0);
             if (received_bytes < 1) {
                break;
             }
             printf("New data: ");
             printf("Set temperature: %d || ", buffer[0] + 10 * buffer[1]);
             printf("Swing: %s || ", onoff[buffer[2]]);
             printf("Blade state: %d || ", buffer[3]);
             printf("AC is %s ||\n", onoff[buffer[4]]);

//             for (int i = 0; i < received_bytes; i++) {
//                printf("%d\t", buffer[i]);//
//             }
             Sleep(500);
        }
        closesocket(new_socket);
    }
    closesocket(s);
    WSACleanup();

    return 0;
}
