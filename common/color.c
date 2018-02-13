#include <stdlib.h>
#include "includes_common.h"

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
                                  {"bold_red", "\033[1;31m"},
                                  {"bold_green", "\033[1;32m"},
                                  {"bold_yellow", "\033[1;33m"},
                                  {"bold_blue", "\033[1;34m"},
                                  {"bold_magenta", "\033[1;35m"},
                                  {"bold_cyan", "\033[1;36m"},
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
