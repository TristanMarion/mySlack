#ifndef __UTILS_H__
#define __UTILS_H__

#include "includes_server.h"

t_channel *get_channel(t_server *server, char *name);
t_client *get_client(t_server *server, char *name);
t_color get_color(char *color);
void send_special(t_client *client, char *special, char *message);

#endif
