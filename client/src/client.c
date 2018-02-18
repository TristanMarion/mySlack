#include "includes_client.h"

void send_message(int fd, char *message)
{
	char *command;
	char *core_message;

	command = get_message_command(message);
	core_message = get_core_message(message);
	if (my_strcmp(core_message, "send_message") == 0 && my_strcmp(core_message, "") == 0)
		return;
	my_send(fd, command, core_message);
}

void send_infos(int sock, char **argv)
{
	my_send(sock, argv[3], argv[4]);
}

void my_send(int fd, char *first_str, char *second_str)
{
	char *sent_message;

	sent_message = generate_message(my_strdup("%s\037%s"), 1, first_str, second_str);
	send(fd, sent_message, my_strlen(sent_message), 0);
	free(sent_message);
}
