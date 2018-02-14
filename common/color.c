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
                                  {"black", "30"},
                                  {"red", "31"},
                                  {"green", "32"},
                                  {"yellow", "33"},
                                  {"blue", "34"},
                                  {"magenta", "35"},
                                  {"cyan", "36"},
                                  {"white", "37"},
                                  {NULL, NULL}};

static const t_color g_bg_color[] = {{"clear", "\033[H\033[2J"},
                                     {"black", "40"},
                                     {"red", "41"},
                                     {"green", "42"},
                                     {"yellow", "43"},
                                     {"blue", "44"},
                                     {"magenta", "45"},
                                     {"cyan", "46"},
                                     {"white", "47"},
                                     {NULL, NULL}};

void my_putstr_color(const char *color, const char *bg_color, const char *str, int bold, int underline)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (g_color[i].color != NULL &&
           (my_strcmp(g_color[i].color, color) != 0))
        i++;
    while (g_bg_color[j].color != NULL &&
           (my_strcmp(g_bg_color[j].color, bg_color) != 0))
        j++;
    if (g_color[i].color == NULL)
    {
        my_putstr(str);
        return;
    }
    my_putstr("\033[");
    if (bold)
        my_putstr("1;");
    if (underline)
        my_putstr("4;");
    if (g_bg_color[j].color != NULL)
    {
        my_putstr(g_bg_color[j].unicode);
        my_putstr(";");
    }
    my_putstr(g_color[i].unicode);
    my_putstr("m");
    my_putstr(str);
    my_putstr(reset_color);
}
