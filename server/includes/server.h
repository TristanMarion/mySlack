#ifndef __SERVER_H__
#define __SERVER_H__

#include "includes_server.h"

typedef struct s_server
{
    int                 sockfd;
    struct sockaddr_in  serv_addr;
    fd_set              fds;
    t_clients_list		*clients_list;
    char 				client_message[2000];
} t_server;

t_server *create_server(uint);
void init_server(t_server *server);
int new_client(t_server *server);
void add_client_to_list(t_server *server, t_client *client);
void display_clients(t_server *server);
void welcome_message(t_client *client);
void notify_new_client(t_server *server, t_client *client);
void poll_incoming_messages(t_server *server);

#endif
