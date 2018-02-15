#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "includes_server.h"

typedef struct s_server_command
{
    char *command;
    void (*cmd_ptr)(t_server *server, t_client *client, char **splitted_message);
    char *description;
} t_server_command;

void manage_message(t_server *server, t_client *client, char *message);
void send_message(t_server *server, t_client *client, char **splitted_message);
void list_commands(t_server *server, t_client *client, char **splitted_message);
void direct_message(t_server *server, t_client *client, char **splitted_message);
void send_direct_message(char *nickname, int target, char *message);
void help(t_server *server, t_client *client, char **splitted_message);
void list_channels(t_server *server, t_client *client, char **splitted_message);
void join(t_server *server, t_client *client, char **splitted_message);
void leave(t_server *server, t_client *client, char **splitted_message);
int check_channel_availability(t_server *server, char *name);
void create(t_server *server, t_client *client, char **splitted_message);
void notify_channel(t_server *server, t_client *client, char *action);
void ping(t_server *server, t_client *client, char **splitted_message);
void send_server_message(t_server *server, char *message);
const t_server_command *get_command(char *command);
void send_special(t_client *client, char *special, char *message);
void nickname(t_server *server, t_client *client, char **splitted_message);
void important(t_server *server, t_client *client, char **splitted_message);

#endif
