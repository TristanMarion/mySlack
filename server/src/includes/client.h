#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <netinet/in.h>

typedef struct s_client
{
    int                 fd_id;
    uint                clilen;
    struct sockaddr_in  cli_addr;
} t_client;

#endif
