#include "includes_server.h"

const t_client_command client_command_array[] = {
    {"send_message", send_message, "Sends a message to all connected users in your channel"},
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
    {"bg_color", bg_color, "Changes your messages' background color"},
    {"list_colors", list_colors, "Lists all available colors for your messages and messages' background"},
    {"reset_color", reset_color, "Reset your messages' color"},
    {"reset_bg_color", reset_bg_color, "Reset your messages' background color"},
    {"logout", logout, "Disconnects you from the server"},
    {NULL, NULL, NULL}};

void manage_message(t_server *server, t_client *client, char *message)
{
    char **splitted_message;
    const t_client_command *command;
    char *response;

    splitted_message = parse_command(message, '\037');
    if ((command = get_command(splitted_message[0])) != NULL)
    {
        command->cmd_ptr(server, client, splitted_message);
        return;
    }
    response = my_strdup("Unknown command. Type /list_commands to show available commands.");
    send_special(client, my_strdup("error"), response);
}

void send_message(t_server *server, t_client *client, char **splitted_message)
{
    t_client *current_client;
    char *message;

    if (client->muted == 1)
    {
        send_special(client, my_strdup("error"), my_strdup("No one cares, you are muted... (probably for a good reason)"));
        return;
    }
    current_client = server->clients_list->first_client;
    message = generate_message(my_strdup("message\037%s\037%s\037%s\037%s\037%s"), 1, client->color, client->bg_color, client->current_channel->name, client->nickname, splitted_message[1]);
    while (current_client != NULL)
    {
        if (current_client->fd_id != client->fd_id && my_strcmp(current_client->current_channel->name, client->current_channel->name) == 0)
            send(current_client->fd_id, message, my_strlen(message), 0);
        current_client = current_client->next;
    }
    free(message);
}

void list_commands(t_server *server, t_client *client, char **splitted_message)
{
    int i;
    int len;
    t_client_command current_command;
    char *all_commands;
    (void)server;
    (void)splitted_message;

    i = 0;
    all_commands = my_strdup("List of all server commands :\n");
    len = my_strlen(all_commands);
    while ((current_command = client_command_array[i]).command != NULL)
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
    const t_client_command *command;
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

    sent_message = generate_message(my_strdup("direct_message\037%s\037%s"), 1, nickname, message);
    send(target, sent_message, my_strlen(sent_message), 0);
    free(sent_message);
}

void notify(t_server *server, t_client *client, char *action, int check_channel)
{
    char *message;
    t_client *current_client;
    char *place;
    char *server_message;

    place = my_strdup(check_channel == 1 ? "channel" : "server");
    message = generate_message(my_strdup("info\037%s %s the %s !"), 1, client->nickname, action, place);
    current_client = server->clients_list->first_client;
    while (current_client != NULL)
    {
        if (current_client->fd_id != client->fd_id)
            if (check_channel == 0 || my_strcmp(current_client->current_channel->name, client->current_channel->name) == 0)
                send(current_client->fd_id, message, my_strlen(message), 0);
        current_client = current_client->next;
    }
    if (client->current_channel != NULL)
        server_message = generate_message(my_strdup("%s %s the %s %s !"), 1, client->nickname, action, place, client->current_channel->name);
    else
        server_message = generate_message(my_strdup("%s %s the %s !"), 1, client->nickname, action, place);
    server_info(server_message);
    free(message);
    free(action);
    free(place);
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
    if (client->muted == 1)
    {
        send_special(client, my_strdup("error"), my_strdup("No one cares, you are muted... (probably for a good reason)"));
        return;
    }
    current_client = server->clients_list->first_client;
    message = generate_message(my_strdup("important\037%s\037%s"), 1, client->nickname, splitted_message[1]);
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
    (void)server;

    change_color(client, splitted_message[0], splitted_message[1]);
}

void bg_color(t_server *server, t_client *client, char **splitted_message)
{
    (void)server;

    change_color(client, splitted_message[0], splitted_message[1]);
}

void change_color(t_client *client, char *mode, char *message)
{
    char **splitted_core_message;
    char *sent_message;
    t_color color;
    char *to_change;

    splitted_core_message = parse_command(message, ' ');
    if (my_strcmp(splitted_core_message[0], "") == 0)
    {
        sent_message = generate_message(my_strdup("Usage : /%s <color>"), 1, mode);
        send_special(client, my_strdup("error"), sent_message);
        return;
    }
    if ((color = get_color(splitted_core_message[0])).color == NULL)
    {
        send_special(client, my_strdup("error"), my_strdup("This color doest'n exists. To check colors list use `/list_colors`"));
        return;
    }
    else if (my_strcmp(color.color, "clear") == 0)
    {
        send_special(client, my_strdup("error"), my_strdup("Forbidden color. To check colors list use `/list_colors`"));
        return;
    }
    to_change = my_strcmp(mode, "color") == 0 ? client->color : client->bg_color;
    free(to_change);
    to_change = my_strdup(splitted_core_message[0]);
    sent_message = generate_message(my_strdup("You changed your messages' %s to %s"), 1, mode, to_change);
    send_special(client, my_strdup("info"), sent_message);
}

void reset_color(t_server *server, t_client *client, char **splitted_message)
{
    (void)server;

    reset_colors(server, client, splitted_message[0]);
}

void reset_bg_color(t_server *server, t_client *client, char **splitted_message)
{
    (void)server;

    reset_colors(server, client, splitted_message[0]);
}

void reset_colors(t_server *server, t_client *client, char *mode)
{
    t_config *config;
    int i_mode;
    char *sent_message;
    char *to_change;

    config = server->serv_config;
    i_mode = my_strcmp(mode, "reset_color") ? 1 : 0;
    to_change = i_mode == 0 ? client->color : client->bg_color;
    free(to_change);
    to_change = my_strdup(i_mode == 0 ? config->default_color : config->default_bg_color);
    sent_message = generate_message(my_strdup("You reseted your messages' %s to %s"), 1, mode, to_change);
    send_special(client, my_strdup("info"), sent_message);
}

void list_colors(t_server *server, t_client *client, char **splitted_message)
{
    int i;
    int len;
    t_color current_color;
    char *all_colors;
    (void)server;
    (void)splitted_message;

    i = 1; /* Skip the `clear` */
    all_colors = my_strdup("List of all server commands :\n");
    len = my_strlen(all_colors);
    while ((current_color = g_color[i]).color != NULL)
    {
        len += my_strlen(current_color.color) + 5;
        all_colors = realloc(all_colors, len);
        my_strcat(all_colors, "\t- ");
        my_strcat(all_colors, current_color.color);
        my_strcat(all_colors, "\n");
        i++;
    }
    send_special(client, my_strdup("info"), all_colors);
}

void logout(t_server *server, t_client *client, char **splitted_message)
{
    (void)splitted_message;

    send_special(client, my_strdup("disconnect"), my_strdup("You have been disconnected, bye !"));
    disconnect(server, client);
}

void move_client(t_server *server, t_client *client, t_channel *target_channel)
{
    if (client->current_channel == target_channel)
        return;
    if (client->current_channel != NULL)
        notify(server, client, my_strdup("left"), 1);
    else
        notify(server, client, my_strdup("joined"), 0);
    client->current_channel = target_channel;
    if (client->current_channel != NULL)
        notify(server, client, my_strdup("joined"), 1);
    else
        notify(server, client, my_strdup("left"), 0);
}

const t_client_command *get_command(char *command)
{
    int i;
    const t_client_command *current_command;

    i = 0;
    while ((current_command = &(client_command_array[i]))->command != NULL)
    {
        if (my_strcmp(current_command->command, command) == 0)
            return current_command;
        i++;
    }
    return NULL;
}
