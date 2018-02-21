#ifndef __SERVER_FUNCTIONS_H__
#define __SERVER_FUNCTIONS_H__

#include "includes_server.h"

void main_loop(t_server *server);
void poll_events(t_server *server, t_client *client);
void disconnect(t_server *server, t_client *client);
void before_manage_message(t_server *server, char *message, int *end);

#endif
