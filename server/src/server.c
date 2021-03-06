#include "includes_server.h"

void main_loop(t_server *server)
{
    int max;
    char message[MAX_LEN];
    int end;

    end = 0;
    while (end == 0)
    {
        setup_loop(server, &max);
        if (select(max + 1, &(server->fds), NULL, NULL, NULL) < 0)
        {
            put_error("select()");
            return;
        }
        if (FD_ISSET(server->sockfd, &(server->fds)))
        {
            if (new_client(server) != 0)
            {
                put_error("new client");
                return;
            }
        }

        if (FD_ISSET(0, &(server->fds)))
            read_standard_input(server, message, &end);
        clients_loop(server);
    }
}

void setup_loop(t_server *server, int *max)
{
    t_client *current_client;

    FD_ZERO(&(server->fds));
    FD_SET(0, &(server->fds));
    FD_SET(server->sockfd, &(server->fds));
    *max = server->sockfd;
    current_client = server->clients_list->first_client;
    while (current_client != NULL)
    {
        FD_SET(current_client->fd_id, &(server->fds));
        *max = current_client->fd_id > *max ? current_client->fd_id : *max;
        current_client = current_client->next;
    }
}

void read_standard_input(t_server *server, char *message, int *end)
{
    read(0, message, MAX_LEN - 1);
    {
        char *p = NULL;
        p = my_strstr(message, "\n");
        if (p != NULL)
        {
            *p = 0;
        }
        else
        {
            message[MAX_LEN - 2] = '\n';
            message[MAX_LEN - 1] = 0;
        }
    }
    before_manage_message(server, message, end);
}

void clients_loop(t_server *server)
{
    t_client *current_client;
    
    current_client = server->clients_list->first_client;
    while (current_client != NULL)
    {
        if (FD_ISSET(current_client->fd_id, &(server->fds)))
        {
            poll_events(server, current_client);
        }
        current_client = current_client->next;
    }
}

void poll_events(t_server *server, t_client *client)
{
    int read_size;
    char read[MAX_LEN];

    if ((read_size = recv(client->fd_id, read, MAX_LEN - 1, 0)) > 0)
    {
        read[read_size] = 0;
        manage_message(server, client, read);
    }
    else
    {
        disconnect(server, client);
    }
}

void disconnect(t_server *server, t_client *client)
{
    move_client(server, client, NULL);
    remove_client_from_list(server, client);
}

void before_manage_message(t_server *server, char *message, int *end)
{
    char *command;
    char *core_message;
    char *sent_message;

    command = get_message_command(message);
    core_message = get_core_message(message);
    sent_message = generate_message(my_strdup("%s\037%s"), 1, command, core_message);
    server_manage_message(server, end, sent_message);
    free(sent_message);
}

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
