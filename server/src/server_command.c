#include "includes_server.h"

const t_server_command server_command_array[] = {
    {"send_message", server_send_message, "Sends a message to all connected users"},
    {"kick", kick, "Disconnects a user from the server"},
    {"create_channel", create_channel, "Creates a channel"},
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

void server_send_message(t_server *server, int *end, char **splitted_message)
{
    char *sent_message;
    t_client *current_client;
    (void)end;

    if (my_strcmp(splitted_message[1], "") == 0)
        return;
    sent_message = generate_message(my_strdup("server\037%s"), 1, splitted_message[1]);
    current_client = server->clients_list->first_client;
    while (current_client != NULL)
    {
        send(current_client->fd_id, sent_message, my_strlen(sent_message), 0);
        current_client = current_client->next;
    }
    free(sent_message);
}

void kick(t_server *server, int *end, char **splitted_message)
{
    t_client *kicked_client;
    char *message;
    char **splitted_core_message;
    (void)end;

    message = NULL;
    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strlen(splitted_core_message[0]) == 0)
        message = my_strdup("Usage : /kick <nickname>");
    else if ((kicked_client = get_client(server, splitted_core_message[0])) == NULL)
        message = my_strdup("User not found");
    if (message != NULL)
    {
        server_error(message);
        return;
    }
    if (splitted_core_message[1] != NULL)
        message = generate_message(my_strdup("You have been kicked from the server. Reason : %s"),
                                   1, my_implode(splitted_core_message, " ", 1));
    else
        message = my_strdup("You have been kicked from the server.");
    send_special(kicked_client, my_strdup("disconnect"), message);
    remove_client_from_list(server, kicked_client);
}

void create_channel(t_server *server, int *end, char **splitted_message)
{
    char **splitted_core_message;
    (void)end;

    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strlen(splitted_core_message[0]) <= 0)
    {
        server_error(my_strdup("Usage : /create_channel <channel>"));
        return;
    }
    if (!check_channel_availability(server, splitted_core_message[0]))
    {
        server_error(my_strdup("This channel already exists"));
        return;
    }
    add_channel(server->serv_config->channels_list, my_strdup(splitted_core_message[0]));
    server_info(generate_message(my_strdup("Channel %s created"), 1, splitted_core_message[0]));
}

void mute(t_server *server, int *end, char **splitted_message)
{
    t_client *muted_client;
    char *message;
    char **splitted_core_message;
    (void)end;

    message = NULL;
    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strlen(splitted_core_message[0]) == 0)
        message = my_strdup("Usage : /mute <nickname> [reason]");
    else if ((muted_client = get_client(server, splitted_core_message[0])) == NULL)
        message = my_strdup("User not found");
    else if (muted_client->muted == 1)
        message = my_strdup("User is already muted");
    if (message != NULL)
    {
        server_error(message);
        return;
    }
    if (splitted_core_message[1] != NULL)
        message = generate_message(my_strdup("You have been muted. Reason : %s"), 1, my_implode(splitted_core_message, " ", 1));
    else
        message = my_strdup("You have been muted.");
    send_special(muted_client, my_strdup("info"), message);
    put_info(generate_message(my_strdup("%s has been muted."), 1, muted_client->nickname));
    muted_client->muted = 1;
}

void unmute(t_server *server, int *end, char **splitted_message)
{
    t_client *unmuted_client;
    char *message;
    char **splitted_core_message;
    (void)end;

    message = NULL;
    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strlen(splitted_core_message[0]) == 0)
        message = my_strdup("Usage : /unmute <nickname>");
    else if ((unmuted_client = get_client(server, splitted_core_message[0])) == NULL)
        message = my_strdup("User not found");
    else if (unmuted_client->muted == 0)
        message = my_strdup("User is not muted");
    if (message != NULL)
    {
        server_error(message);
        return;
    }
    send_special(unmuted_client, my_strdup("info"), my_strdup("You have been unmuted"));
    put_info(generate_message(my_strdup("%s has been unmuted."), 1, unmuted_client->nickname));
    unmuted_client->muted = 0;
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
    (void) splitted_message;

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
