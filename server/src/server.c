#include "server.h"

t_server *create_server(uint port)
{
    t_server *server;
    if ((server = malloc(sizeof(t_server))) == NULL)
    {
        return NULL;
    }
    server->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->sockfd == -1)
    {
        perror("socket()");
        return NULL;
    }
    server->serv_addr.sin_port = htons(port);
    server->serv_addr.sin_family = AF_INET;
    server->serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    server->clients = new_clients_list();
    return server;
}

void init_server(t_server *server)
{
    listen(server->sockfd, 5); // on dit a notre socket d'ecouter d'eventuelles connexions entrante, avec un buffer de 5 (buffer d'attente, man listen)

    FD_ZERO(&(server->fds));                // on init notre fd_set
    FD_SET(server->sockfd, &(server->fds)); // on ne met dans notre fd_set que la socket serveur, les autres FDs ne nous interessent pas pour l'exemple
}

int new_client(t_server *server)
{
    t_client *client;
    if ((client = malloc(sizeof(t_client))) == NULL)
    {
        printf("client error\n");
        return 1;
    }
    client->clilen = sizeof(client->cli_addr);
    client->fd_id = accept(server->sockfd, (struct sockaddr *)&(client->cli_addr), &(client->clilen)); // on accept la nouvelle connexion, tmp contiendra le FD du nouveau client
    if (client->fd_id < 0)                                                                             // on check les erreurs
        printf("cannot accept\n");
    else
        printf("accepted new client with FD %d\n", client->fd_id); // on indique qu'on a un nouveau client

    return 0;
}
