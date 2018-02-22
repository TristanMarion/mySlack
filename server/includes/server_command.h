#ifndef __SERVER_COMMAND_H__
#define __SERVER_COMMAND_H__

#include "includes_server.h"

typedef struct s_server_command
{
    char *command;
    void (*cmd_ptr)(t_server *server, int *end, char **splitted_message);
    char *description;
} t_server_command;

void server_manage_message(t_server *server, int *end, char *message);
const t_server_command *get_server_command(char *command);

/* Messages */
void server_send_message(t_server *server, int *end, char **splitted_message);

/* Channels */
void create_channel(t_server *server, int *end, char **splitted_message);
void remove_channel(t_server *server, int *end, char **splitted_message);
void move_all_clients(t_server *server, t_channel *channel);

/* Clients */
void kick(t_server *server, int *end, char **splitted_message);
void mute(t_server *server, int *end, char **splitted_message);
void unmute(t_server *server, int *end, char **splitted_message);

/* Commands */
void list_server_commands(t_server *server, int *end, char **splitted_message);

/* Server */
void stop(t_server *server, int *end, char **splitted_message);

void server_error(char *error);
void server_info(t_server *server, char *info);

#endif
