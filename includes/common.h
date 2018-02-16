#ifndef __COMMON_H_
#define __COMMON_H_

char *generate_message(char *str, int do_free, ...);
char *get_core_message(char *message);
char *get_message_command(char *message);

#endif
