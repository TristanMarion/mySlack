#ifndef __COLOR_H__
#define __COLOR_H__

typedef struct s_color t_color;

struct s_color
{
    char *color;
    char *fg_unicode;
    char *bg_unicode;
};

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

void my_putstr_color(char *color, char *bg_color, char *str, int bold, int underline, int italic);
void print_special(int bold, int underline, int italic);
void increment(int *i, char *color);

#endif
