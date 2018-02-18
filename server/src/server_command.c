#include "includes_server.h"

void server_send_message(t_server *server, t_client *client, char **splitted_message)
{
    char *sent_message;
    t_client *current_client;
    (void)client;

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
    char *error_message;
    (void)client;

    error_message = NULL;
    if (my_strlen(splitted_message[1]) == 0)
        error_message = my_strdup("Usage : /kick <nickname>");
    /* Maybe should we replace `splitted_message[1]` with a `parse_command(splitted_message[1], ' ')[0]` */
    else if ((kicked_client = get_client(server, splitted_message[1])) == NULL)
        error_message = my_strdup("User not found");
    if (error_message != NULL)
    {
        server_error(error_message);
        return;
    }
    send_special(kicked_client, my_strdup("disconnect"), my_strdup("You have been kicked from the server."));
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