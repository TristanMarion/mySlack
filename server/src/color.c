#include <stdlib.h>
#include "includes.h"

const char *reset_color = "\033[0m";

typedef struct s_color t_color;

struct s_color
{
    char *color;
    char *unicode;
};

static const t_color g_color[] = {{"clear", "\033[H\033[2J"},
                                  {"red", "\033[31m"},
                                  {"green", "\033[32m"},
                                  {"yellow", "\033[33m"},
                                  {"blue", "\033[34m"},
                                  {"magenta", "\033[35m"},
                                  {"cyan", "\033[36m"},
                                  {NULL, NULL}};

void my_putstr_color(const char *color, const char *str)
{
    int i;

    i = 0;
    while (g_color[i].color != NULL &&
           (my_strcmp(g_color[i].color, color) != 0))
        i++;
    if (g_color[i].color == NULL)
    {
        my_putstr(str);
        return;
    }
    my_putstr(g_color[i].unicode);
    my_putstr(str);
    my_putstr(reset_color);
}
