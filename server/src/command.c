#include "includes_server.h"

void manage_message(t_server *server, t_client *client, char *message)
{
    char **splitted_message;
    const t_client_command *command;
    char *response;

    splitted_message = parse_command(message, '\037');
    if ((command = get_command(splitted_message[0])) != NULL)
    {
        command->cmd_ptr(server, client, splitted_message);
        return;
    }
    response = my_strdup("Unknown command. Type /list_commands to show available commands.");
    send_special(client, my_strdup("error"), response);
}

void nickname(t_server *server, t_client *client, char **splitted_message)
{
    char *sent_message;
    char **splitted_core_message;

    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strcmp(splitted_core_message[0], "") == 0)
    {
        send_special(client, my_strdup("error"), my_strdup("Usage : /nickname <nickname>"));
        return;
    }
    if (get_client(server, splitted_core_message[0]) != NULL)
    {
        send_special(client, my_strdup("error"), my_strdup("This nickname is already taken"));
        return;
    }
    my_strcpy(client->nickname, splitted_core_message[0]);
    sent_message = generate_message(my_strdup("You changed your nickname to %s"), 1, client->nickname);
    send_special(client, my_strdup("info"), sent_message);
}

void logout(t_server *server, t_client *client, char **splitted_message)
{
    (void)splitted_message;

    send_special(client, my_strdup("disconnect"), my_strdup("You have been disconnected, bye !"));
    disconnect(server, client);
}
