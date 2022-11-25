//Mrishika Nair 2020389
//Concurrent server using multi threading(2b)

// PORT number
#define PORT 8080
 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

 
 
struct variable_struct
{
   int sock_cli;
   FILE *fileP;
   struct sockaddr_in cli_adr;
};

void *func_cal(void *var){
    int cli_req;
    for (int i=0 ;i<20; i=i+1 ){
        struct variable_struct *var_size = var;
        int n=read(var_size->sock_cli, &cli_req, sizeof(cli_req)) ;
        if(n<0){
            printf("oooppps");
        }
        printf("server recieved the number: %d from client \n", cli_req);
        fprintf(var_size->fileP,"\n\nClient address is %s:%d\n", inet_ntoa((var_size->cli_adr).sin_addr), htons((var_size->cli_adr).sin_port));
        
        fprintf(var_size->fileP, "%s %s %s %d\n", "client", "sent", "the number:", cli_req);
        int i=1;
        long fact_n=1;
        for(i=1;i<=cli_req;i++){    
        fact_n=fact_n*i; 
        }
        write(var_size->sock_cli, &fact_n , sizeof(fact_n));
        fprintf(var_size->fileP, "%s %s %s %lu\n", "server", "sent back", "the factorial:", fact_n);
        fflush(var_size->fileP);
    }
}
int main()
{
    // Server socket id
    int sock_serv;
 
    // Server socket address structures
    struct sockaddr_in serv_adr;
    struct sockaddr_in cli_adr;
 
    // Creates a TCP socket id from IPV4 family
    sock_serv = socket(AF_INET, SOCK_STREAM, 0);
 
    // Error handling if socket id is not valid
    if (sock_serv < 0) {
        printf("Error in connection.\n");
        exit(1);
    }
 
    printf("Server Socket is created.\n");
  
    // Assign port number and IP address
    // to the socket created
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_port = htons(PORT);
 
    // 127.0.0.1 is a loopback address
    serv_adr.sin_addr.s_addr
        = inet_addr("127.0.0.1");
 
    // Binding the socket id with
    // the socket structure
    int ret = bind(sock_serv,(struct sockaddr*)&serv_adr,
               sizeof(serv_adr));
 
    // Error handling
    if (ret < 0) {
        printf("Error in binding.\n");
        exit(1);
    }
 
    // Listening for connections (upto 10)
    listen(sock_serv, 10);
    printf("Listening...\n\n");
    
 
    int cnt = 0;
    
    FILE *fileP=fopen("results_c.text","w");
    
    //struct sockaddr_in cli_adr;
    int sock_cli;
    int accepted[10];
    pthread_t threads[10];
    for(int i=0;i<10;i++){
        socklen_t addrlen= sizeof(struct sockaddr_in);
        sock_cli = accept (sock_serv, (struct sockaddr *)&(cli_adr) ,&addrlen);
        accepted[i]=sock_cli;
    }
    for (int i=0 ;i<10; i=i+1 ){
 
        //socklen_t addrlen= sizeof(struct sockaddr_in);
        //sock_cli = accept (sock_serv, (struct sockaddr *)&(cli_adr) ,&addrlen);
            struct variable_struct *var_size =malloc(sizeof(struct variable_struct) * 1);
            var_size->sock_cli=accepted[i];
            var_size->fileP=fileP;
            var_size->cli_adr=cli_adr;

            pthread_create(&(threads[i]), NULL, &func_cal, (void *)var_size);
    }
    for(int i=0;i<10;i++){
            pthread_join(threads[i], NULL);
    }
    
    // Close the client socket id
    //wait(NULL);
    close(sock_cli);
    return 0;
}