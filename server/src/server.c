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

int init_server(t_server *server)
{
    put_info("Server up\n");
    if ((bind(server->sockfd, (struct sockaddr *)&(server->serv_addr), sizeof(server->serv_addr))) < 0) // on bind notre socket au port et a l'interface, et on verifie bien le retour
    {
        put_error("bind()");
        return (1);
    }

    if (listen(server->sockfd, 5) < 0) // on dit a notre socket d'ecouter d'eventuelles connexions entrante, avec un buffer de 5 (buffer d'attente, man listen)
    {
        put_error("bind()");
        return (1);
    }
    put_success("Server bind success\n");
    put_info("Waiting for connections\n");
    return (0);
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

void remove_client_from_list(t_server *server, t_client *client)
{
    t_clients_list *clients_list;

    clients_list = server->clients_list;
    if (clients_list->first_client == NULL || client == NULL)
        return;
    if (clients_list->first_client == client)
        clients_list->first_client = client->next;
    if (clients_list->last_client == client)
        clients_list->last_client = client->prev;
    if (client->next != NULL)
        client->next->prev = client->prev;
    if (client->prev != NULL)
        client->prev->next = client->next;
    clients_list->nb_clients--;
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

void poll_events(t_server *server, t_client *client)
{
    int read_size;
    size_t needed;
    char *message;
    t_client *current_client;

    if ((read_size = recv(client->fd_id, server->client_message, 2000, 0)) > 0)
    {
        needed = snprintf(NULL, 0, "%d : %s !\n", client->fd_id, server->client_message) + 1;
        message = malloc(needed);
        snprintf(message, needed, "%d : %s !\n", client->fd_id, server->client_message);
    }
    else
    {
        needed = snprintf(NULL, 0, "%d left the server !\n", client->fd_id) + 1;
        message = malloc(needed);
        snprintf(message, needed, "%d left the server !\n", client->fd_id);
        remove_client_from_list(server, client);
    }
    put_info(message);
    current_client = server->clients_list->first_client;    
    while (current_client != NULL)
    {
        send(current_client->fd_id, message, my_strlen(message), 0);
        current_client = current_client->next;
    }
}

void main_loop(t_server *server)
{
    t_client *current_client;
    int max;
    while (1)
    {
        FD_ZERO(&(server->fds));
        FD_SET(server->sockfd, &(server->fds));
        max = server->sockfd;

        current_client = server->clients_list->first_client;
        while (current_client != NULL)
        {
            FD_SET(current_client->fd_id, &(server->fds));
            max = current_client->fd_id > max ? current_client->fd_id : max;
            current_client = current_client->next;
        }

        if (select(max + 1, &(server->fds), NULL, NULL, NULL) < 0)
        {
            put_error("select()");
            return;
        }

        if (FD_ISSET(server->sockfd, &(server->fds)))
        {
            if (new_client(server) != 0)
            {
                put_error("new client");
                return;
            }
        }

        current_client = server->clients_list->first_client;
        while (current_client != NULL)
        {
            if (FD_ISSET(current_client->fd_id, &(server->fds)))
            {
                poll_events(server, current_client);
            }
            current_client = current_client->next;
        }
    }
}
