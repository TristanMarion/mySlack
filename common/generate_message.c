#include "includes_common.h"

char *generate_message(char *str, ...)
{
    va_list args;
    va_list tmp_args;
    int needed;
    char *generated_message;

    va_start(args, str);
    va_copy(tmp_args, args);
    needed = vsnprintf(NULL, 0, str, args) + 1;
    generated_message = malloc(needed);
    vsnprintf(generated_message, needed, str, tmp_args);
    free(str);
    return (generated_message);
}
