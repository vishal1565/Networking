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
  
// funtion to clear buffer 
void clearBuf(char* b) 
{ 
    int i; 
    for (i = 0; i < 1024; i++) 
        b[i] = '\0'; 
} 
  
// function for decryption 
char Cipher(char ch) 
{ 
    return ch ^ cipherKey; 
} 
  
// function to receive file 
int recvFile(char* fileName, char* buf, int s){ 
    int i; 
    char ch; 
    FILE *fp;
    fp = fopen(fileName,"w");
    for (i = 0; i < s; i++) { 
        ch = buf[i]; 
        ch = Cipher(ch); 
        if (ch == EOF){
            printf("\nFile Saved Successfully in '%s'\n",fileName);
            fclose(fp);
            return 1; 
        }
        else{
            fprintf(fp,"%c",ch);
        }
    } 
    return 0; 
} 
  
int main(){
    int sock;
    struct sockaddr_in client;
    char buffer[1024];
    int addrlen=sizeof(client);
    sock=socket(AF_INET,SOCK_DGRAM,0);
    if(sock==-1){
        printf("Not created\n");
    }
    client.sin_family=AF_INET;
    client.sin_addr.s_addr=INADDR_ANY;
    client.sin_port=htons(PORT);

    while(1){
        char *fileName = malloc(sizeof(char));
        printf("\nEnter the FileName:\n>> ");
        scanf("%s",fileName);
        sendto(sock, fileName, 2048, sendrecvflag, (struct sockaddr *)&client, addrlen);
          while (1) { 
            // receive 
            clearBuf(buffer); 
            int  nBytes = recvfrom(sock, buffer, 2048, sendrecvflag, (struct sockaddr*)&client, &addrlen); 
  
            // process 
            if (recvFile(fileName, buffer, 2048)) { 
                break; 
            } 
        } 
    }
   
}