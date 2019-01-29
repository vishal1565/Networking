#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<time.h>
#define PORT 1555

int main(){
    struct sockaddr_in serverAdd, cliAdd;
    
    int sd = socket(AF_INET,SOCK_DGRAM,0);
    if(sd<0){
        perror("Failed to create Socket...\n");
        exit(EXIT_FAILURE);
    }

    memset(&serverAdd, 0, sizeof(serverAdd));
    memset(&cliAdd, 0, sizeof(cliAdd));

    serverAdd.sin_family = AF_INET;
    serverAdd.sin_addr.s_addr = INADDR_ANY;
    serverAdd.sin_port = htons(PORT);

    int bindStatus = bind(sd,(struct sockaddr*)&serverAdd,sizeof(serverAdd));
    if(bindStatus<0){
        perror("Binding Failed..\n");
        exit(EXIT_FAILURE);
    }

    int len,n,i;
    len = sizeof(cliAdd);
    char buffer[1024];
    for(i=0;i<10;i++){
        n = recvfrom(sd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&cliAdd, &len);
        buffer[n] = '\0';
        printf("Client: %s\n",buffer);

        char *msg = "PONG from Server";
        sendto(sd, (char *)msg, strlen(msg), MSG_CONFIRM, (struct sockaddr *)&cliAdd, len);
        printf("Server: %s\n",msg);
    }
    return 0;
}