#include "includes_server.h"

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
    server_info(server, generate_message(my_strdup("Channel %s created"), 1, splitted_core_message[0]));
}

void remove_channel(t_server *server, int *end, char **splitted_message)
{
    char **splitted_core_message;
    t_channel *removed_channel;
    (void)end;

    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strlen(splitted_core_message[0]) <= 0)
    {
        server_error(my_strdup("Usage : /remove_channel <channel>"));
        return;
    }
    if ((removed_channel = get_channel(server, my_strdup(splitted_core_message[0]))) == NULL)
    {
        server_error(my_strdup("This channel doesn't exists"));
        return;
    }
    else if (removed_channel == server->serv_config->channels_list->first_channel)
    {
        server_error(my_strdup("You can't delete the default channel"));
        return;
    }
    move_all_clients(server, removed_channel);
    remove_channel_from_list(server, removed_channel);
    server_info(server, generate_message(my_strdup("Channel %s removed"), 1, splitted_core_message[0]));
}

void move_all_clients(t_server *server, t_channel *channel)
{
    char *sent_message;
    t_client *current_client;

    current_client = server->clients_list->first_client;
    sent_message = generate_message(my_strdup("%s channel removed, you have been moved to default channel"), 1, channel->name);
    while (current_client != NULL)
    {
        if (my_strcmp(current_client->current_channel->name, channel->name) == 0)
        {
            send_special(current_client, my_strdup("info"), my_strdup(sent_message));
            move_client(server, current_client, server->serv_config->channels_list->first_channel);
            server_info(server, generate_message(my_strdup("%s has been moved to default channel"), 1, current_client->nickname));
        }
        current_client = current_client->next;
    }
    free(sent_message);
}
