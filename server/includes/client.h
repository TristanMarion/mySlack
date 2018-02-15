#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "includes_server.h"

typedef struct s_client
{
    int                 		fd_id;
    uint                		clilen;
    struct sockaddr_in  		cli_addr;
    struct s_client 			*prev;
    struct s_client 			*next;
    char                        nickname[NICKNAME_MAX_LEN];
    char                        *color;
    struct s_channel 			*current_channel;
} t_client;

#endif
