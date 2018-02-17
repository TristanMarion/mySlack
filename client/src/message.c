#include "includes_client.h"

const t_display_function display_function_array[] = {
    {"info", info},
    {"error", error},
    {"disconnect", disconnect},
    {"message", message},
    {"direct_message", direct_message},
    {"important", important},
    {"server", server},
    {NULL, NULL}};

void handle_message(int *end, char *message)
{
    char **parsed_message;
    const t_display_function *function;
    (void)end;

    parsed_message = parse_command(message, '\037');
    if ((function = get_function(parsed_message[0])) != NULL)
    {
        function->cmd_ptr(my_implode(parsed_message, "\037", 1), end);
        return;
    }
}

void info(char *message, int *end)
{
    (void)end;

    put_info(parse_command(message, '\037')[0]);
}

void error(char *message, int *end)
{
    (void)end;

    put_error(parse_command(message, '\037')[0]);
}

void disconnect(char *message, int *end)
{
    *end = 1;
    my_putstr_color("red", "", parse_command(message, '\037')[0], 1, 1, 0);
    my_putstr("\n");
}

void important(char *message, int *end)
{
    char **parsed_message;
    (void)end;

    parsed_message = parse_command(message, '\037');    
    my_putstr("[");
    my_putstr_color("red", "", "IMPORTANT", 1, 0, 0);
    my_putstr("] ");
    my_putstr(parsed_message[0]);
    my_putstr(" - ");
    my_putstr(parsed_message[1]);
    my_putstr("\n");
}

void message(char *message, int *end)
{
    char **parsed_message;
    (void)end;

    parsed_message = parse_command(message, '\037');
    my_putstr_color("", "", "[", 0, 0, 0);
    my_putstr_color("", "", parsed_message[2], 0, 0, 0);
    my_putstr_color("", "", "] ", 0, 0, 0);
    my_putstr_color("", "", parsed_message[3], 0, 0, 0);
    my_putstr_color("", "", " : ", 0, 0, 0);
    my_putstr_color(parsed_message[0], parsed_message[1], parsed_message[4], 0, 0, 0);
    my_putstr("\n");
}

void direct_message(char *message, int *end)
{
    char **parsed_message;
    (void)end;

    parsed_message = parse_command(message, '\037');
    my_putstr_color("", "", parsed_message[0], 0, 0, 0);
    my_putstr_color("", "", " - ", 0, 0, 0);
    my_putstr_color("", "", parsed_message[1], 0, 0, 1);
    my_putstr("\n");
}

void server(char *message, int *end)
{
    (void)end;

    my_putstr_color("yellow", "", "! ", 1, 0, 0);
    my_putstr_color("yellow", "", parse_command(message, '\037')[0], 1, 1, 1);
    my_putstr("\n");
}

const t_display_function *get_function(char *command)
{
    int i;
    const t_display_function *current_function;

    i = 0;
    while ((current_function = &(display_function_array[i]))->command != NULL)
    {
        if (my_strcmp(current_function->command, command) == 0)
            return current_function;
        i++;
    }
    return NULL;
}
