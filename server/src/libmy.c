#include <stdlib.h>
#include <unistd.h>

void my_putchar(const char c)
{
    write(1, &c, 1);
}

int my_strlen(const char *str)
{
    int i;

    i = 0;
    while (str[i] != '\0')
        i++;
    return (i);
}

void my_putstr(const char *str)
{
    write(1, str, my_strlen(str));
}

void my_put_nbr(int n)
{
    if (n < 0)
    {
        n = -n;
        my_putchar('-');
    }
    if (n >= 10)
    {
        my_put_nbr(n / 10);
        my_put_nbr(n % 10);
    }
    else
        my_putchar(n + 48);
}
