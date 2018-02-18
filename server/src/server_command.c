#include "includes_server.h"

void server_send_message(t_server *server, t_client *client, char **splitted_message)
{
    char *sent_message;
    t_client *current_client;
    (void)client;

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

void kick(t_server *server, t_client *client, char **splitted_message)
{
    t_client *kicked_client;
    char *message;
    char **splitted_core_message;
    (void)client;

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

void create_channel(t_server *server, t_client *client, char **splitted_message)
{
    char **splitted_core_message;
    (void)client;

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

void mute(t_server *server, t_client *client, char **splitted_message)
{
    t_client *muted_client;
    char *message;
    char **splitted_core_message;
    (void)client;

    message = NULL;
    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strlen(splitted_core_message[0]) == 0)
        message = my_strdup("Usage : /mute <nickname> [reason]");
    else if ((muted_client = get_client(server, splitted_core_message[0])) == NULL)
        message = my_strdup("User not found");
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

void unmute(t_server *server, t_client *client, char **splitted_message)
{
    t_client *unmuted_client;
    char *message;
    char **splitted_core_message;
    (void)client;

    message = NULL;
    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strlen(splitted_core_message[0]) == 0)
        message = my_strdup("Usage : /unmute <nickname>");
    else if ((unmuted_client = get_client(server, splitted_core_message[0])) == NULL)
        message = my_strdup("User not found");
    if (message != NULL)
    {
        server_error(message);
        return;
    }
    send_special(unmuted_client, my_strdup("info"), my_strdup("You have been unmuted"));
    put_info(generate_message(my_strdup("%s has been unmuted."), 1, unmuted_client->nickname));
    unmuted_client->muted = 0;
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