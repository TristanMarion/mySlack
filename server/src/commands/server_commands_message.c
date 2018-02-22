#include "includes_server.h"

void server_send_message(t_server *server, int *end, char **splitted_message)
{
    char *sent_message;
    t_client *current_client;
    (void)end;

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
