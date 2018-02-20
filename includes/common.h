#ifndef __COMMON_H_
#define __COMMON_H_

typedef enum style
{
    Italic,
    Bold,
    Underline
} style;

typedef struct s_markdown_entity
{
	style 	style;
	char 	symbol;
	char 	*escape_sequence;
} t_markdown_entity;

char *generate_message(char *str, int do_free, ...);
char *get_core_message(char *message);
char *get_message_command(char *message);
void format_and_apply_markdown(char *str, int begin, int end, t_markdown_entity);
char *poll_markdown_triggers(char *str);
void apply_markdown(char *str, int begin, int end, t_markdown_entity);
void remove_character(char *str, int index);
t_markdown_entity trigger_markdown(char *str, int i);

#endif
