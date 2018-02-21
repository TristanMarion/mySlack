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

