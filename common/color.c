#include "includes_common.h"

const char *reset_color = "\033[0m";

static const t_color g_color[] = {{"clear", "\033[H\033[2J", ""},
                                  {"black", "30", "40"},
                                  {"red", "31", "41"},
                                  {"green", "32", "42"},
                                  {"yellow", "33", "43"},
                                  {"blue", "34", "44"},
                                  {"magenta", "35", "45"},
                                  {"cyan", "36", "46"},
                                  {"white", "37", "47"},
                                  {NULL, NULL, NULL}};

void my_putstr_color(char *color, char *bg_color, char *str, int bold, int underline, int italic)
{
    int i;
    int j;

    i = 0;
    j = 0;
    increment(&i, color);
    increment(&j, bg_color);
    my_putstr("\033[");
    print_special(bold, underline, italic);
    if (g_color[j].color != NULL)
    {
        my_putstr(g_color[j].bg_unicode);
        my_putstr(";");
    }
    if (g_color[i].color == NULL)
        my_putstr(g_color[8].fg_unicode);
    else
        my_putstr(g_color[i].fg_unicode);
    my_putstr("m");
    my_putstr(str);
    my_putstr(reset_color);
}

void print_special(int bold, int underline, int italic)
{
    if (bold)
        my_putstr("1;");
    if (underline)
        my_putstr("4;");
    if (italic)
        my_putstr("3;");
}

void increment(int *i, char *color)
{
    while (g_color[*i].color != NULL &&
           (my_strcmp(g_color[*i].color, color) != 0))
        (*i)++;
}
