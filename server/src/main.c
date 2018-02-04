#include "server.h"
#include "libmy.h"

int main()
{
    t_server *server;
    if ((server = create_server(12345)) == NULL)
    {
        my_putstr("server error\n");
        return 1;
    }

    my_putstr("Server up\n");
    if ((bind(server->sockfd, (struct sockaddr *)&(server->serv_addr), sizeof(server->serv_addr))) < 0) // on bind notre socket au port et a l'interface, et on verifie bien le retour
        perror("bind()");

    my_putstr("Server bind success\n");
    init_server(server);
    my_putstr("Waiting for connections\n");    
    while (1) // boucle infinie
    {
        if (select(FD_SETSIZE, &(server->fds), NULL, NULL, NULL) < 0) // appel blocant, on bloquera ici tant que la socket n'aura pas recu de connexion
        {
            perror("select()");
            return 1;
        }

        for (int i = 0; i < FD_SETSIZE; ++i) // on boucle sur tous les files descriptor en commencant a 1 (d'ou la pre-incrementation)
        {
            if (FD_ISSET(i, &(server->fds)) && i == server->sockfd) // on verifie si l'index courant a bien recu une modification et si il s'agit bien du FD de notre socket
            {
                if (new_client(server) != 0) {
                    perror("new client");
                    return 1;
                }
            }
        }
    }
}
