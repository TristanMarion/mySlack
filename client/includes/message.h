#ifndef __MESSAGE_H__
#define __MESSAGE_H__

typedef struct s_display_function
{
    char *command;
    void (*cmd_ptr)(char *message, int *end);
} t_display_function;

void handle_message(int *end, char *message);
void info(char *message, int *end);
void error(char *message, int *end);
void message(char *message, int *end);
void disconnect(char *message, int *end);
void important(char *message, int *end);
void direct_message(char *message, int *end);
const t_display_function *get_function(char *command);

#endif
