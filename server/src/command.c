#include "includes_server.h"

const t_server_command server_command_array[] = {
    {"send_message", send_message},
    {"list_commands", list_commands},
    {"direct_message", direct_message},
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

void send_error(t_client *client, char *error_message)
{
    send(client->fd_id, error_message, my_strlen(error_message), 0);
    free(error_message);
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

void list_commands(t_server *server, t_client *client, char **splitted_message)
{
    // silence unused variables warnings.
    (void)server;
    (void)splitted_message;
    int i;
    t_server_command current_command;

    i = 0;
    while ((current_command = server_command_array[i]).command != NULL)
    {
        send(client->fd_id, current_command.command, my_strlen(current_command.command), 0);
        i++;
    }
}

void direct_message(t_server *server, t_client *client, char **splitted_message)
{
    t_client *current_client;
    char **splitted_core_message;

    splitted_core_message = parse_command(splitted_message[1], ' ');
    current_client = server->clients_list->first_client;
    if (splitted_core_message[0] == NULL || splitted_core_message[1] == NULL)
    {
        send_error(client, my_strdup("Usage : /direct_message <nickname> <message>"));
        return;
    }
    if (my_strcmp(client->nickname, splitted_core_message[0]) == 0)
    {
        send_error(client, my_strdup("WHAT THE FUCK ARE YOU DOING"));
        return;
    }
    while (current_client != NULL)
    {
        if (my_strcmp(current_client->nickname, splitted_core_message[0]) == 0)
        {
            // TODO : Use implode function to send the whole splitted_core_message except first element
            send_direct_message(client, current_client->fd_id, splitted_core_message[1]);
            return;
        }
        current_client = current_client->next;
    }
    send_error(client, my_strdup("User not found"));
    return;
}

void send_direct_message(t_client *client, int target, char *message)
{
    int needed;
    char *sent_message;

    needed = snprintf(NULL, 0, "%s : %s", client->nickname, message) + 1;
    sent_message = malloc(needed);
    snprintf(sent_message, needed, "%s : %s", client->nickname, message);
    send(target, sent_message, my_strlen(sent_message), 0);
    free(sent_message);
}
