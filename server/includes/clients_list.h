#ifndef __CLIENTS_LIST_H__
#define __CLIENTS_LIST_H__

#include "includes_server.h"

typedef struct s_clients_list
{
	uint		nb_clients;
	t_client	*first_client;
	t_client	*last_client;
} t_clients_list;

t_clients_list *new_clients_list();

#endif
