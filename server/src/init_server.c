#include "includes_server.h"

t_server *create_server(char *path)
{
    t_server *server;
    if ((server = malloc(sizeof(t_server))) == NULL)
    {
        return NULL;
    }
    server->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->sockfd == -1)
    {
        put_error("socket()");
        return NULL;
    }
    server->serv_config = get_config(path);
    server->serv_addr.sin_port = htons(server->serv_config->port);
    server->serv_addr.sin_family = AF_INET;
    server->serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    server->clients_list = new_clients_list();
    return server;
}

int init_server(t_server *server)
{
    put_info("Server up");
    if ((bind(server->sockfd, (struct sockaddr *)&(server->serv_addr), sizeof(server->serv_addr))) < 0)
    {
        put_error("bind()");
        return (1);
    }

    if (listen(server->sockfd, 5) < 0)
    {
        put_error("bind()");
        return (1);
    }
    put_success("Server bind success");
    put_info("Waiting for connections");
    return (0);
}
