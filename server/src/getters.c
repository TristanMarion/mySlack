#include "includes_server.h"

t_channel *get_channel(t_server *server, char *name)
{
    t_channel *current_channel;

    current_channel = server->serv_config->channels_list->first_channel;
    while (current_channel != NULL)
    {
        if (my_strcmp(current_channel->name, name) == 0)
        {
            free(name);
            return (current_channel);
        }
        current_channel = current_channel->next;
    }
    free(name);
    return (NULL);
}

t_client *get_client(t_server *server, char *name)
{
    t_client *current_client;

    current_client = server->clients_list->first_client;
    while (current_client != NULL)
    {
        if (my_strcmp(current_client->nickname, name) == 0)
            return (current_client);
        current_client = current_client->next;
    }
    return (NULL);
}
