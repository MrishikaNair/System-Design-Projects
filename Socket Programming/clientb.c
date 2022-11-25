// Client Side program to test
// the TCP server that returns
// a 'hi client' message

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

// PORT number
#define PORT 8080


void *threadfunc(void *arg)
{
	int sock_cli;
    sock_cli = socket (AF_INET, SOCK_STREAM, 0) ;

    struct sockaddr_in serv_adr;
    serv_adr.sin_family= AF_INET;
    serv_adr.sin_port = htons(PORT);
    serv_adr.sin_addr.s_addr= inet_addr("127.0.0.1");


    int connection_status = connect (sock_cli, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) ;

    if (connection_status == -1) {
        printf("connection err");
    }
    printf("Connection established!");
    
    int msg = 1;
    int count = 1;
    while(count<=20){
        

            send (sock_cli, &msg , sizeof(msg),0);
            printf("the client sent: %d\n", msg);

        
            long res ;
            recv(sock_cli, &res, sizeof(res), 0) ;
         
            printf( "The server sent the data: %lu\n", res) ;
    

        msg+=1;
        count++;
        

    }
    return NULL;
}


int main(){

    pthread_t thread_id[10];
    for(int i=0;i<10;i++){
     
        pthread_create(&(thread_id[i]), NULL, threadfunc, NULL);
    }
    for(int i=0;i<10;i++){
        pthread_join(thread_id[i], NULL);
    }
    return 0;
}