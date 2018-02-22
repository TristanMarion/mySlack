#include "includes_server.h"

const t_client_command client_command_array[] = {
    {"send_message", send_message, "Sends a message to all connected users in your channel"},
    {"list_commands", list_commands, "Lists all available commands"},
    {"commands_list", list_commands, "Lists all available commands"},
    {"help", help, "Gives informations about a command, or explains how to get the list of available commands"},
    {"direct_message", direct_message, "Sends a direct message to a user"},
    {"list_channels", list_channels, "Lists all available channels"},
    {"join", join, "Joins the specified channel"},
    {"leave", leave, "Leaves the current channel and gets back to the default channel"},
    {"create", create, "Create a channel"},
    {"ping", ping, "Pings the server"},
    {"nickname", nickname, "Changes your nickname"},
    {"important", important, "Sends a message to every connected user"},
    {"color", color, "Changes your messages' color"},
    {"bg_color", bg_color, "Changes your messages' background color"},
    {"list_colors", list_colors, "Lists all available colors for your messages and messages' background"},
    {"reset_color", reset_color, "Reset your messages' color"},
    {"reset_bg_color", reset_bg_color, "Reset your messages' background color"},
    {"logout", logout, "Disconnects you from the server"},
    {NULL, NULL, NULL}};

void list_commands(t_server *server, t_client *client, char **splitted_message)
{
    int i;
    int len;
    t_client_command current_command;
    char *all_commands;
    (void)server;
    (void)splitted_message;

    i = 0;
    all_commands = my_strdup("List of all server commands :\n");
    len = my_strlen(all_commands);
    while ((current_command = client_command_array[i]).command != NULL)
    {
        len += my_strlen(current_command.command) + my_strlen(current_command.description) + 8;
        all_commands = realloc(all_commands, len);
        my_strcat(all_commands, "\t- ");
        my_strcat(all_commands, current_command.command);
        my_strcat(all_commands, " : ");
        my_strcat(all_commands, current_command.description);
        my_strcat(all_commands, "\n");
        i++;
    }
    send_special(client, my_strdup("info"), all_commands);
}

void help(t_server *server, t_client *client, char **splitted_message)
{
    const t_client_command *command;
    char *sent_message;
    char **splitted_core_message;
    (void)server;

    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strcmp(splitted_core_message[0], "") == 0)
    {
        send_special(client, my_strdup("error"), my_strdup("Usage: /help <command>, available commands : /list_commands"));
        return;
    }
    if ((command = get_command(splitted_core_message[0])) != NULL)
    {
        sent_message = generate_message(my_strdup("%s : %s"), 1, command->command, command->description);
        send_special(client, my_strdup("info"), sent_message);
        return;
    }
    sent_message = generate_message(my_strdup("Unknown command %s"), 1, splitted_core_message[0]);
    send_special(client, my_strdup("error"), sent_message);
}

const t_client_command *get_command(char *command)
{
    int i;
    const t_client_command *current_command;

    i = 0;
    while ((current_command = &(client_command_array[i]))->command != NULL)
    {
        if (my_strcmp(current_command->command, command) == 0)
            return current_command;
        i++;
    }
    return NULL;
}
