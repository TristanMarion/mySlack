#include "includes_server.h"

void send_special(t_client *client, char *special, char *message)
{
    char *sent_message;

    sent_message = generate_message(my_strdup("%s;%s"), 1, special, message);
    send(client->fd_id, sent_message, my_strlen(sent_message), 0);
    free(special);
    free(message);
    free(sent_message);
}
