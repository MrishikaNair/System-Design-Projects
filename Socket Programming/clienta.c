//Mrishika Nair 2020389
//Sequential client(1a)

// PORT number
#define PORT 8080

#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>


int main(){
    
    char *loc_addr = "127.0.0.1";

    int sock_cli_id; // id refering to the socket
    struct sockaddr_in adr_serv; //refers to server address
    sock_cli_id = socket (AF_INET, SOCK_STREAM, 0) ;
    
    adr_serv.sin_port = htons(PORT);
    //port address
    adr_serv.sin_family= AF_INET;
    
    adr_serv.sin_addr.s_addr= inet_addr(loc_addr);

    int con_check= connect (sock_cli_id, (struct sockaddr *) &adr_serv, sizeof(adr_serv)) ;

    if (con_check == -1) {
        printf("Error while connecting!!!");
    }
    else
        printf("Established connection");
    int data = 1, inc=1;
    long res;
    do{
        send (sock_cli_id, &data , sizeof(data),0);
        printf("Number sent by client: %d\n", data);

        recv(sock_cli_id, &res, sizeof(res), 0) ;
        printf( "Value recieved: %lu\n", res) ;
        data+=1;
        inc++;
    
    }while(inc<=20);
    //closing socket
    close(sock_cli_id);
}