#include "includes_client.h"
#include <string.h>

int main(int argc, char **argv)
{
    char message[MAX_LEN], server_reply[MAX_LEN];
    if (argc != 3)
    {
        put_info("Usage : ./client serv_addr port\n");
        return (1);
    }

    int sock;
    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        put_error("Could not create socket");
    }
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(my_getnbr(argv[2]));

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        put_error("connect failed. Error");
        return 1;
    }

    put_success("Connected\n");

    fd_set fds;

    while (1)
    {
        FD_ZERO(&fds);
        FD_SET(0, &fds);
        FD_SET(sock, &fds);
        if (select(sock + 1, &fds, NULL, NULL, NULL) < 0)
        {
            put_error("select()");
            return (1);
        }

        if (FD_ISSET(0, &fds))
        {
            read(0, message, MAX_LEN - 1);
            {
                char *p = NULL;
                p = my_strstr(message, "\n");
                if (p != NULL)
                {
                    *p = 0;
                }
                else
                {
                    message[MAX_LEN - 2] = '\n';
                    message[MAX_LEN - 1] = 0;
                }
            }
            send(sock, message, my_strlen(message), 0);
        }
        else if (FD_ISSET(sock, &fds))
        {
            if (recv(sock, server_reply, MAX_LEN - 1, 0) > 0)
            {
                put_info(server_reply);
                my_reset(server_reply, MAX_LEN);
            }
        }
    }

    return 0;
}
