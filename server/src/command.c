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
    {"nickname", nickname, "Changes your nickname"},
    {"important", important, "Sends a message to every connected user"},
    {"color", color, "Changes your messages' color"},
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
    send_special(client, my_strdup("error"), response);
}

void send_special(t_client *client, char *special, char *message)
{
    char *sent_message;

    sent_message = generate_message(my_strdup("%s;%s"), 1, special, message);
    send(client->fd_id, sent_message, my_strlen(sent_message), 0);
    free(special);
    free(message);
    free(sent_message);
}

void send_message(t_server *server, t_client *client, char **splitted_message)
{
    t_client *current_client;
    char *message;

    current_client = server->clients_list->first_client;
    message = generate_message(my_strdup("message;%s;%s;%s;%s"), 1, client->color, client->current_channel->name, client->nickname, splitted_message[1]);
    while (current_client != NULL)
    {
        if (current_client->fd_id != client->fd_id && my_strcmp(current_client->current_channel->name, client->current_channel->name) == 0)
            send(current_client->fd_id, message, my_strlen(message), 0);
        current_client = current_client->next;
    }
    free(message);
}

void send_server_message(t_server *server, char *message)
{
    char *sent_message;
    t_client *current_client;

    sent_message = generate_message(my_strdup("server;%s"), 1, message);
    current_client = server->clients_list->first_client;
    while (current_client != NULL)
    {
        send(current_client->fd_id, sent_message, my_strlen(sent_message), 0);
        current_client = current_client->next;
    }
    free(sent_message);
}

void list_commands(t_server *server, t_client *client, char **splitted_message)
{
    int i;
    int len;
    t_server_command current_command;
    char *all_commands;
    (void)server;
    (void)splitted_message;

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
    send_special(client, my_strdup("info"), all_commands);
}

void help(t_server *server, t_client *client, char **splitted_message)
{
    const t_server_command *command;
    char *sent_message;
    char **splitted_core_message;
    (void)server;

    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strcmp(splitted_core_message[0], "") == 0)
    {
        send_special(client, my_strdup("error"), my_strdup("Usage: /help <command>, available commands : /list_commands"));
        return;
    }
    if ((command = get_command(splitted_core_message[0])) != NULL)
    {
        sent_message = generate_message(my_strdup("%s : %s"), 1, command->command, command->description);
        send_special(client, my_strdup("info"), sent_message);
        return;
    }
    sent_message = generate_message(my_strdup("Unknown command %s"), 1, splitted_core_message[0]);
    send_special(client, my_strdup("error"), sent_message);
}

void direct_message(t_server *server, t_client *client, char **splitted_message)
{
    t_client *target_client;
    char **splitted_core_message;

    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (splitted_core_message[0] == NULL || splitted_core_message[1] == NULL)
    {
        send_special(client, my_strdup("error"), my_strdup("Usage : /direct_message <nickname> <message>"));
        return;
    }
    if (my_strcmp(client->nickname, splitted_core_message[0]) == 0)
    {
        send_special(client, my_strdup("error"), my_strdup("WHAT THE FUCK ARE YOU DOING"));
        return;
    }
    if ((target_client = get_client(server, splitted_core_message[0])) != NULL)
        send_direct_message(client->nickname, target_client->fd_id, my_implode(splitted_core_message, " ", 1));
    else
        send_special(client, my_strdup("error"), my_strdup("User not found"));
    return;
}

void send_direct_message(char *nickname, int target, char *message)
{
    char *sent_message;

    sent_message = generate_message(my_strdup("direct_message;%s;%s"), 1, nickname, message);
    send(target, sent_message, my_strlen(sent_message), 0);
    free(sent_message);
}

void list_channels(t_server *server, t_client *client, char **splitted_message)
{
    t_channel *current_channel;
    int len;
    char *all_channels;
    (void)splitted_message;

    current_channel = server->serv_config->channels_list->first_channel;
    len = 0;
    all_channels = my_strdup("List of all server channels :\n");
    len = my_strlen(all_channels);
    while (current_channel != NULL)
    {
        len += my_strlen(current_channel->name) + 4;
        all_channels = realloc(all_channels, len);
        if (my_strcmp(current_channel->name, client->current_channel->name) == 0)
        {
            len += 6;
            all_channels = realloc(all_channels, len);
            my_strcat(all_channels, "--->");
        }
        my_strcat(all_channels, "\t- ");
        my_strcat(all_channels, current_channel->name);
        my_strcat(all_channels, "\n");
        current_channel = current_channel->next;
    }
    send_special(client, my_strdup("info"), all_channels);
}

void join(t_server *server, t_client *client, char **splitted_message)
{
    t_channel *current_channel;
    char **splitted_core_message;
    char *sent_message;

    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strcmp(splitted_core_message[0], "") == 0)
    {
        send_special(client, my_strdup("error"), my_strdup("Usage : /join <channel>"));
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
            sent_message = generate_message(my_strdup("You enter %s"), 1, client->current_channel->name);
            send_special(client, my_strdup("info"), sent_message);
            notify_channel(server, client, my_strdup("joined"));
            return;
        }
        current_channel = current_channel->next;
    }
    send_special(client, my_strdup("error"), my_strdup("Channel not found"));
}

void notify_channel(t_server *server, t_client *client, char *action)
{
    char *message;
    t_client *current_client;

    message = generate_message(my_strdup("info;%s %s this channel !"), 1, client->nickname, action);
    current_client = server->clients_list->first_client;
    while (current_client != NULL)
    {
        if (my_strcmp(current_client->current_channel->name, client->current_channel->name) == 0 && current_client->fd_id != client->fd_id)
            send(current_client->fd_id, message, my_strlen(message), 0);
        current_client = current_client->next;
    }
    free(message);
    free(action);
}

void leave(t_server *server, t_client *client, char **splitted_message)
{
    char *sent_message;
    (void)splitted_message;

    notify_channel(server, client, my_strdup("left"));
    client->current_channel = get_channel(server, my_strdup(server->serv_config->channels_list->first_channel->name));
    sent_message = generate_message(my_strdup("You are back in %s"), 1, client->current_channel->name);
    send_special(client, my_strdup("info"), sent_message);
}

void create(t_server *server, t_client *client, char **splitted_message)
{
    char **splitted_core_message;
    char *sent_message;

    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strcmp(splitted_core_message[0], "") == 0)
    {
        send_special(client, my_strdup("error"), my_strdup("Usage : /create <channel>"));
        return;
    }
    if (!check_channel_availability(server, splitted_core_message[0]))
    {
        send_special(client, my_strdup("error"), my_strdup("This channel already exists"));
        return;
    }
    add_channel(server->serv_config->channels_list, my_strdup(splitted_core_message[0]));
    client->current_channel = get_channel(server, my_strdup(splitted_core_message[0]));
    sent_message = generate_message(my_strdup("You create and join %s"), 1, client->current_channel->name);
    send_special(client, my_strdup("info"), sent_message);
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
    char **splitted_core_message;
    t_client *target;
    int needed;
    char *sent_message;
    (void)server;

    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strcmp(splitted_core_message[0], "") == 0)
    {
        send_special(client, my_strdup("info"), my_strdup("Pong !"));
        return;
    }
    if (my_strcmp(splitted_core_message[0], client->nickname) == 0)
    {
        send_special(client, my_strdup("info"), my_strdup("Did you understand the purpose of this command ? 🤔"));
        return;
    }
    if ((target = get_client(server, splitted_core_message[0])) != NULL)
    {
        needed = snprintf(NULL, 0, "%s pinged you !", client->nickname) + 1;
        sent_message = malloc(needed);
        snprintf(sent_message, needed, "%s pinged you !", client->nickname);
        send_special(target, my_strdup("info"), sent_message);
        return;
    }
    sent_message = my_strdup("Your ping didn't find its target and is now lost in space and time :(");
    send_special(client, my_strdup("info"), sent_message);
}

void nickname(t_server *server, t_client *client, char **splitted_message)
{
    char *sent_message;
    char **splitted_core_message;

    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strcmp(splitted_core_message[0], "") == 0)
    {
        send_special(client, my_strdup("error"), my_strdup("Usage : /nickname <nickname>"));
        return;
    }
    if (get_client(server, splitted_core_message[0]) != NULL)
    {
        send_special(client, my_strdup("error"), my_strdup("This nickname is already taken"));
        return;
    }
    my_strcpy(client->nickname, splitted_core_message[0]);
    sent_message = generate_message(my_strdup("You changed your nickname to %s"), 1, client->nickname);
    send_special(client, my_strdup("info"), sent_message);
}

void important(t_server *server, t_client *client, char **splitted_message)
{
    char *message;
    t_client *current_client;

    if (my_strlen(splitted_message[1]) == 0)
    {
        send_special(client, my_strdup("error"), my_strdup("Usage : /important <message>"));
        return;
    }
    current_client = server->clients_list->first_client;
    message = generate_message(my_strdup("important;%s;%s"), 1, client->nickname, splitted_message[1]);
    while (current_client != NULL)
    {
        if (current_client->fd_id != client->fd_id)
            send(current_client->fd_id, message, my_strlen(message), 0);
        current_client = current_client->next;
    }
    free(message);
}

void color(t_server *server, t_client *client, char **splitted_message)
{
    char *sent_message;
    char **splitted_core_message;
    (void)server;

    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strcmp(splitted_core_message[0], "") == 0)
    {
        send_special(client, my_strdup("error"), my_strdup("Usage : /color <color>"));
        return;
    }
    if (get_color(splitted_core_message[0]).color == NULL)
    {
        send_special(client, my_strdup("error"), my_strdup("This color doest'n exists. To check colors list use `/list_colors`"));
        return;
    }
    free(client->color);
    client->color = my_strdup(splitted_core_message[0]);
    sent_message = generate_message(my_strdup("You changed your messgages' color to %s"), 1, client->color);
    send_special(client, my_strdup("info"), sent_message);
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