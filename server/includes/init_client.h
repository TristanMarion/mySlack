#ifndef __INIT_CLIENT_H__
#define __INIT_CLIENT_H__

#include "includes_server.h"

int new_client(t_server *server);
void setup_client(t_server *server, t_client *client);
int check_nickname(t_server *server, t_client *client);
void display_clients(t_server *server);

#endif
