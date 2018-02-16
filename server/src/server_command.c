#include "includes_server.h"

void server_send_message(t_server *server, t_client *client, char **splitted_message)
{
    char *sent_message;
    t_client *current_client;
    (void)client;

    sent_message = generate_message(my_strdup("server;%s"), 1, splitted_message[1]);
    current_client = server->clients_list->first_client;
    while (current_client != NULL)
    {
        send(current_client->fd_id, sent_message, my_strlen(sent_message), 0);
        current_client = current_client->next;
    }
    free(sent_message);
}

void server_error(char *error)
{
    put_error(error);
    free(error);
}