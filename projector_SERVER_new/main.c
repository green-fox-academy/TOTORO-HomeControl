#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>


int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET s , new_socket;
    struct sockaddr_in server , client;
    int c;
    char *message;

        if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }

   printf("****PROJECTOR TEST SERVER***\n");

    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8003 );
//    server.sin_port = htons( 16000 );

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
    int buffer = 0;

    while( (new_socket = accept(s , (struct sockaddr *)&client, &c)) != INVALID_SOCKET )
    {
        //Reply to the client
        int received_bytes = 0;

        //do {
             received_bytes = recv(new_socket, &buffer, sizeof(buffer), 0);
             //printf("Buffer: %d ", buffer);
             switch(buffer) {
                case 1:
                    printf("Projector canvas UP\n");
                    break;
                case 2:
                    printf("Projector canvas STOP\n");;
                    break;
                case 3:
                    printf("Projector canvas DOWN\n");
                    break;
             }
             Sleep(100);

        closesocket(new_socket);
    }
    closesocket(s);
    WSACleanup();
    return 0;
}
