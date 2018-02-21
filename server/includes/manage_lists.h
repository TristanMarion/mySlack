#ifndef __MANAGE_LISTS_H__
#define __MANAGE_LISTS_H__

#include "includes_server.h"

void add_client_to_list(t_server *server, t_client *client);
void remove_client_from_list(t_server *server, t_client *client);
t_channel *add_channel(t_channels_list *channels_list, char *name);
void remove_channel_from_list(t_server *server, t_channel *channel);

#endif
