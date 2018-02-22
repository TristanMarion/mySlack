#include "includes_server.h"

void join(t_server *server, t_client *client, char **splitted_message)
{
    char **splitted_core_message;
    char *sent_message;
    t_channel *target_channel;

    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strcmp(splitted_core_message[0], "") == 0)
    {
        send_special(client, my_strdup("error"), my_strdup("Usage : /join <channel>"));
        return;
    }
    else if ((target_channel = get_channel(server, splitted_core_message[0])) == NULL)
    {
        send_special(client, my_strdup("error"), my_strdup("Channel not found"));
        return;
    }
    else if (my_strcmp(client->current_channel->name, target_channel->name) == 0)
    {
        send_special(client, my_strdup("error"), my_strdup("You already are in this channel"));
        return;
    }
    move_client(server, client, target_channel);
    sent_message = generate_message(my_strdup("You enter %s"), 1, client->current_channel->name);
    send_special(client, my_strdup("info"), sent_message);
}

void leave(t_server *server, t_client *client, char **splitted_message)
{
    char *sent_message;
    (void)splitted_message;

    if (my_strcmp(client->current_channel->name, server->serv_config->channels_list->first_channel->name) == 0)
    {
        send_special(client, my_strdup("error"), my_strdup("You can't leave the default channel"));
        return;
    }
    move_client(server, client, server->serv_config->channels_list->first_channel);
    sent_message = generate_message(my_strdup("You are back in %s"), 1, client->current_channel->name);
    send_special(client, my_strdup("info"), sent_message);
}

void create(t_server *server, t_client *client, char **splitted_message)
{
    char **splitted_core_message;
    char *sent_message;
    t_channel *target_channel;

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
    target_channel = add_channel(server->serv_config->channels_list, my_strdup(splitted_core_message[0]));
    sent_message = generate_message(my_strdup("You create and join %s"), 1, splitted_core_message[0]);
    server_info(server, generate_message(my_strdup("%s created the channel %s"), 1, client->nickname, splitted_core_message[0]));
    send_special(client, my_strdup("info"), sent_message);
    move_client(server, client, target_channel);
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
        my_strcat(all_channels, "\t- ");
        my_strcat(all_channels, current_channel->name);
        if (my_strcmp(current_channel->name, client->current_channel->name) == 0)
        {
            len += 6;
            all_channels = realloc(all_channels, len);
            my_strcat(all_channels, " <--");
        }
        my_strcat(all_channels, "\n");
        current_channel = current_channel->next;
    }
    send_special(client, my_strdup("info"), all_channels);
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
    server_info(server, server_message);
    free(message);
    free(action);
    free(place);
}
