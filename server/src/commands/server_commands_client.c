#include "includes_server.h"

void kick(t_server *server, int *end, char **splitted_message)
{
    t_client *kicked_client;
    char *message;
    char **splitted_core_message;
    (void)end;

    message = NULL;
    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strlen(splitted_core_message[0]) == 0)
        message = my_strdup("Usage : /kick <nickname>");
    else if ((kicked_client = get_client(server, splitted_core_message[0])) == NULL)
        message = my_strdup("User not found");
    if (message != NULL)
    {
        server_error(message);
        return;
    }
    if (splitted_core_message[1] != NULL)
        message = generate_message(my_strdup("You have been kicked from the server. Reason : %s"),
                                   1, my_implode(splitted_core_message, " ", 1));
    else
        message = my_strdup("You have been kicked from the server.");
    send_special(kicked_client, my_strdup("disconnect"), message);
    remove_client_from_list(server, kicked_client);
}

void mute(t_server *server, int *end, char **splitted_message)
{
    t_client *muted_client;
    char *message;
    char **splitted_core_message;
    (void)end;

    message = NULL;
    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strlen(splitted_core_message[0]) == 0)
        message = my_strdup("Usage : /mute <nickname> [reason]");
    else if ((muted_client = get_client(server, splitted_core_message[0])) == NULL)
        message = my_strdup("User not found");
    else if (muted_client->muted == 1)
        message = my_strdup("User is already muted");
    if (message != NULL)
    {
        server_error(message);
        return;
    }
    if (splitted_core_message[1] != NULL)
        message = generate_message(my_strdup("You have been muted. Reason : %s"), 1, my_implode(splitted_core_message, " ", 1));
    else
        message = my_strdup("You have been muted.");
    send_special(muted_client, my_strdup("info"), message);
    put_info(generate_message(my_strdup("%s has been muted."), 1, muted_client->nickname));
    muted_client->muted = 1;
}

void unmute(t_server *server, int *end, char **splitted_message)
{
    t_client *unmuted_client;
    char *message;
    char **splitted_core_message;
    (void)end;

    message = NULL;
    splitted_core_message = parse_command(splitted_message[1], ' ');
    if (my_strlen(splitted_core_message[0]) == 0)
        message = my_strdup("Usage : /unmute <nickname>");
    else if ((unmuted_client = get_client(server, splitted_core_message[0])) == NULL)
        message = my_strdup("User not found");
    else if (unmuted_client->muted == 0)
        message = my_strdup("User is not muted");
    if (message != NULL)
    {
        server_error(message);
        return;
    }
    send_special(unmuted_client, my_strdup("info"), my_strdup("You have been unmuted"));
    put_info(generate_message(my_strdup("%s has been unmuted."), 1, unmuted_client->nickname));
    unmuted_client->muted = 0;
}
