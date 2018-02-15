#ifndef __LIBMY_H__
#define __LIBMY_H__

void my_putchar(const char c);
int my_strlen(const char *str);
void my_putstr(const char *str);
void my_put_nbr(int n);
char *my_strdup(const char *str);
int my_strcmp(const char *s1, const char *s2);
int my_getnbr(char *str);
char *readline(void);
void my_reset(char *, int);
char *my_strstr(char *str, char *to_find);
char **parse_command(char *str, char delimiter);
int get_nb_field(char *str, char delimiter);
char *my_strcat(char *str1, char *str2);
char *my_implode(char **tab, char *str, int offset);
char *my_strcpy(char *dest, char *src);

#endif
