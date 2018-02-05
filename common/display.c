#include "includes_server.h"

void put_title(char *title)
{
	my_putstr_color("cyan", "\nÂ·.Â¸Â¸.Â·Â·.Â¸Â¸.Â·\t");
	my_putstr_color("cyan", title);
	my_putstr_color("cyan", "\tÂ·.Â¸Â¸.Â·Â·.Â¸Â¸.Â·\n\n");
}

void put_stat(char *label, int value)
{
	my_putstr_color("blue", "\t• ");
	my_putstr_color("blue", label);
	my_putstr_color("blue", " : ");
	my_put_nbr(value);
	my_putchar('\n');
}

void put_success(char* message)
{
	my_putstr_color("green", "✔ ");
	my_putstr_color("green", message);
	my_putchar('\n');
}

void put_error(char* message)
{
	my_putstr_color("red", "✘ ");
	my_putstr_color("red", message);
	my_putchar('\n');
}

void put_info(char* message)
{
	my_putstr_color("blue", "ℹ ");
	my_putstr_color("blue", message);
	my_putchar('\n');
}
