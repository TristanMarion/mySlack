#include "includes_server.h"

int new_client(t_server *server)
{
    t_client *client;
    char *error_message;

    if ((client = malloc(sizeof(t_client))) == NULL)
    {
        put_error("client error");
        return 1;
    }
    client->clilen = sizeof(client->cli_addr);
    client->fd_id = accept(server->sockfd, (struct sockaddr *)&(client->cli_addr), &(client->clilen));
    if (client->fd_id < 0)
        put_error("cannot accept");
    else
    {
        setup_client(server, client);
        if (server->serv_config->max_clients <= server->clients_list->nb_clients)
        {
            error_message = my_strdup("Sorry the server is full. Please try again later.");
            send_special(client, my_strdup("disconnect"), error_message);
            return 0;
        }
        if (!check_nickname(server, client))
            return 0;
        add_client_to_list(server, client);
        display_clients(server);
    }
    return 0;
}

void setup_client(t_server *server, t_client *client)
{
    char received_infos[NICKNAME_MAX_LEN];
    char **client_infos;
    t_channel *target_channel;

    recv(client->fd_id, received_infos, NICKNAME_MAX_LEN, 0);
    client_infos = parse_command(received_infos, '\037');
    my_strcpy(client->nickname, client_infos[0]);
    client->color = my_strdup(server->serv_config->default_color);
    client->bg_color = my_strdup(server->serv_config->default_bg_color);
    client->muted = 0;
    target_channel = get_channel(server, client_infos[1]);
    if (target_channel == NULL)
        target_channel = server->serv_config->channels_list->first_channel;
    move_client(server, client, target_channel);
}

int check_nickname(t_server *server, t_client *client)
{
    t_client *same_nickname_client;
    char *error_message;

    error_message = my_strdup("Sorry, this username is already used. Please try another one.");
    same_nickname_client = get_client(server, client->nickname);
    if (same_nickname_client != NULL)
        send_special(client, my_strdup("disconnect"), error_message);
    return (same_nickname_client == NULL);
}
