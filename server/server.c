#include "server.h"

t_server *create_server(uint port)
{
    t_server *server = malloc(sizeof(t_server));
    server->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->sockfd == -1)
    {
        perror("socket()");
        return NULL;
    }
    server->serv_addr.sin_port = htons(port);
    server->serv_addr.sin_family = AF_INET;
    server->serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    return server;
}