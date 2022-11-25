//Mrishika Nair 2020389
//Concurrent server using threads multi server(2a)

// PORT number
#define PORT 8080

#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>


int main(){
    char *loc_addr = "127.0.0.1";
    int sock_serv_id; //id refering to server
    sock_serv_id = socket (AF_INET , SOCK_STREAM, 0);
    struct sockaddr_in adr_serv;

    int sock_cli_id; //id refering to client
    struct sockaddr_in adr_cli;
    
    
    adr_serv.sin_port = htons(PORT);
    //port address
    adr_serv.sin_family= AF_INET;
    adr_serv.sin_addr.s_addr = INADDR_ANY ;

    FILE *fileresult;
    fileresult=fopen("results_a.text","w");

    socklen_t server=sizeof(adr_serv);
    bind(sock_serv_id, (struct sockaddr*)&adr_serv, server);
    listen(sock_serv_id, 5);

    
    socklen_t clieln= sizeof(struct sockaddr_in);
    sock_cli_id = accept (sock_serv_id, (struct sockaddr *)&adr_cli ,&clieln);
    
    //recieve and calculate
    int i=1;
    while(i<=20){
        
        int cli_msgrec ; //stores the message sent by client
        fprintf(fileresult,"\nAddress of client %s:%d\n", inet_ntoa(adr_cli.sin_addr), htons(adr_cli.sin_port));
        int n=recv(sock_cli_id, &cli_msgrec, sizeof(cli_msgrec), 0) ;
        if(n<0){
            printf("error while recieving message");
        }
        printf("Number sent by client %d\n", cli_msgrec);
        fprintf(fileresult, "%s %d\n", "Number sent by client:", cli_msgrec);
        fflush(fileresult);
        int i=1;
        long fact_n=1;
        for(i=1;i<=cli_msgrec;i++){    
        fact_n=fact_n*i; 
        }
        send (sock_cli_id, &fact_n , sizeof(fact_n),0);
        fprintf(fileresult, "%s %lu\n", "Factorial recieved:", fact_n);
        i++;
        
    }
    
    fclose(fileresult);
    pclose(sock_serv_id);
    return 0;
}