#include "includes_client.h"
#include <string.h>

int main(int argc, char **argv)
{
    char server_reply[MAX_LEN];
    if (argc != 3)
    {
        put_info("Usage : ./client serv_addr port\n");
        return (1);
    }

    int sock;
    struct sockaddr_in server;
    //char message[1000] , server_reply[2000];

    //Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        put_error("Could not create socket");
    }
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(my_getnbr(argv[2]));

    //Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        put_error("connect failed. Error");
        return 1;
    }

    put_success("Connected\n");

    //keep communicating with server
    while (1)
    {
        if (recv(sock, server_reply, MAX_LEN -1, 0) > 0)
        {
            put_info(server_reply);
            my_reset(server_reply, MAX_LEN);
        }

        //puts("Server reply :");
        // put_info("Enter message : ");
        // char *message = readline();

        // if (message == NULL)
        // {
        //     message = my_strdup(" ");
        // }
        //Send some data

        // if (send(sock, message, my_strlen(message), 0) < 0)
        // {
        //     put_error("Send failed");
        //     return 1;
        // }

        //Receive a reply from the server

        //memset(server_reply, 0, 255);
    }

    return 0;
}
