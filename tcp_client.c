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

    if(sd == -1){
        perror(" Client's Socket Creation Failed!!\n");
        exit(0);
    }
    printf("Client's Socket created successfully!!\n");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    printf("Client now trying to connect\n");
    int new_conn = connect(sd,(struct sockaddr*)&address,sizeof(address));
    if(new_conn == -1){
        printf("Connection Failed %d\n",new_conn);
    }
    else{
        printf("Connection Established \n");
    }
    char *msg = "Hello Server";
    char buffer[1024] = {0};
    printf("Sending Message to Server\n");
    int sendMsg = send(sd, msg, strlen(msg),0);
    if(sendMsg == -1){
        printf("\nMessage Sending Failed\n");
    }

    printf("Message sent to Server\n");
    int readMsg = read(sd,buffer,1024);
    if(readMsg == -1){
        printf("\nFailed to read message from Client\n");
    }

    printf("Reading Message from Server\n");
    printf("\nMessage from Server:\n");
    printf("%s\n",buffer);
    
    return 0;
}