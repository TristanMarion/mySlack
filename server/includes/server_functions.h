#ifndef __SERVER_FUNCTIONS_H__
#define __SERVER_FUNCTIONS_H__

#include "includes_server.h"

t_server *create_server(char *path);
int init_server(t_server *server);
int new_client(t_server *server);
void add_client_to_list(t_server *server, t_client *client);
void display_clients(t_server *server);
void welcome_message(t_server *server, t_client *client);
void poll_events(t_server *server, t_client *client);
void main_loop(t_server *server);
void remove_client_from_list(t_server *server, t_client *client);
void disconnect(t_server *server, t_client *client);
int check_nickname(t_server *server, t_client *client);
t_config *get_config(char *path);
t_channel *add_channel(t_channels_list *channels_list, char *name);
void remove_channel_from_list(t_server *server, t_channel *channel);
t_channels_list *get_channels_list(char *channels);
t_channel *get_channel(t_server *server, char *name);
void setup_client(t_server *server, t_client *client);
t_client *get_client(t_server *server, char *name);
char *format_field(char *field);
void before_manage_message(t_server *server, char *message, int *end);

#endif
