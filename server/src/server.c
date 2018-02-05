#include "includes_server.h"

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
        put_error("socket()");
        return NULL;
    }
    server->serv_addr.sin_port = htons(port);
    server->serv_addr.sin_family = AF_INET;
    server->serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    server->clients_list = new_clients_list();
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
        put_error("client error\n");
        return 1;
    }
    client->clilen = sizeof(client->cli_addr);
    client->fd_id = accept(server->sockfd, (struct sockaddr *)&(client->cli_addr), &(client->clilen)); // on accept la nouvelle connexion, tmp contiendra le FD du nouveau client
    if (client->fd_id < 0)                                                                             // on check les erreurs
        put_error("cannot accept\n");
    else
    {
        add_client_to_list(server, client);
        display_clients(server);
    }

    return 0;
}

void add_client_to_list(t_server *server, t_client *client)
{
    notify_new_client(server, client);
    welcome_message(client);
    client->prev = NULL;
    client->next = NULL;
    if (server->clients_list->last_client == NULL)
    {
        server->clients_list->first_client = client;
    }
    else
    {
        client->prev = server->clients_list->last_client;
        server->clients_list->last_client->next = client;
    }
    server->clients_list->last_client = client;
    server->clients_list->nb_clients++;
}

void display_clients(t_server *server)
{
    t_client *tmp;

    my_putstr_color("blue", "\nNumber of connected users : ");
    my_put_nbr(server->clients_list->nb_clients);
    my_putstr_color("cyan", "\n\nList of connected users :");
    tmp = server->clients_list->first_client;
    while (tmp != NULL)
    {
        my_putstr("\n\t - ");
        my_put_nbr(tmp->fd_id);
        tmp = tmp->next;
    }
}

void welcome_message(t_client *client)
{
    char *message;

    message = my_strdup("Welcome to the Tacos Team Server !\r\n");
    send(client->fd_id, message, my_strlen(message), 0);    
    my_putstr_color("magenta", "\n\nAccepted new client with FD "); // TODO : Add client FD
    my_put_nbr(client->fd_id);
    my_putstr("\n");
}

void notify_new_client(t_server *server, t_client *client)
{
    char *message;
    t_client *tmp;
    size_t needed;

    needed = snprintf(NULL, 0, "%d joined the server !\n", client->fd_id) + 1;
    message = malloc(needed);
    snprintf(message, needed, "%d joined the server !\n", client->fd_id);
    tmp = server->clients_list->first_client;
    while (tmp != NULL)
    {
        send(tmp->fd_id, message, my_strlen(message), 0);
        tmp = tmp->next;
    }
    free(message);
}
