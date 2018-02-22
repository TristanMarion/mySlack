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

#endif
