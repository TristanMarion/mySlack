#include "includes_client.h"

void send_message(int fd, char *message)
{
    int needed;
    char *sent_message;

    needed = snprintf(NULL, 0, "send_message;%s", message) + 1;
    sent_message = malloc(needed);
    snprintf(sent_message, needed, "send_message;%s", message);
    send(fd, sent_message, my_strlen(sent_message), 0);
}
