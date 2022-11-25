//Mrishika Nair 2020389
//Server select(2d)

// PORT number
#define PORT 8080
#include <stdio.h>
 #include <netinet/in.h> 
 #include <unistd.h> 
 #include <string.h>
 #include <errno.h>
 #include <arpa/inet.h>

 #define CON_MAX 20

 int sockCreate() {
     struct sockaddr_in adr_serv;
     int sock_serv_id;
     
     sock_serv_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //tcp socket creation

     printf("Created a socket with socket id: %d\n", sock_serv_id);

     adr_serv.sin_port = htons(PORT);
     //port address
     adr_serv.sin_family= AF_INET;
     adr_serv.sin_addr.s_addr = INADDR_ANY ;
     
     int check; //error handling
     check = bind(sock_serv_id, (struct sockaddr *)&adr_serv, sizeof(struct sockaddr_in));
     if (check != 0) {
         printf("Binding failed");
         return -1;
     }

     check= listen(sock_serv_id,15);
     if (check!= 0) {
         printf("Listen failed");
         return -1;
     }
     return sock_serv_id;
 }

 int main () {
     fd_set read_sock_set;
     int sock_serv_id, sock_cli_id, check, i;

     struct sockaddr_in adr_cli;
     int pos_con[CON_MAX];
     socklen_t len_cli;
     
     

     /* Get the server sock id*/
     sock_serv_id = sockCreate(); 
     if (sock_serv_id == -1) {
       printf("Server creation failed");
       return -1; 
     }  

     FILE *fileP=fopen("results_d_select.text","w"); 
     pos_con[0] = sock_serv_id;
     for (i=1;i < CON_MAX ;i++) {
         pos_con[i] = -1; //initialising connections, first is server
     }
     
    int n=1;
     while (n<=200) {
         FD_ZERO(&read_sock_set); 

         for (i=0;i < CON_MAX ;i++) {
             if (pos_con[i] >= 0) {
                 FD_SET(pos_con[i], &read_sock_set); //seting before select
             }
         }
         check= select(FD_SETSIZE, &read_sock_set, NULL, NULL, NULL); //events incoming are listened

         if (check>= 0 ) {
             printf("Return value of select: %d\n", check);
             
             if (FD_ISSET(sock_serv_id, &read_sock_set)) { //checking if its the server only then accepts take place
                 
                 
                 sock_cli_id = accept(sock_serv_id, (struct sockaddr*)&adr_cli, &len_cli);
                 if (sock_cli_id >= 0) {
                     for (i=0;i < CON_MAX ;i++) {
                         if (pos_con[i] <0) {
                             pos_con[i] = sock_cli_id; 
                             break;
                         }
                     }
                     printf("New connection accepted of id: %d\n", sock_cli_id);
                 } else {
                     printf("Accept failed");
                 }
                 printf("Returned socket id is %d\n", sock_serv_id);
                 if(check==1)
                    continue; //continue the process

                
             } 
            // For the nonserver event ids
             for (i=1;i <=CON_MAX ;i++) {
                 if ((pos_con[i] > 0) && (FD_ISSET(pos_con[i], &read_sock_set))) {
                        
                    int sock_cli;
                    int cli_msgrec;
                    sock_cli =read(pos_con[i], &cli_msgrec, sizeof(cli_msgrec)) ;
                    
                     if (sock_cli == 0) {
                         printf("Closing connection for client id:%d\n", pos_con[i]);
                         close(pos_con[i]);
                         pos_con[i] = -1; // Connection closed
                     } 
                     else if (sock_cli== -1) {
                         printf("Reciving failed for client id: %d \n", pos_con[i]);
                        close(pos_con[i]);
                        pos_con[i]=0;
                         break;
                     }
                     else{
                        printf("Number recieved: %d \n", cli_msgrec);
                        
                        long fact_n=1;
                        for(int j=1;j<=cli_msgrec;j++){    
                            fact_n=fact_n*j; 
                        }
                        sock_cli=write(pos_con[i], &fact_n , sizeof(fact_n));

                        fprintf(fileP,"\n\nClient address is %s:%d\n", inet_ntoa(adr_cli.sin_addr), htons(adr_cli.sin_port));
                        fprintf(fileP, "%s %d\n", "Number sent by client:", cli_msgrec);
                        fprintf(fileP, "%s %lu\n", "Factorial sent:", fact_n);

                        fflush(fileP);
                     }
                 }
             }
         } 
     } 

     for (i=0;i < CON_MAX ;i++) {
         if (pos_con[i] > 0) {
             close(pos_con[i]);
         }
     }
     return 0;
 }