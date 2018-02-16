#include "includes_common.h"

char *generate_message(char *str, int do_free, ...)
{
    va_list args;
    va_list tmp_args;
    int needed;
    char *generated_message;

    va_start(args, do_free);
    va_copy(tmp_args, args);
    needed = vsnprintf(NULL, 0, str, args) + 1;
    generated_message = malloc(needed);
    vsnprintf(generated_message, needed, str, tmp_args);
    if (do_free)
        free(str);
    return (generated_message);
}

char *get_core_message(char *message)
{
	int i;
	int j;
	int k;
	char *message_core;

	i = 0;
	j = 0;
	k = 0;
	if (message[0] == '/')
	{
		while (message[++i] != ' ' && message[i] != '\0')
			;
		j = k = i;
		while (message[j++] != '\0')
			;
		message_core = malloc(sizeof(char) * j);
		i = 0;
		while (message[k++] != '\0')
		{
			message_core[i] = message[k];
			i++;
		}
		return (message_core);
	}
	return (message);
}

char *get_message_command(char *message)
{
	int i;
	int j;
	char *command;

	i = 0;
	j = 0;
	if (message[0] == '/')
	{
		while (message[++i] != ' ' && message[i] != '\0')
			;
		command = malloc(sizeof(char) * i - 1);
		while (j < i - 1)
		{
			command[j] = message[j + 1];
			j++;
		}
		command[j] = '\0';
		return command;
	}
	return my_strdup("send_message");
}
