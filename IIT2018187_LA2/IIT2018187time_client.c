/*
submitted bby Abhishek(IIT2018187)
*/
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#define IP_PROTOCOL 0 
#define IP_ADDRESS "127.0.0.1" // localhost 
#define PORT_NO 5000 
#define KeyC 'S' 
#define flagrc 0 

int main()
{
    int socketFileDescrptr = 0, n = 0;
    char buffer_net[1024];
    struct sockaddr_in serv_addr;
    memset(buffer_net, '0',sizeof(buffer_net));
    if((socketFileDescrptr = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n socket not created \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 

    if(inet_pton(AF_INET,IP_ADDRESS, &serv_addr.sin_addr)<=0)
    {
        printf("\n inet errror\n");
        return 1;
    } 

    if( connect(socketFileDescrptr, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n connection cant establish\n");
       return 1;
    } 

    while ( (n = read(socketFileDescrptr, buffer_net, sizeof(buffer_net)-1)) > 0)
    {
        buffer_net[n] = 0;
        if(fputs(buffer_net, stdout) == EOF)
        {
            printf("\n erooor\n");
        }
    } 

    if(n < 0)
    {
        printf("\n cant read \n");
    } 

    return 0;
}
