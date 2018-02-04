#include "server.h"

int                   main()
{   
    t_server *server;
    if ((server = create_server(12345)) == NULL)
    {
        printf("server error\n");
        return 1;
    }

    if ((bind(server->sockfd, (struct sockaddr *) &(server->serv_addr), sizeof(server->serv_addr))) < 0)  // on bind notre socket au port et a l'interface, et on verifie bien le retour
        perror("bind()");

    listen(server->sockfd, 5);  // on dit a notre socket d'ecouter d'eventuelles connexions entrante, avec un buffer de 5 (buffer d'attente, man listen)

    FD_ZERO(&(server->fds));  // on init notre fd_set
    FD_SET(server->sockfd, &(server->fds));  // on ne met dans notre fd_set que la socket serveur, les autres FDs ne nous interessent pas pour l'exemple

    while (1)  // boucle infinie
    {
        if (select(FD_SETSIZE, &(server->fds), NULL, NULL, NULL) < 0)  // appel blocant, on bloquera ici tant que la socket n'aura pas recu de connexion
        {
            perror("select()");
            return 1;
        }

        for (int i = 0; i < FD_SETSIZE; ++i)  // on boucle sur tous les files descriptor en commencant a 1 (d'ou la pre-incrementation)
        {
            if (FD_ISSET(i, &(server->fds)) && i == server->sockfd)  // on verifie si l'index courant a bien recu une modification et si il s'agit bien du FD de notre socket
            {
                t_client *client;
                if ((client = malloc(sizeof(t_client))) == NULL)
                {
                    printf("client error\n");
                    return 1;
                }
                client->clilen = sizeof(client->cli_addr);
                client->fd_id = accept(server->sockfd, (struct sockaddr *) &(client->cli_addr), &(client->clilen));  // on accept la nouvelle connexion, tmp contiendra le FD du nouveau client
                if (client->fd_id < 0)  // on check les erreurs
                    printf("cannot accept\n");
                else
                    printf("accepted new client with FD %d\n", client->fd_id);  // on indique qu'on a un nouveau client
            }
        }
    }
}
