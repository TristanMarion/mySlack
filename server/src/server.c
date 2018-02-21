#include "includes_server.h"

void display_clients(t_server *server)
{
    t_client *tmp;

    my_putstr_color("blue", "", "\nNumber of connected users : ", 0, 0, 1);
    my_put_nbr(server->clients_list->nb_clients);
    my_putstr_color("cyan", "", "\n\nList of connected users :", 0, 0, 1);
    tmp = server->clients_list->first_client;
    while (tmp != NULL)
    {
        my_putstr("\n\t - ");
        my_put_nbr(tmp->fd_id);
        tmp = tmp->next;
    }
    my_putstr("\n");
}

void welcome_message(t_server *server, t_client *client)
{
    char *message;

    message = generate_message(server->serv_config->welcome_message, 0, client->nickname);
    send_special(client, my_strdup("info"), message);
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
    move_client(server, client, NULL);
    remove_client_from_list(server, client);
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
    config->welcome_message = my_strdup("Bienvenue !");
    config->default_color = my_strdup("white");
    config->default_bg_color = my_strdup("");
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
                if (my_strcmp("welcome", tab[0]) == 0)
                {
                    free(config->welcome_message);
                    config->welcome_message = my_strdup(tab[1]);
                }
                if (my_strcmp("color", tab[0]) == 0)
                {
                    free(config->default_color);
                    config->default_color = my_strdup(format_field(tab[1]));
                }
                if (my_strcmp("bg_color", tab[0]) == 0)
                {
                    free(config->default_bg_color);
                    config->default_bg_color = my_strdup(format_field(tab[1]));
                }
            }
        }
        fclose(file);
    }
    if (config->channels_list == NULL)
    {
        config->channels_list = malloc(sizeof(t_channels_list));
        config->channels_list->first_channel = NULL;
        config->channels_list->last_channel = NULL;
        add_channel(config->channels_list, "General");
    }
    return config;
}

char *format_field(char *field)
{
    if (field[my_strlen(field) - 1] == '\n')
        field[my_strlen(field) - 1] = 0;
    return (field);
}

t_channels_list *get_channels_list(char *channels)
{
    t_channels_list *channels_list;
    int i;
    char **each_channel;

    i = 0;
    channels_list = malloc(sizeof(channels_list));
    channels_list->first_channel = NULL;
    channels_list->last_channel = NULL;
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
    return (NULL);
}

void main_loop(t_server *server)
{
    t_client *current_client;
    int max;
    char message[MAX_LEN];
    int end;

    end = 0;
    while (end == 0)
    {
        FD_ZERO(&(server->fds));
        FD_SET(0, &(server->fds));
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

        if (FD_ISSET(0, &(server->fds)))
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
            before_manage_message(server, message, &end);
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

void before_manage_message(t_server *server, char *message, int *end)
{
    char *command;
	char *core_message;
    char *sent_message;
	
	command = get_message_command(message);
	core_message = get_core_message(message);
	sent_message = generate_message(my_strdup("%s\037%s"), 1, command, core_message);
	server_manage_message(server, end, sent_message);
	free(sent_message);
}

t_client *get_client(t_server *server, char *name)
{
    t_client *current_client;

    current_client = server->clients_list->first_client;
    while (current_client != NULL)
    {
        if (my_strcmp(current_client->nickname, name) == 0)
            return (current_client);
        current_client = current_client->next;
    }
    return (NULL);
}

