#include "includes_server.h"

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

void logout(t_server *server, t_client *client, char **splitted_message)
{
    (void)splitted_message;

    send_special(client, my_strdup("disconnect"), my_strdup("You have been disconnected, bye !"));
    disconnect(server, client);
}
