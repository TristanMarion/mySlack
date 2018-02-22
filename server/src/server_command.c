#include "includes_server.h"

const t_server_command server_command_array[] = {
    {"send_message", server_send_message, "Sends a message to all connected users"},
    {"kick", kick, "Disconnects a user from the server"},
    {"create_channel", create_channel, "Creates a channel"},
    {"remove_channel", remove_channel, "Removes a channel"},
    {"mute", mute, "Mutes a user"},
    {"unmute", unmute, "Unmutes a user"},
    {"list_commands", list_server_commands, "List all available server commands"},
    {"stop", stop, "Stops the server"},
    {NULL, NULL, NULL}};

void server_manage_message(t_server *server, int *end, char *message)
{
    char **splitted_message;
    const t_server_command *command;
    char *response;

    splitted_message = parse_command(message, '\037');
    if ((command = get_server_command(splitted_message[0])) != NULL)
    {
        command->cmd_ptr(server, end, splitted_message);
        return;
    }
    response = my_strdup("Unknown command. Type /list_commands to show available commands.");
    server_error(response);
}

void list_server_commands(t_server *server, int *end, char **splitted_message)
{
    int i;
    int len;
    t_server_command current_command;
    char *all_commands;
    (void)server;
    (void)splitted_message;
    (void)end;

    i = 0;
    all_commands = my_strdup("List of all server commands :\n");
    len = my_strlen(all_commands);
    while ((current_command = server_command_array[i]).command != NULL)
    {
        len += my_strlen(current_command.command) + my_strlen(current_command.description) + 8;
        all_commands = realloc(all_commands, len);
        my_strcat(all_commands, "\t- ");
        my_strcat(all_commands, current_command.command);
        my_strcat(all_commands, " : ");
        my_strcat(all_commands, current_command.description);
        my_strcat(all_commands, "\n");
        i++;
    }
    put_info(my_strdup(all_commands));
}

void stop(t_server *server, int *end, char **splitted_message)
{
    t_client *current_client;
    (void)splitted_message;

    current_client = server->clients_list->first_client;
    while (current_client != NULL)
    {
        send_special(current_client, my_strdup("disconnect"), my_strdup("The server has been stopped"));
        remove_client_from_list(server, current_client);
        current_client = current_client->next;
    }
    server_info(my_strdup("The server will now stop"));
    *end = 1;
}

const t_server_command *get_server_command(char *command)
{
    int i;
    const t_server_command *current_command;

    i = 0;
    while ((current_command = &(server_command_array[i]))->command != NULL)
    {
        if (my_strcmp(current_command->command, command) == 0)
            return current_command;
        i++;
    }
    return NULL;
}

void server_error(char *error)
{
    put_error(error);
    free(error);
}

void server_info(char *info)
{
    put_info(info);
    free(info);
}
