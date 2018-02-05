#include "includes_client.h"
 
int main(int argc, char** argv)
{
    if (argc != 3) {
        put_info("Usage : ./client serv_addr port\n");
        return (1);
    }

    int sock;
    struct sockaddr_in server;
    //char message[1000] , server_reply[2000];
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        put_error("Could not create socket");
    }
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(my_getnbr(argv[2]));
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        put_error("connect failed. Error");
        return 1;
    }
     
    put_success("Connected\n");
     
    //keep communicating with server
    while(1)
    {
        /*printf("Enter message : ");
        scanf("%s" , message);
         
        //Send some data
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
         
        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
         
        puts("Server reply :");
        puts(server_reply);*/
    }
    
    return 0;
}
