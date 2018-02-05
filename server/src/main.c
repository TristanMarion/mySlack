#include "includes_server.h"

int main()
{
    t_server *server;
    if ((server = create_server(12345)) == NULL)
    {
        put_error("server error\n");
        return 1;
    }

    put_info("Server up\n");
    if ((bind(server->sockfd, (struct sockaddr *)&(server->serv_addr), sizeof(server->serv_addr))) < 0) // on bind notre socket au port et a l'interface, et on verifie bien le retour
        put_error("bind()");

    put_success("Server bind success\n");
    init_server(server);
    put_info("Waiting for connections\n");    
    while (1) // boucle infinie
    {
        if (select(FD_SETSIZE, &(server->fds), NULL, NULL, NULL) < 0) // appel blocant, on bloquera ici tant que la socket n'aura pas recu de connexion
        {
            put_error("select()");
            return 1;
        }

        for (int i = 0; i < FD_SETSIZE; ++i) // on boucle sur tous les files descriptor en commencant a 1 (d'ou la pre-incrementation)
        {
            if (FD_ISSET(i, &(server->fds)) && i == server->sockfd) // on verifie si l'index courant a bien recu une modification et si il s'agit bien du FD de notre socket
            {
                if (new_client(server) != 0) {
                    put_error("new client");
                    return 1;
                }
            }
        }
    }
}
