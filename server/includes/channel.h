#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include "includes_server.h"

typedef struct s_channel
{
	char 				*name;
	struct s_channel 	*next;
	struct s_channel 	*prev;
} t_channel;

typedef struct s_channels_list
{
	t_channel 	*first_channel;
	t_channel	*last_channel;
} t_channels_list;

#endif
