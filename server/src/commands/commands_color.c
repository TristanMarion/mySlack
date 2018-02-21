#include "includes_server.h"

void color(t_server *server, t_client *client, char **splitted_message)
{
    (void)server;

    change_color(client, splitted_message[0], splitted_message[1]);
}

void bg_color(t_server *server, t_client *client, char **splitted_message)
{
    (void)server;

    change_color(client, splitted_message[0], splitted_message[1]);
}

void change_color(t_client *client, char *mode, char *message)
{
    char **splitted_core_message;
    char *sent_message;
    t_color color;
    char *to_change;

    splitted_core_message = parse_command(message, ' ');
    if (my_strcmp(splitted_core_message[0], "") == 0)
    {
        sent_message = generate_message(my_strdup("Usage : /%s <color>"), 1, mode);
        send_special(client, my_strdup("error"), sent_message);
        return;
    }
    if ((color = get_color(splitted_core_message[0])).color == NULL)
    {
        send_special(client, my_strdup("error"), my_strdup("This color doest'n exists. To check colors list use `/list_colors`"));
        return;
    }
    else if (my_strcmp(color.color, "clear") == 0)
    {
        send_special(client, my_strdup("error"), my_strdup("Forbidden color. To check colors list use `/list_colors`"));
        return;
    }
    to_change = my_strcmp(mode, "color") == 0 ? client->color : client->bg_color;
    free(to_change);
    to_change = my_strdup(splitted_core_message[0]);
    sent_message = generate_message(my_strdup("You changed your messages' %s to %s"), 1, mode, to_change);
    send_special(client, my_strdup("info"), sent_message);
}

void reset_color(t_server *server, t_client *client, char **splitted_message)
{
    (void)server;

    reset_colors(server, client, splitted_message[0]);
}

void reset_bg_color(t_server *server, t_client *client, char **splitted_message)
{
    (void)server;

    reset_colors(server, client, splitted_message[0]);
}

void reset_colors(t_server *server, t_client *client, char *mode)
{
    t_config *config;
    int i_mode;
    char *sent_message;
    char *to_change;

    config = server->serv_config;
    i_mode = my_strcmp(mode, "reset_color") ? 1 : 0;
    to_change = i_mode == 0 ? client->color : client->bg_color;
    free(to_change);
    to_change = my_strdup(i_mode == 0 ? config->default_color : config->default_bg_color);
    sent_message = generate_message(my_strdup("You reseted your messages' %s to %s"), 1, mode, to_change);
    send_special(client, my_strdup("info"), sent_message);
}

void list_colors(t_server *server, t_client *client, char **splitted_message)
{
    int i;
    int len;
    t_color current_color;
    char *all_colors;
    (void)server;
    (void)splitted_message;

    i = 1; /* Skip the `clear` */
    all_colors = my_strdup("List of all server commands :\n");
    len = my_strlen(all_colors);
    while ((current_color = g_color[i]).color != NULL)
    {
        len += my_strlen(current_color.color) + 5;
        all_colors = realloc(all_colors, len);
        my_strcat(all_colors, "\t- ");
        my_strcat(all_colors, current_color.color);
        my_strcat(all_colors, "\n");
        i++;
    }
    send_special(client, my_strdup("info"), all_colors);
}
