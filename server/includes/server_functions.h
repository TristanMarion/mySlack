#ifndef __SERVER_FUNCTIONS_H__
#define __SERVER_FUNCTIONS_H__

#include "includes_server.h"

void display_clients(t_server *server);
void welcome_message(t_server *server, t_client *client);
void poll_events(t_server *server, t_client *client);
void main_loop(t_server *server);
void disconnect(t_server *server, t_client *client);
t_config *get_config(char *path);
t_channels_list *get_channels_list(char *channels);
t_channel *get_channel(t_server *server, char *name);
t_client *get_client(t_server *server, char *name);
char *format_field(char *field);
void before_manage_message(t_server *server, char *message, int *end);

#endif
