//Mrishika Nair 2020389
//Concurrent server using fork(2b)

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
 
int main()
{
    char *loc_addr = "127.0.0.1";
    int sock_serv;  //id refering to server
    sock_serv = socket(AF_INET, SOCK_STREAM, 0); //TCP connect socket
    // Server socket address structures
    struct sockaddr_in adr_serv;

 
    // Child process id
    int childpid;
 
    // Error handling if socket id is not valid
    if (sock_serv < 0) {
        printf("Error in connection.\n");
        exit(1);
    }
 
    printf("Server Socket is created.\n");
  
    adr_serv.sin_port = htons(PORT);
    //port address
    adr_serv.sin_family= AF_INET;
    adr_serv.sin_addr.s_addr= inet_addr(loc_addr);
 
    // Binding the socket id with
    // the socket structure
    bind(sock_serv,(struct sockaddr*)&adr_serv, sizeof(adr_serv));
 
    // Listening upto 10 connections 
    listen(sock_serv, 10);
    printf("Listening going on...\n");
    
    FILE *fileresult;
    fileresult=fopen("results_b.text","w");
    
    struct sockaddr_in cli_adr;
    int sock_cli;
    for (int j=0 ;j<10; j=j+1 ){
 
        socklen_t addrlen= sizeof(struct sockaddr_in);
        // store their information in cliAddr
        sock_cli = accept (sock_serv, (struct sockaddr *)&cli_adr ,&addrlen);
 
        // Creates a child process
        if ((childpid = fork()) == 0) {
            
            int cli_msgrec;
            for (int i=0 ;i<20; i=i+1 ){
                recv(sock_cli, &cli_msgrec, sizeof(cli_msgrec), 0) ;
                
                int i=1;
                long fact_n=1;
                for(i=1;i<=cli_msgrec;i++){    
                fact_n=fact_n*i; 
                }
                printf("Number sent by client %d\n", cli_msgrec);
                send (sock_cli, &fact_n , sizeof(fact_n),0);

                fprintf(fileresult,"\n\nClient address is %s:%d\n", inet_ntoa(cli_adr.sin_addr), htons(cli_adr.sin_port));
                fprintf(fileresult, "%s %d\n", "Number sent by client:", cli_msgrec);
                fprintf(fileresult, "%s %lu\n", "Factorial sent back:", fact_n);
            }
            exit(0);
        }else{
            continue;
        }

    }
    
    // Close the client socket id
    wait(NULL);
    //=close(sock_cli);
    return 0;
}