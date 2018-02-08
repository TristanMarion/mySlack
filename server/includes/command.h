#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "includes_server.h"

typedef struct s_server_command
{
    char *command;
    void (*cmd_ptr)(t_server *server, t_client *client, char **splitted_message);
} t_server_command;

void manage_message(t_server *server, t_client *client, char *message);

void send_message(t_server *server, t_client *client, char **splitted_message);

void list_commands(t_server *server, t_client *client, char **splitted_message);

#endif
