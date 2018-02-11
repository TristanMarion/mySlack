#include "includes_server.h"

t_channels_list *new_channels_list()
{
	t_channels_list *channels;
	t_channel 		*main_channel;

	channels = malloc(sizeof(t_channels_list));
	if (channels == NULL)
	{
		return (NULL);
	}
	main_channel = malloc(sizeof(t_channel));
	if (main_channel == NULL)
	{
		return (NULL);
	}
	main_channel->name = my_strdup("General");
	channels->first_channel = main_channel;
	channels->last_channel = main_channel;
	return (channels);
}
