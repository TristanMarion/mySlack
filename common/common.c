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

const t_markdown_entity markdown_entities_array[] = {
    {Bold, '*', "\033[1m", "\033[22m"},
    {Underline, '_', "\033[4m", "\033[24m"},
    {-1, -1, "", ""}};


t_markdown_entity trigger_markdown(char *str, int index)
{
    t_markdown_entity current_markdown;
    int i;

    i = 0;
    while ((current_markdown = markdown_entities_array[i]).symbol != -1)
    {
        if (str[index] != current_markdown.symbol && str[index - 1] == current_markdown.symbol)
            return current_markdown;
        i++;
    }
    return current_markdown;
}

char *poll_markdown_triggers(char *str)
{
    int i;
    int j;
    t_markdown_entity trigger;

    i = 0;
    while (str[i] != '\0')
    {
        if ((trigger = trigger_markdown(str, i)).symbol != -1)
        {
            j = i;
            while (str[j] != '\0' && str[j + 1] != trigger.symbol)
            {
                j++;
            }
            format_and_apply_markdown(str, i - 1, j + 1, trigger);
            j = 0;
        }
        i++;
    }
    return str;
}

void format_and_apply_markdown(char *str, int begin, int end, t_markdown_entity entity)
{
	if (end == my_strlen(str) + 1)
		return;
    if (str[begin - 1] == entity.symbol && str[end + 1] == entity.symbol)
    {
        remove_character(str, begin - 1);
        remove_character(str, begin - 1);
        remove_character(str, end - 2);
        remove_character(str, end - 2);
        apply_markdown(str, begin - 1, end - 1, entity);
    }
    else 
    {
        entity.style = Italic;
        entity.escape_sequence = "\033[3m";
        entity.reset_sequence = "\033[23m";
        remove_character(str, begin);
        remove_character(str, end - 1);
        apply_markdown(str, begin, end, entity);
    }
}

void remove_character(char *str, int index)
{
    int i;
    int j;

    j = 0;
    for (i = 0 ; i < my_strlen(str) ; i++)
    {
        if (i == index)
            j++;
        str[i] = str[j];
        j++;
    }
    str[i] = 0;
}

void apply_markdown(char *str, int begin, int end, t_markdown_entity entity)
{
    char *new_message;
    int i;
    int j;
    int k;

    i = 0;
    j = 0;
    k = 0;
    new_message = malloc(sizeof(char) * my_strlen(str) + 9);
    for (i = 0 ; i < begin ; i++)
        new_message[i] = str[i];
    my_strcat(new_message, entity.escape_sequence);
    for (j = begin ; j < end - 1 ; j++)
        new_message[j + 4] = str[j];
    my_strcat(new_message, entity.reset_sequence);
    for (k = end - 1 ; k < my_strlen(str) ; k++)
    {
        new_message[k + 9] = str[k];
    }
    new_message[my_strlen(str) + 9] = 0;
    my_strcpy(str, new_message);
}
