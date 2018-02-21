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

void logout(t_server *server, t_client *client, char **splitted_message)
{
    (void)splitted_message;

    send_special(client, my_strdup("disconnect"), my_strdup("You have been disconnected, bye !"));
    disconnect(server, client);
}
