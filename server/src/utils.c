#include "includes_server.h"

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

t_color get_color(char *color)
{
    int i;
    t_color current_color;

    i = 0;
    while ((current_color = g_color[i]).color != NULL)
    {
        if (my_strcmp(current_color.color, color) == 0)
            return current_color;
        i++;
    }
    return g_color[i];
}

void send_special(t_client *client, char *special, char *message)
{
    char *sent_message;

    sent_message = generate_message(my_strdup("%s\037%s"), 1, special, message);
    send(client->fd_id, sent_message, my_strlen(sent_message), 0);
    free(special);
    free(message);
    free(sent_message);
}

void write_to_log_file(t_server *server, char *message)
{
    int fd;

    if (my_strcmp(server->serv_config->log_file_path, "None") == 0)
        return;
    fd = open(server->serv_config->log_file_path, O_APPEND | O_RDWR| O_CREAT, 0666);
    write(fd, message, my_strlen(message));
    write(fd, "\n", 1);
    close(fd);
}
