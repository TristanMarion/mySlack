#include "includes_client.h"
#include <string.h>

int hostname_to_ip(char *hostname, char *ip);

int main(int argc, char **argv)
{
    char message[MAX_LEN], server_reply[MAX_LEN], ip[100];
    int end;
    int sock;
    struct sockaddr_in server;
    fd_set fds;

    end = 0;
    if (argc < 4 || argc > 5)
    {
        put_info("Usage : ./client <serv_addr> <port> <nickname> [channel]");
        return (1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        put_error("Could not create socket");
    }

    hostname_to_ip(argv[1], ip);
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(my_getnbr(argv[2]));
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        put_error("connect failed. Error");
        return 1;
    }

    put_success("Connected");
    send_infos(sock, argv);
    while (end == 0)
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
            send_message(sock, message);
        }
        else if (FD_ISSET(sock, &fds))
        {
            if (recv(sock, server_reply, MAX_LEN - 1, 0) > 0)
            {
                handle_message(&end, server_reply);
                my_reset(server_reply, MAX_LEN);
            }
        }
    }
    close(sock);

    return 0;
}

int hostname_to_ip(char *hostname, char *ip)
{
    struct hostent *he;

    if ((he = gethostbyname(hostname)) == NULL)
    {
        put_error("gethostbyname");
        return 1;
    }

    my_strcpy(ip, inet_ntoa(*(struct in_addr *)he->h_addr));
    return 0;
}
