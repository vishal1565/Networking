#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#define PORT 1600
#define cipherKey 'S' 
#define sendrecvflag 0 
#define nofile "File Not Found!" 

void clearBuf(char* b) 
{ 
    int i; 
    for (i = 0; i < 1024; i++) 
        b[i] = '\0'; 
} 
  
// funtion to encrypt 
char Cipher(char ch) 
{ 
    return ch ^ cipherKey; 
} 
  
// funtion sending file 
int sendFile(FILE* fp, char* buf, int s) 
{ 
    int i, len; 
    if (fp == NULL) { 
        strcpy(buf, nofile); 
        len = strlen(nofile); 
        buf[len] = EOF; 
        for (i = 0; i <= len; i++) 
            buf[i] = Cipher(buf[i]); 
        return 1; 
    } 
  
    char ch, ch2; 
    for (i = 0; i < s; i++) { 
        ch = fgetc(fp); 
        ch2 = Cipher(ch); 
        buf[i] = ch2; 
        if (ch == EOF) 
            return 1; 
    } 
    return 0; 
} 
  
int main(){
    int sd;
    struct sockaddr_in server;
    int addrlen = sizeof(server);
    char buffer[1024], command[10];
    
    sd = socket(AF_INET,SOCK_DGRAM,0);
    if(sd==-1){
        printf("Socket Creation Failed...\n");
        exit(0);
    }
    else{
        printf("Socket Creation Successful!!...\n");
    }

    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(PORT);

    int binding=bind(sd,(struct sockaddr *)&server,sizeof(server));
    if(binding==-1){
        printf("Binding Failed!!...\n");
    }
    else{
        printf("\nSocket Binding Successful!!...\n");
    }
    listen(sd,3);
    accept(sd,(struct sockaddr *)&server,(socklen_t*)&addrlen);

    FILE *fp;
    while(1){
        printf("\nWaiting for Client's Request....\n");
        clearBuf(buffer);
        int nBytes=recvfrom(sd,buffer,2048,sendrecvflag,(struct sockaddr *)&server,&addrlen);
        printf("\nClient is requesting file: %s\n",buffer);
        fp=fopen(buffer,"r");
        if(fp==NULL){
            printf("\nFailed to Open File !!\n");
        }
        printf("Now sending file to client..\n");
         while (1) { 
            // process 
            if (sendFile(fp, buffer, 2048)) { 
                sendto(sd, buffer, 2048, sendrecvflag, (struct sockaddr*)&server, addrlen); 
                break; 
            } 
            // send 
            sendto(sd, buffer, 2048, sendrecvflag, (struct sockaddr*)&server, addrlen); 
            clearBuf(buffer); 
        } 
        if (fp != NULL) 
            fclose(fp); 
    }

}