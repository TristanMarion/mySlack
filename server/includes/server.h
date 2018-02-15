#ifndef __SERVER_H__
#define __SERVER_H__

#include "includes_server.h"
#include "channel.h"

typedef struct s_config
{
	uint 				port;
	uint 				max_clients;
	t_channels_list		*channels_list;
	char 				*welcome_message;
    char                *default_color;
    char                *default_bg_color;
} t_config;

typedef struct s_server
{
    int                 sockfd;
    struct sockaddr_in  serv_addr;
    struct s_config		*serv_config;
    fd_set              fds;
    t_clients_list		*clients_list;
} t_server;

t_server *create_server(char *path);
int init_server(t_server *server);
int new_client(t_server *server);
void add_client_to_list(t_server *server, t_client *client);
void display_clients(t_server *server);
void welcome_message(t_server *server, t_client *client);
void notify_new_client(t_server *server, t_client *client);
void poll_events(t_server *server, t_client *client);
void main_loop(t_server *server);
void remove_client_from_list(t_server *server, t_client *client);
void disconnect(t_server *server, t_client *client);
int check_nickname(t_server *server, t_client *client);
t_config *get_config(char *path);
void add_channel(t_channels_list *channels_list, char *name);
t_channels_list *get_channels_list(char *channels);
t_channel *get_channel(t_server *server, char *name);
void setup_client(t_server *server, t_client *client);
t_client *get_client(t_server *server, char *name);

#endif
