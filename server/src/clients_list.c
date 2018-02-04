#include "clients_list.h"

t_clients_list *new_clients_list()
{
	t_clients_list *clients;
	clients = malloc(sizeof(t_clients_list));
	if (clients == NULL)
	{
		return (NULL);
	}
	clients->first_client = NULL;
	clients->last_client = NULL;
	clients->nb_clients = 0;
	return (clients);
}
