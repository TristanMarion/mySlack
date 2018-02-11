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
    put_info("Server up\n");
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
    put_success("Server bind success\n");
    put_info("Waiting for connections\n");
    return (0);
}

int new_client(t_server *server)
{
    t_client *client;
    char *error_message;
    if ((client = malloc(sizeof(t_client))) == NULL)
    {
        put_error("client error\n");
        return 1;
    }
    client->clilen = sizeof(client->cli_addr);
    client->fd_id = accept(server->sockfd, (struct sockaddr *)&(client->cli_addr), &(client->clilen));
    if (client->fd_id < 0)
        put_error("cannot accept\n");
    else
    {
        recv(client->fd_id, client->nickname, NICKNAME_MAX_LEN, 0);
        if (server->serv_config->max_clients <= server->clients_list->nb_clients)
        {
            error_message = my_strdup("Sorry the server is full. Please log out and try again.");
            send(client->fd_id, error_message, my_strlen(error_message), 0);
            return 0;
        }
        if (!check_nickname(server, client))
            return 0;
        add_client_to_list(server, client);
        display_clients(server);
    }
    return 0;
}

int check_nickname(t_server *server, t_client *client)
{
    t_client *current_client;
    char *error_message;
    int is_nickname_ok;

    current_client = server->clients_list->first_client;
    error_message = my_strdup("Sorry, this username is already used. Please log out and try another one.");
    is_nickname_ok = 1;
    while (current_client != NULL && is_nickname_ok)
    {
        if (my_strcmp(current_client->nickname, client->nickname) == 0)
        {
            is_nickname_ok = 0;
        }
        current_client = current_client->next;
    }
    if (!is_nickname_ok)
        send(client->fd_id, error_message, my_strlen(error_message), 0);
    free(error_message);
    return (is_nickname_ok);
}

void add_client_to_list(t_server *server, t_client *client)
{
    notify_new_client(server, client);
    welcome_message(client);
    client->current_channel = server->serv_config->channels_list->first_channel;
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
    my_putstr("\n");
}

void welcome_message(t_client *client)
{
    char *message;
    int needed;

    message = my_strdup("Welcome to the Tacos Team Server !\r\n");
    send(client->fd_id, message, my_strlen(message), 0);
    free(message);
    needed = snprintf(NULL, 0, "%s joined the server with FD %d !\n", client->nickname, client->fd_id) + 1;
    message = malloc(needed);
    snprintf(message, needed, "%s joined the server with FD %d !\n", client->nickname, client->fd_id);
    put_info(message);
    free(message);
}

void notify_new_client(t_server *server, t_client *client)
{
    char *message;
    t_client *tmp;
    size_t needed;

    needed = snprintf(NULL, 0, "%s joined the server !\n", client->nickname) + 1;
    message = malloc(needed);
    snprintf(message, needed, "%s joined the server !\n", client->nickname);
    tmp = server->clients_list->first_client;
    while (tmp != NULL)
    {
        send(tmp->fd_id, message, my_strlen(message), 0);
        tmp = tmp->next;
    }
    free(message);
}

void add_channel(t_channels_list *channels_list, char *name)
{
    t_channel *channel;

    channel = malloc(sizeof(t_channel));
    channel->name = my_strdup(name);
    channel->prev = NULL;
    channel->next = NULL;
    if (channels_list->last_channel == NULL)
    {
        channels_list->first_channel = channel;
    }
    else
    {
        channel->prev = channels_list->last_channel;
        channels_list->last_channel->next = channel;
    }
    channels_list->last_channel = channel;
}

void poll_events(t_server *server, t_client *client)
{
    int read_size;
    char read[MAX_LEN];

    if ((read_size = recv(client->fd_id, read, MAX_LEN - 1, 0)) > 0)
    {
        read[read_size] = 0;
        manage_message(server, client, read);
    }
    else
    {
        disconnect(server, client);
    }
}

void disconnect(t_server *server, t_client *client)
{
    int needed;
    char *message;
    t_client *current_client;

    needed = snprintf(NULL, 0, "%s left the server !\n", client->nickname) + 1;
    message = malloc(needed);
    snprintf(message, needed, "%s left the server !\n", client->nickname);
    put_info(message);
    remove_client_from_list(server, client);
    current_client = server->clients_list->first_client;
    while (current_client != NULL)
    {
        send(current_client->fd_id, message, my_strlen(message), 0);
        current_client = current_client->next;
    }
    free(message);
}

t_config *get_config(char *path)
{
    t_config *config;
    FILE *file;
    char buffer[255];
    char **tab;

    config = malloc(sizeof(t_config));
    config->port = 12345;
    config->max_clients = 4;
    config->channels_list = NULL;
    if ((file = fopen(path, "r")))
    {
        while (fgets(buffer, 255, file) != NULL)
        {
            tab = parse_command(buffer, ':');
            if (tab[0] != NULL && tab[1] != NULL)
            {
                if (my_strcmp("port", tab[0]) == 0)
                    config->port = my_getnbr(tab[1]);
                if (my_strcmp("max_clients", tab[0]) == 0)
                    config->max_clients = my_getnbr(tab[1]);
                if (my_strcmp("channels", tab[0]) == 0)
                    config->channels_list = get_channels_list(tab[1]);
            }
        }
        fclose(file);
    }
    if (config->channels_list == NULL)
    {
        config->channels_list = malloc(sizeof(t_channels_list));
        add_channel(config->channels_list, "General");
    }
    return config;
}

t_channels_list *get_channels_list(char *channels)
{
    t_channels_list *channels_list;
    int i;
    char **each_channel;
    
    i = 0;
    channels_list = malloc(sizeof(channels_list));
    if (channels[my_strlen(channels) - 1] == '\n')
       channels[my_strlen(channels) - 1] = 0; 
    each_channel = parse_command(channels, ',');
    while (each_channel[i])
    {
        add_channel(channels_list, each_channel[i]);
        i++;
    }
    return (channels_list);
}

t_channel *get_channel(t_server *server, char *name)
{
    t_channel *current_channel;

    current_channel = server->serv_config->channels_list->first_channel;
    while (current_channel != NULL)
    {
        if (my_strcmp(current_channel->name, name) == 0)
        {
            free(name);
            return (current_channel);
        }
        current_channel = current_channel->next;
    }
    free(name);
    return (server->serv_config->channels_list->first_channel);
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
