#ifndef __CLIENT_H__
#define __CLIENT_H__

void send_message(int, char *);
char *get_command(char*);
char *get_core_message(char *message);
void send_infos(int sock, char **argv);
void my_send(int fd, char *first_str, char *second_str);

#endif
