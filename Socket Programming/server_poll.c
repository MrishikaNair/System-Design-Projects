//Mrishika Nair 2020389
//Server poll(2d)

// PORT number
#define PORT 8080
#include <stdio.h>
 #include <netinet/in.h> 
 #include <unistd.h> 
 #include <string.h>
 #include <errno.h>
 #include <arpa/inet.h>
 #include <poll.h>
 #include <time.h>

  #define CON_MAX 20

 int sockCreate() {
     struct sockaddr_in adr_serv;
     int sock_serv_id;

     sock_serv_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
     
     printf("Created a socket with socket id: %d\n", sock_serv_id);

     adr_serv.sin_port = htons(PORT);
     //port address
     adr_serv.sin_family= AF_INET;
     adr_serv.sin_addr.s_addr = INADDR_ANY ;

    int check;
     check = bind(sock_serv_id, (struct sockaddr *)&adr_serv, sizeof(struct sockaddr_in));
     if (check != 0) {
         printf("Binding failed");
         return -1;
     }

     check = listen(sock_serv_id,15);
     if (check != 0) {
         printf("Listen failed");
         return -1;
     }
     return sock_serv_id;
 }
 int main(){
     struct sockaddr_in adr_cli;
     int sock_serv_id, sock_cli_id, check, i;
     socklen_t len_cli;

     struct pollfd fd_poll[CON_MAX+1];
     struct pollfd *pfds;
     int nfds = CON_MAX-1;
     

     /* Get the server sock id */
     sock_serv_id = sockCreate(); 
     if (sock_serv_id == -1) {
       printf("Server creation failed");
       return -1; 
     }  

     FILE *fileP=fopen("results_d_poll.text","w"); 
     
     int cli_onuse=0;

     fd_poll[0].fd=sock_serv_id;
     fd_poll[0].events=POLLIN;
     int j=1;
     while(j<CON_MAX){
        fd_poll[j].fd = 0;
         fd_poll[j].events = POLLIN;
        j++;
     }
     
     while(1){
         check =poll(fd_poll, cli_onuse+1 , 5000);

         if(check>=0){
            if(fd_poll[0].revents & POLLIN){ //checking if server is readi=y to pollin (accept clients)

                 sock_cli_id = accept(sock_serv_id, (struct sockaddr*)&adr_cli, &len_cli);
                 if (sock_cli_id >= 0) {


                     for (i=1;i < CON_MAX;i++) {
                         if (fd_poll[i].fd ==0) {
                             cli_onuse++;
                             fd_poll[i].fd = sock_cli_id; 
                             fd_poll[i].events = POLLIN;
                             break;
                         }
                     }
                     printf("New connection accepted of id: %d\n", sock_cli_id);
                 } else {
                     printf("Accept failed");
                 }
                 printf("Returned socket id is %d\n", sock_serv_id);
                 if(check==1)
                    continue;

            }
           // For the nonserver event ids
            for (i=1;i <=CON_MAX;i++){
                int sock_cli;
                if(fd_poll[i].fd>0 && fd_poll[i].revents & POLLIN){
                    
                    int cli_msgrec =read(fd_poll[i].fd, &sock_cli, sizeof(sock_cli)) ;
                    if(cli_msgrec<=0){
                        cli_onuse--;
                        fd_poll[i].fd =0;
                        fd_poll[i].revents =0;
                        fd_poll[i].events =0;
                    }
                    else{
                        
                        long fact_n=1;
                        for(int j=1;j<=sock_cli;j++){    
                            fact_n=fact_n*j; 
                        }
                        printf("Number recieved: %d \n",sock_cli);
                        cli_msgrec =write(fd_poll[i].fd, &fact_n , sizeof(fact_n));
                       
                        fprintf(fileP,"\n\nClient address is %s:%d\n", inet_ntoa(adr_cli.sin_addr), htons(adr_cli.sin_port));
                        fprintf(fileP, "%s %d\n", "Number sent by client:", sock_cli);
                        fprintf(fileP, "%s %lu\n", "Factorial sent:", fact_n);

                        fflush(fileP);
                    }

                }
            }


         }
         else
            printf("Connection failed.\n");
     }
     return 0;
 }