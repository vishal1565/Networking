#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#define PORT 1555

int main(){
    struct sockaddr_in address;
    int sd = socket(AF_INET,SOCK_STREAM,0);
    char *serverMsg = "Hello Client";
    char buffer[1024] = {0};

    if(sd == -1){
        perror("Server's Socket Creation Failed!!\n");
        exit(0);
    }
    printf("Server's Socket created successfully!!\n");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    bind(sd,(struct sockaddr*)&address,sizeof(address));
    listen(sd,1);
    printf("Server is in LISTEN MODE\n");
    
    int add_len = sizeof(address);
    int accept_conn = accept(sd,(struct sockaddr*)&address,(socklen_t*)&add_len);
    
    printf("Reading Message from Client\n");
    int readMsg = read(accept_conn,buffer,1024);
    if(readMsg == -1){
        printf("\nFailed to read message from client\n");
    }

    printf("Message from Client:\n");
    printf("%s\n",buffer);
    printf("\nNow Sending Message to client\n");
    int sendMsg = send(accept_conn,serverMsg,strlen(serverMsg),0);
    if(sendMsg == -1){
        printf("\nFailed to send message to client\n");
    }
    printf("Message sent to Client\n");
    return 0;
}