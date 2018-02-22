#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "includes_server.h"

t_config *get_config(char *path);
t_channels_list *get_channels_list(char *channels);
char *format_field(char *field);

#endif
