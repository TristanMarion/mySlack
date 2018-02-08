#include "includes_server.h"

const t_server_command server_command_array[] = {
    {"send_message", send_message},
    {NULL, NULL}};

void manage_message(t_server *server, t_client *client, char *message)
{
    int i;
    char **splitted_message;
    t_server_command current_command;

    splitted_message = parse_command(message, ';');
    i = 0;
    while ((current_command = server_command_array[i]).command != NULL)
    {
        if (my_strcmp(splitted_message[0], current_command.command) == 0)
        {
            current_command.cmd_ptr(server, client, splitted_message);
        }
        i++;
    }
}

void send_message(t_server *server, t_client *client, char **splitted_message)
{
    t_client *current_client;
    int needed;
    char *message;

    current_client = server->clients_list->first_client;
    needed = snprintf(NULL, 0, "%s : %s", client->nickname, splitted_message[1]) + 1;
    message = malloc(needed);
    snprintf(message, needed, "%s : %s", client->nickname, splitted_message[1]);
    while (current_client != NULL)
    {
        if (current_client->fd_id != client->fd_id)
        {
            send(current_client->fd_id, message, my_strlen(message), 0);
        }
        current_client = current_client->next;
    }
}
