#include "includes_server.h"

int main()
{
    t_server *server;
    if ((server = create_server(12345)) == NULL)
    {
        put_error("server error");
        return (1);
    }

    if(init_server(server) != 0)
    {
        put_error("init server error");
        return (1);
    }

    main_loop(server);
}
