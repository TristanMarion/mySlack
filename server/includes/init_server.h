#ifndef __INIT_SERVER_H__
#define __INIT_SERVER_H__

#include "includes_server.h"

t_server *create_server(char *path);
int init_server(t_server *server);
t_clients_list *new_clients_list();

#endif
