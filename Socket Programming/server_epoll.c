//Mrishika Nair 2020389
//Server epoll(2d)

// PORT number
#define PORT 8080

#include <stdio.h>
 #include <netinet/in.h> 
 #include <unistd.h> 
 #include <string.h>
 #include <errno.h>
 #include <arpa/inet.h>
 #include <poll.h>
 #include <sys/epoll.h>
 #include <time.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>


 #define CON_MAX 20

 int sockCreate() {
     struct sockaddr_in adr_serv;
     int sock_serv_id;

     sock_serv_id= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    
     printf("Created a socket with fd: %d\n", sock_serv_id);

     adr_serv.sin_port = htons(PORT);
     //port address
     adr_serv.sin_family= AF_INET;
     adr_serv.sin_addr.s_addr = INADDR_ANY ;

     int check;

     /* Step2: bind the socket to port 7000 on the local host */
     check = bind(sock_serv_id, (struct sockaddr *)&adr_serv, sizeof(struct sockaddr_in));
     if (check != 0) {
         printf("Binding failed");
         return -1;
     }

     /* Step3: listen for incoming connections */
     check = listen(sock_serv_id,25);
     if (check != 0) {
         printf("Listen failed");
         return -1;
     }
     return sock_serv_id;
 }

 int main(){

     struct sockaddr_in adr_cli;
     int sock_serv_id, efd, check, i;
     socklen_t len_cli;

     int nfds = CON_MAX-1;
     struct epoll_event fd_poll;
     struct epoll_event *fds_poll;

    /* Get the server sock id */
     sock_serv_id = sockCreate(); 
     if (sock_serv_id == -1) {
       printf("Server creation failed");
       return -1; 
     }  

     FILE *fileP=fopen("results_d_epoll.text","w"); 

     efd= epoll_create1(0);
     
     fd_poll.data.fd = sock_serv_id;
     fd_poll.events=EPOLLIN;
     int s=epoll_ctl(efd, EPOLL_CTL_ADD, sock_serv_id, &fd_poll);
     
     if(s==-1){
         perror("Error epoll_ctl");
         abort();
     }
     int useClient = 0;

     fds_poll = (struct epoll_event*)calloc(CON_MAX, sizeof(fd_poll));
     for(int i=0;i<CON_MAX;i++){
         fds_poll[i].data.fd = 0;
         fds_poll[i].events = 0;
     }
     while(1){
         check = epoll_wait(efd, fds_poll, CON_MAX, -1);
         if(check==0){
            continue;
         }
         
         for(int i=0;i<check; i++){


             int fd = fds_poll[i].data.fd;

             printf("Returned fd is %d (server's fd)\n", sock_serv_id);
            
             int new_fd = accept(sock_serv_id, (struct sockaddr*)&adr_cli, &len_cli);
             
                 if (new_fd >= 0) {
                     printf("Accepted a new connection with fd: %d\n", new_fd);
                     for (i=1;i < CON_MAX;i++) {
                         if (fds_poll[i].data.fd ==0) {
                             fds_poll[i].data.fd = new_fd; 
                             fds_poll[i].events = EPOLLIN;
                             useClient++;
                             break;
                         }
                     }
                 } else {
                     fprintf(stderr, "accept failed [%s]\n", strerror(errno));
                 }

         }
                 
          for(int i=1;i<CON_MAX;i++)  {
            if(fds_poll[i].data.fd>0 && fds_poll[i].events & EPOLLIN){     

             //memset(buf, 0, sizeof(buf));
   
            int sock_cli;
            int bufSize =read(fds_poll[i].data.fd, &sock_cli, sizeof(sock_cli)) ;


            if(bufSize==-1){               

                printf("Error reading");
            }
            else if(bufSize==0){
                fds_poll[i].data.fd =0;
                fds_poll[i].events =0;
                
                useClient--;
                break;
            }
            else{
                
                printf("From client: %d\n",sock_cli);

                printf("server recieved the number: %d from client \n", sock_cli);
                fprintf(fileP,"\n\nClient address is %s:%d\n", inet_ntoa(adr_cli.sin_addr), htons(adr_cli.sin_port));
                
                fprintf(fileP, "%s %s %s %d\n", "client", "sent", "the number:", sock_cli);
                int k=1;
                long fact_n=1;
                for(k=1;k<=sock_cli;k++){    
                    fact_n=fact_n*k; 
                }
                bufSize=write(fds_poll[i].data.fd, &fact_n , sizeof(fact_n));
                
                fprintf(fileP, "%s %s %s %lu\n", "server", "sent back", "the factorial:", fact_n);
                fflush(fileP);
                

                }

            }
          }
     }
     for (i=0;i < CON_MAX;i++) {
         close(fds_poll[i].data.fd);
     }
     free(fds_poll);
     close(sock_serv_id);
     return 0;

 }
