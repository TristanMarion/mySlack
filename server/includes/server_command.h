#ifndef __SERVER_COMMAND_H__
#define __SERVER_COMMAND_H__

#include "includes_server.h"

void server_send_message(t_server *server, t_client *client, char **splitted_message);
void kick(t_server *server, t_client *client, char **splitted_message);
void server_error(char *error);

#endif
