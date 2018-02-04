#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "clients_list.h"

typedef struct s_server
{
    int                 sockfd;
    struct sockaddr_in  serv_addr;
    fd_set              fds;
    t_clients_list		*clients;
} t_server;

t_server *create_server(uint);

#endif
