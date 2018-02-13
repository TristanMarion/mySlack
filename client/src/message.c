#include "includes_client.h"

void handle_message(int *end, char *message)
{
    (void)end;
    char **parsed_message;
    
    parsed_message = parse_command(message, ';');
    put_info(parsed_message[0]);
    put_info(parsed_message[1]);
}
