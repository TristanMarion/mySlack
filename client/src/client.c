#include "includes_client.h"

void send_message(int fd, char *message)
{
	int needed;
	char *sent_message;
	char *command = get_command(message);
	message = get_core_message(message);

	needed = snprintf(NULL, 0, "%s;%s", command, message) + 1;
	sent_message = malloc(needed);
	snprintf(sent_message, needed, "%s;%s", command, message);
	send(fd, sent_message, my_strlen(sent_message), 0);
}

void send_infos(int sock, char **argv)
{
	int needed;
	char *sent_message;

	(void)sock;

	needed = snprintf(NULL, 0, "%s;%s", argv[3], argv[4]) + 1;
	sent_message = malloc(needed);
	snprintf(sent_message, needed, "%s;%s", argv[3], argv[4]);
	send(sock, sent_message, my_strlen(sent_message), 0);
}

char *get_core_message(char *message)
{
	int i;
	int j;
	int k;

	i = 0;
	j = 0;
	k = 0;
	char *message_core;
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

	i = 0;
	j = 0;
	char *command;
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
