#include "includes_server.h"

t_config *get_config(char *path)
{
    t_config *config;
    FILE *file;
    char buffer[255];
    char **tab;

    config = malloc(sizeof(t_config));
    config->port = 12345;
    config->max_clients = 4;
    config->channels_list = NULL;
    config->welcome_message = my_strdup("Bienvenue !");
    config->default_color = my_strdup("white");
    config->default_bg_color = my_strdup("");
    if ((file = fopen(path, "r")))
    {
        while (fgets(buffer, 255, file) != NULL)
        {
            tab = parse_command(buffer, ':');
            if (tab[0] != NULL && tab[1] != NULL)
            {
                if (my_strcmp("port", tab[0]) == 0)
                    config->port = my_getnbr(tab[1]);
                if (my_strcmp("max_clients", tab[0]) == 0)
                    config->max_clients = my_getnbr(tab[1]);
                if (my_strcmp("channels", tab[0]) == 0)
                    config->channels_list = get_channels_list(tab[1]);
                if (my_strcmp("welcome", tab[0]) == 0)
                {
                    free(config->welcome_message);
                    config->welcome_message = my_strdup(tab[1]);
                }
                if (my_strcmp("color", tab[0]) == 0)
                {
                    free(config->default_color);
                    config->default_color = my_strdup(format_field(tab[1]));
                }
                if (my_strcmp("bg_color", tab[0]) == 0)
                {
                    free(config->default_bg_color);
                    config->default_bg_color = my_strdup(format_field(tab[1]));
                }
            }
        }
        fclose(file);
    }
    if (config->channels_list == NULL)
    {
        config->channels_list = malloc(sizeof(t_channels_list));
        config->channels_list->first_channel = NULL;
        config->channels_list->last_channel = NULL;
        add_channel(config->channels_list, "General");
    }
    return config;
}

char *format_field(char *field)
{
    if (field[my_strlen(field) - 1] == '\n')
        field[my_strlen(field) - 1] = 0;
    return (field);
}

t_channels_list *get_channels_list(char *channels)
{
    t_channels_list *channels_list;
    int i;
    char **each_channel;

    i = 0;
    channels_list = malloc(sizeof(channels_list));
    channels_list->first_channel = NULL;
    channels_list->last_channel = NULL;
    if (channels[my_strlen(channels) - 1] == '\n')
        channels[my_strlen(channels) - 1] = 0;
    each_channel = parse_command(channels, ',');
    while (each_channel[i])
    {
        add_channel(channels_list, each_channel[i]);
        i++;
    }
    return (channels_list);
}
