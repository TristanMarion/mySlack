#include "includes_client.h"

void send_message(int fd, char *message)
{
	char *command;
	char *core_message;
	
	command = get_command(message);
	core_message = get_core_message(message);
	my_send(fd, command, core_message);
}

void send_infos(int sock, char **argv)
{
	my_send(sock, argv[3], argv[4]);
}

void my_send(int fd, char *first_str, char *second_str)
{
	char *sent_message;

	sent_message = generate_message(my_strdup("%s;%s"), 1, first_str, second_str);
	send(fd, sent_message, my_strlen(sent_message), 0);
	free(sent_message);
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

char *get_command(char *message)
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
