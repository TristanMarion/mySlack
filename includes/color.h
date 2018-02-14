#ifndef __COLOR_H__
#define __COLOR_H__

typedef struct s_color t_color;

struct s_color
{
    char *color;
    char *fg_unicode;
    char *bg_unicode;
};

void my_putstr_color(char *color, char *bg_color, char *str, int bold, int underline);
void print_bold(int bold);
void print_underline(int underline);
void increment(int *i, char *color);

#endif
