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
    clock_t start,end;
    double t1, avg=0;
    int sd = socket(AF_INET,SOCK_DGRAM,0);
    if(sd<0){
        perror("Failed to create Socket...\n");
        exit(EXIT_FAILURE);
    }

    serverAdd.sin_family = AF_INET;
    serverAdd.sin_addr.s_addr = INADDR_ANY;
    serverAdd.sin_port = htons(PORT);

    int len,n,i;
    char buffer[1024];
    char *msg = "PING from Client";
    for (i=0;i<10;i++){
        start = clock();
        sendto(sd, (char *)msg, strlen(msg), MSG_CONFIRM, (struct sockaddr *)&serverAdd, sizeof(serverAdd));
        printf("Client: %s\n",msg);

        n = recvfrom(sd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&cliAdd, &len);
        buffer[n] = '\0';
        printf("Server: %s\n",buffer);
        end = clock();
        t1 = (double)((end-start)/CLOCKS_PER_SEC)*1000000;
        avg += t1;
    }
    avg /= 10;
    printf("Average RTT is %f\n",avg);
    return 0;
}