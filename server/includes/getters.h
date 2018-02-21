#ifndef __GETTERS_H__
#define __GETTERS_H__

#include "includes_server.h"

t_channel *get_channel(t_server *server, char *name);
t_client *get_client(t_server *server, char *name);
t_color get_color(char *color);

#endif
