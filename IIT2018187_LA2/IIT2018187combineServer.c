// server code for UDP and tcp socket programming 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include<errno.h>
#include<netdb.h>
#include<time.h>

#define IP_PROTOCOL 0 
#define PORT_NO 5000 
#define cipherKey 'S' 
#define flagrc 0 
int max(int x, int y) 
{ 
    if (x > y) 
        return x; 
    else
        return y; 
} 
int hostname_to_ip(char * hostname , char* ip)
{
	struct hostent *he;
	struct in_addr **addr_list;
	int i;
		
	if ( (he = gethostbyname( hostname ) ) == NULL) 
	{
		// get the host info
		herror("gethostbyname");
		return 1;
	}

	addr_list = (struct in_addr **) he->h_addr_list;
	
	for(i = 0; addr_list[i] != NULL; i++) 
	{
		//Return the first one;
		strcpy(ip , inet_ntoa(*addr_list[i]) );
		//strcat(ip," \n");
		return 0;
	}
	
	return 1;
}

void bufferClear(char* b) 
{ 
	int i; 
	for (i = 0; i < 32; i++) 
		b[i] = '\0'; 
} 

// function to encrypt 

int main() 
{ 
	int jk=1;
	int jj=1;
	char net_buf[32]; 
	char ip[32];
    char sendBuff[1025];
    time_t ticks; 
fd_set rset;
	struct sockaddr_in addr_con; 
	int lengthOfAdress = sizeof(addr_con); 

	 int listenfd = 0, connfd = 0;

    listenfd = socket(AF_INET, SOCK_STREAM, IP_PROTOCOL);
     memset(&addr_con, '0', sizeof(addr_con));
    memset(sendBuff, '0', sizeof(sendBuff)); 

	addr_con.sin_family = AF_INET; 

	addr_con.sin_port = htons(PORT_NO); 

	addr_con.sin_addr.s_addr = INADDR_ANY; 
	 bind(listenfd, (struct sockaddr*)&addr_con, sizeof(addr_con)); 
	 printf("\nSuccessfully tcp binded!\n"); 
	  listen(listenfd, 10); 


	

	//FILE* fp; 

	// socket() 
	int usocketFileDescripter = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL); 


	if (usocketFileDescripter < 0) 
		printf("\n!!!!!!!!!!1descriptor not found!!!1!!\n"); 
	else
		printf("\n#########file descriptor %d received########\n", usocketFileDescripter); 

	// bind() 
	if (bind(usocketFileDescripter, (struct sockaddr*)&addr_con, sizeof(addr_con)) == 0) 
		printf("\nSuccessfully udp binded!\n"); 
	else
		printf("\nBinding Failed!\n"); 
	FD_ZERO(&rset); 
	int maxfdp1 = max(listenfd, usocketFileDescripter) + 1; 


	while (1) { 
		FD_SET(listenfd, &rset); 
        FD_SET( usocketFileDescripter, &rset); 
         int nready = select(maxfdp1, &rset, NULL, NULL, NULL);
      if (FD_ISSET(usocketFileDescripter, &rset)){
		printf("\n%d udp client. connection\n",jj++); 

		// receive file name 
		bufferClear(net_buf);
		bufferClear(ip); 

		int nBytes = recvfrom(usocketFileDescripter, net_buf, 
						32, flagrc, 
						(struct sockaddr*)&addr_con, &lengthOfAdress); 
		printf("DNS host recv\n");

	
		hostname_to_ip(net_buf , ip);//{printf("\n!!!!!!!!!!!!addr not found !!!!!!!!!1\n")
		//printf("%s\n",ip );
		bufferClear(net_buf);
           int i;
          for(i=0;ip[i]!='\0';i++)
            net_buf[i]=ip[i];

		net_buf[i]=EOF;

		
			sendto(usocketFileDescripter, net_buf , 32, 
				flagrc, 
				(struct sockaddr*)&addr_con, lengthOfAdress); 
			
			bufferClear(ip);
		}
		if (FD_ISSET(listenfd, &rset)){
				
				connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
                 printf("\n%d  tcp  client conection",jk);
                 jk++;

             ticks = time(NULL);
             snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
             write(connfd, sendBuff, strlen(sendBuff)); 

             close(connfd);
             sleep(1);
		}

		
	} 
	return 0; 
} 