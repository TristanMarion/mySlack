#include "includes_common.h"

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

char *my_strdup(const char *str)
{
    int i;
    char *copy;

    i = 0;
    copy = NULL;
    if ((copy = malloc((my_strlen(str) + 1) * sizeof(char))) == NULL)
        return (NULL);
    while (str[i] != '\0')
    {
        copy[i] = str[i];
        i++;
    }
    copy[i] = '\0';
    return (copy);
}

int my_strcmp(const char *s1, const char *s2)
{
    int i;

    if (s1 == NULL || s2 == NULL)
        return (-2);
    i = 0;
    while (s1[i] != '\0')
    {
        if (s1[i] > s2[i])
            return (1);
        else if (s1[i] < s2[i])
            return (-1);
        i++;
    }
    if (s2[i] != '\0')
        return (-1);
    return (0);
}

int my_getnbr(char *str)
{
    int nb;
    int isneg;
    int i;

    isneg = 1;
    nb = 0;
    i = 0;
    while (str[i] == '+' || str[i] == '-')
    {
        if (str[i] == '-')
            isneg = isneg * -1;
        i = i + 1;
    }
    while (str[i] != '\0')
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            nb = nb * 10;
            nb = nb + str[i] - '0';
            i = i + 1;
        }
        else
            return (nb * isneg);
    }
    return (nb * isneg);
}

char *readline(void)
{
    ssize_t ret;
    char *buff;

    if ((buff = malloc((50 + 1) * sizeof(char))) == NULL)
        return (NULL);
    if ((ret = read(0, buff, 50)) > 1)
    {
        buff[ret - 1] = '\0';
        return (buff);
    }
    free(buff);
    return (NULL);
}

void my_reset(char *str, int size)
{
    int i;

    for (i = 0; i < size; i++)
        str[i] = '\0';
}

char *my_strstr(char *str, char *to_find)
{
    int i;

    if (str[0] != '\0')
    {
        i = 0;
        while (to_find[i] != '\0')
        {
            if (to_find[i] != str[i])
                return (my_strstr(str + 1, to_find));
            i = i + 1;
        }
        return (str);
    }
    else
        return (NULL);
}

int get_nb_field(char *str, char delimiter)
{
    int i = 0;
    int nb_field = 1;

    while (str[i] != '\0')
    {
        if (str[i] == delimiter)
        {
            nb_field++;
        }
        i++;
    }
    return (nb_field);
}

char **parse_command(char *str, char delimiter)
{
    int j = 0;
    int i = 0;
    int k = 0;
    int len = 0;
    int nb_field = get_nb_field(str, delimiter);
    char **tab = malloc(sizeof(char *) * (nb_field + 1));

    while (i < nb_field)
    {
        while (str[j] != delimiter && str[j] != '\0')
        {
            len++;
            j++;
        }
        tab[i] = malloc((len + 1) * sizeof(char));
        k = 0;
        while (k < len)
        {
            tab[i][k] = str[j - len + k];
            k++;
        }
        tab[i][k] = 0;
        len = 0;
        j++;
        i++;
    }
    tab[i] = NULL;
    return tab;
}

char *my_strcat(char *str1, char *str2)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (str1[i] != '\0')
    {
        i++;
    }
    while (str2[j] != '\0')
    {
        str1[i] = str2[j];
        i++;
        j++;
    }
    str1[i] = '\0';
    return (str1);
}

char *my_implode(char **tab, char *str, int offset)
{
    int length;
    char *current_element;
    char *imploded_string;

    length = 0;
    imploded_string = NULL;
    while ((current_element = tab[offset++]) != NULL)
    {
        length += my_strlen(current_element) + 1;
        imploded_string = realloc(imploded_string, length);
        my_strcat(imploded_string, current_element);
        my_strcat(imploded_string, str);
    }
    return imploded_string;
}

char *my_strcpy(char *dest, char *src)
{
    int i;

    i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return (dest);
}
