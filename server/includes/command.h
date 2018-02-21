#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "includes_server.h"

typedef struct s_client_command
{
    char *command;
    void (*cmd_ptr)(t_server *server, t_client *client, char **splitted_message);
    char *description;
} t_client_command;

void manage_message(t_server *server, t_client *client, char *message);
void nickname(t_server *server, t_client *client, char **splitted_message);
void logout(t_server *server, t_client *client, char **splitted_message);

/* Channels */
void join(t_server *server, t_client *client, char **splitted_message);
void leave(t_server *server, t_client *client, char **splitted_message);
void create(t_server *server, t_client *client, char **splitted_message);
int check_channel_availability(t_server *server, char *name);
void list_channels(t_server *server, t_client *client, char **splitted_message);
void move_client(t_server *server, t_client *client, t_channel *target_channel);

/* Colors */
void color(t_server *server, t_client *client, char **splitted_message);
void bg_color(t_server *server, t_client *client, char **splitted_message);
void change_color(t_client *client, char *mode, char *message);
void list_colors(t_server *server, t_client *client, char **splitted_message);
void reset_colors(t_server *server, t_client *client, char *mode);
void reset_bg_color(t_server *server, t_client *client, char **splitted_message);
void reset_color(t_server *server, t_client *client, char **splitted_message);

/* Commands */
void help(t_server *server, t_client *client, char **splitted_message);
void list_commands(t_server *server, t_client *client, char **splitted_message);
const t_client_command *get_command(char *command);

/* Messages */
void send_message(t_server *server, t_client *client, char **splitted_message);
void direct_message(t_server *server, t_client *client, char **splitted_message);
void send_direct_message(char *nickname, int target, char *message);
void important(t_server *server, t_client *client, char **splitted_message);
void ping(t_server *server, t_client *client, char **splitted_message);
void notify(t_server *server, t_client *client, char *action, int check_channel);

#endif
