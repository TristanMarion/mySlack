#include "includes_common.h"

void put_success(char *message)
{
	my_putstr_color("green", "", "✔ ", 0, 0);
	my_putstr_color("green", "", message, 0, 0);
	my_putchar('\n');
}

void put_error(char *message)
{
	my_putstr_color("red", "", "✘ ", 0, 0);
	my_putstr_color("red", "", message, 0, 0);
	my_putchar('\n');
}

void put_info(char *message)
{
	my_putstr_color("blue", "", "ℹ ", 0, 0);
	my_putstr_color("blue", "", message, 0, 0);
	my_putchar('\n');
}
