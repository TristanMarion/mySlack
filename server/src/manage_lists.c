#include "includes_server.h"

void add_client_to_list(t_server *server, t_client *client)
{
    welcome_message(server, client);
    client->prev = NULL;
    client->next = NULL;
    if (server->clients_list->last_client == NULL)
    {
        server->clients_list->first_client = client;
    }
    else
    {
        client->prev = server->clients_list->last_client;
        server->clients_list->last_client->next = client;
    }
    server->clients_list->last_client = client;
    server->clients_list->nb_clients++;
}

void remove_client_from_list(t_server *server, t_client *client)
{
    t_clients_list *clients_list;

    clients_list = server->clients_list;
    if (clients_list->first_client == NULL || client == NULL)
        return;
    if (clients_list->first_client == client)
        clients_list->first_client = client->next;
    if (clients_list->last_client == client)
        clients_list->last_client = client->prev;
    if (client->next != NULL)
        client->next->prev = client->prev;
    if (client->prev != NULL)
        client->prev->next = client->next;
    close(client->fd_id);
    clients_list->nb_clients--;
}

t_channel *add_channel(t_channels_list *channels_list, char *name)
{
    t_channel *channel;

    channel = malloc(sizeof(t_channel));
    channel->name = my_strdup(name);
    channel->prev = NULL;
    channel->next = NULL;
    if (channels_list->last_channel == NULL)
    {
        channels_list->first_channel = channel;
    }
    else
    {
        channel->prev = channels_list->last_channel;
        channels_list->last_channel->next = channel;
    }
    channels_list->last_channel = channel;
    return (channel);
}

void remove_channel_from_list(t_server *server, t_channel *channel)
{
    t_channels_list *channels_list;

    channels_list = server->serv_config->channels_list;
    if (channels_list->first_channel == NULL || channel == NULL)
        return;
    if (channels_list->first_channel == channel)
        channels_list->first_channel = channel->next;
    if (channels_list->last_channel == channel)
        channels_list->last_channel = channel->prev;
    if (channel->next != NULL)
        channel->next->prev = channel->prev;
    if (channel->prev != NULL)
        channel->prev->next = channel->next;
}
