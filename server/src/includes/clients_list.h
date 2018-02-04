#ifndef __CLIENTS_LIST_H__
#define __CLIENTS_LIST_H__

#include "client.h"

typedef struct s_clients_list
{
	uint		nb_clients;
	t_client	*first_client;
	t_client	*last_client;
} t_clients_list;

#endif
