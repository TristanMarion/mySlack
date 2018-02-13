#include "includes_server.h"

const t_server_command server_command_array[] = {
    {"send_message", send_message, "Sends a message to all connected users"},
    {"list_commands", list_commands, "Lists all available commands"},
    {"commands_list", list_commands, "Lists all available commands"},
    {"help", help, "Gives informations about a command, or explains how to get the list of available commands"},
    {"direct_message", direct_message, "Sends a direct message to a user"},
    {"list_channels", list_channels, "Lists all available channels"},
    {"join", join, "Joins the specified channel"},
    {"leave", leave, "Leaves the current channel and gets back to the default channel"},
    {"create", create, "Create a channel"},
    {"ping", ping, "Pings the server"},
    {NULL, NULL, NULL}};

void manage_message(t_server *server, t_client *client, char *message)
{
    char **splitted_message;
    const t_server_command *command;
    char *response;

    splitted_message = parse_command(message, ';');
    if ((command = get_command(splitted_message[0])) != NULL)
    {
        command->cmd_ptr(server, client, splitted_message);
        return;
    }
    response = my_strdup("Unknown command. Type /list_commands to show available commands.");
    send(client->fd_id, response, my_strlen(response), 0);
    free(response);
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
    needed = snprintf(NULL, 0, "[%s] %s : %s", client->current_channel->name, client->nickname, splitted_message[1]) + 1;
    message = malloc(needed);
    snprintf(message, needed, "[%s] %s : %s", client->current_channel->name, client->nickname, splitted_message[1]);
    while (current_client != NULL)
    {
        if (current_client->fd_id != client->fd_id && my_strcmp(current_client->current_channel->name, client->current_channel->name) == 0)
        {
            send(current_client->fd_id, message, my_strlen(message), 0);
        }
        current_client = current_client->next;
    }
    free(message);
}

void send_server_message(t_server *server, char *message)
{
    t_client *current_client;
    int needed;
    char *sent_message;

    current_client = server->clients_list->first_client;
    needed = snprintf(NULL, 0, "[SERVER] %s", message) + 1;
    sent_message = malloc(needed);
    snprintf(sent_message, needed, "[SERVER] %s", message);
    while (current_client != NULL)
    {
        send(current_client->fd_id, sent_message, my_strlen(sent_message), 0);
        current_client = current_client->next;
    }
    free(sent_message);
}

void list_commands(t_server *server, t_client *client, char **splitted_message)
{
    // silence unused variables warnings.
    (void)server;
    (void)splitted_message;
    int i;
    int len;
    t_server_command current_command;
    char *all_commands = NULL;

    i = 0;
    len = my_strlen("List of all server commands :\n");
    all_commands = my_strdup("List of all server commands :\n");
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
    send(client->fd_id, all_commands, my_strlen(all_commands), 0);
    free(all_commands);
}

void help(t_server *server, t_client *client, char **splitted_message)
{
    (void)server;
    const t_server_command *command;
    int needed;
    char *sent_message;
    char **splitted_core_message;

    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strcmp(splitted_core_message[0], "") == 0)
    {
        send_error(client, my_strdup("Usage: /help <command>, available commands : /list_commands"));
        return;
    }
    if ((command = get_command(splitted_core_message[0])) != NULL)
    {
        needed = snprintf(NULL, 0, "%s : %s", command->command, command->description) + 1;
        sent_message = malloc(needed);
        snprintf(sent_message, needed, "%s : %s", command->command, command->description);
        send(client->fd_id, sent_message, my_strlen(sent_message), 0);
        free(sent_message);
        return;
    }
    needed = snprintf(NULL, 0, "Unknown command %s", splitted_core_message[0]) + 1;
    sent_message = malloc(needed);
    snprintf(sent_message, needed, "Unknown command %s", splitted_core_message[0]);
    send_error(client, sent_message);
}

void direct_message(t_server *server, t_client *client, char **splitted_message)
{
    t_client *target_client;
    char **splitted_core_message;

    splitted_core_message = parse_command(splitted_message[1], ' ');
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
    if ((target_client = get_client(server, splitted_core_message[0])) != NULL)
        send_direct_message(client->nickname, target_client->fd_id, my_implode(splitted_core_message, " ", 1));
    else
        send_error(client, my_strdup("User not found"));
    return;
}

void send_direct_message(char *nickname, int target, char *message)
{
    int needed;
    char *sent_message;

    needed = snprintf(NULL, 0, "/!\\ DM - %s : %s", nickname, message) + 1;
    sent_message = malloc(needed);
    snprintf(sent_message, needed, "/!\\ DM - %s : %s", nickname, message);
    send(target, sent_message, my_strlen(sent_message), 0);
    free(sent_message);
}

void list_channels(t_server *server, t_client *client, char **splitted_message)
{
    (void)splitted_message;
    t_channel *current_channel;
    int len;
    int i;
    char *all_channels;

    current_channel = server->serv_config->channels_list->first_channel;
    len = 0;
    i = 0;
    all_channels = NULL;
    while (current_channel != NULL)
    {
        len += my_strlen(current_channel->name) + 5;
        all_channels = realloc(all_channels, len);
        my_strcat(all_channels, "\t- ");
        my_strcat(all_channels, current_channel->name);
        if (my_strcmp(current_channel->name, client->current_channel->name) == 0)
        {
            len += 5;
            all_channels = realloc(all_channels, len);
            my_strcat(all_channels, " <---");
        }
        my_strcat(all_channels, "\n");
        current_channel = current_channel->next;
    }
    send(client->fd_id, all_channels, my_strlen(all_channels), 0);
    free(all_channels);
}

void join(t_server *server, t_client *client, char **splitted_message)
{
    t_channel *current_channel;
    char **splitted_core_message;
    int needed;
    char *sent_message;

    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strcmp(splitted_core_message[0], "") == 0)
    {
        send_error(client, my_strdup("Usage : /join <channel>"));
        return;
    }
    if (my_strcmp(client->current_channel->name, splitted_core_message[0]) == 0)
        return;
    current_channel = server->serv_config->channels_list->first_channel;
    while (current_channel != NULL)
    {
        if (my_strcmp(splitted_core_message[0], current_channel->name) == 0)
        {
            client->current_channel = get_channel(server, my_strdup(splitted_core_message[0]));
            needed = snprintf(NULL, 0, "You enter %s\n", client->current_channel->name) + 1;
            sent_message = malloc(needed);
            snprintf(sent_message, needed, "You enter %s\n", client->current_channel->name);
            send(client->fd_id, sent_message, my_strlen(sent_message), 0);
            notify_channel(server, client, my_strdup("joined"));
            free(sent_message);
            return;
        }
        current_channel = current_channel->next;
    }
    send_error(client, my_strdup("Channel not found"));
}

void notify_channel(t_server *server, t_client *client, char *action)
{
    char *message;
    t_client *current_client;
    size_t needed;

    needed = snprintf(NULL, 0, "%s %s this channel !\n", client->nickname, action) + 1;
    message = malloc(needed);
    snprintf(message, needed, "%s %s this channel !\n", client->nickname, action);
    current_client = server->clients_list->first_client;
    while (current_client != NULL)
    {
        if (my_strcmp(current_client->current_channel->name, client->current_channel->name) == 0 && current_client->fd_id != client->fd_id)
        {
            send(current_client->fd_id, message, my_strlen(message), 0);
        }
        current_client = current_client->next;
    }
    free(message);
    free(action);
}

void leave(t_server *server, t_client *client, char **splitted_message)
{
    char *sent_message;
    int needed;
    (void)splitted_message;
    notify_channel(server, client, my_strdup("left"));
    client->current_channel = get_channel(server, my_strdup(server->serv_config->channels_list->first_channel->name));
    needed = snprintf(NULL, 0, "You are back in %s\n", client->current_channel->name) + 1;
    sent_message = malloc(needed);
    snprintf(sent_message, needed, "You are back in %s\n", client->current_channel->name);
    send(client->fd_id, sent_message, my_strlen(sent_message), 0);
    free(sent_message);
}

void create(t_server *server, t_client *client, char **splitted_message)
{
    char **splitted_core_message;
    char *sent_message;
    int needed;

    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strcmp(splitted_core_message[0], "") == 0)
    {
        send_error(client, my_strdup("Usage : /create <channel>"));
        return;
    }
    if (!check_channel_availability(server, splitted_core_message[0]))
    {
        send_error(client, my_strdup("This channel already exists"));
        return;
    }
    add_channel(server->serv_config->channels_list, my_strdup(splitted_core_message[0]));
    client->current_channel = get_channel(server, my_strdup(splitted_core_message[0]));
    needed = snprintf(NULL, 0, "You create and join %s\n", client->current_channel->name) + 1;
    sent_message = malloc(needed);
    snprintf(sent_message, needed, "You create and join %s\n", client->current_channel->name);
    send(client->fd_id, sent_message, my_strlen(sent_message), 0);
    free(sent_message);
}

int check_channel_availability(t_server *server, char *name)
{
    t_channel *current_channel;

    current_channel = server->serv_config->channels_list->first_channel;
    while (current_channel != NULL)
    {
        if (my_strcmp(current_channel->name, name) == 0)
            return (0);
        current_channel = current_channel->next;
    }
    return (1);
}

void ping(t_server *server, t_client *client, char **splitted_message)
{
    (void)server;
    (void)splitted_message;
    send(client->fd_id, "Pong", 4, 0);
}

const t_server_command *get_command(char *command)
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
