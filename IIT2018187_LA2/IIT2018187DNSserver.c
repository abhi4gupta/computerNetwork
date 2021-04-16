/*
submitted bby Abhishek(IIT2018187)
*/
// server code for UDP socket programming 
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

#define IP_PROTOCOL 0 
#define PORT_NO 5000 
#define cipherKey 'S' 
#define flagrc 0 
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
	
	struct sockaddr_in addr_con; 
	int lengthOfAdress = sizeof(addr_con); 

	addr_con.sin_family = AF_INET; 

	addr_con.sin_port = htons(PORT_NO); 

	addr_con.sin_addr.s_addr = INADDR_ANY; 

	char net_buf[32]; 
	char ip[32];

	//FILE* fp; 

	// socket() 
	int socketFileDescripter = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL); 

	if (socketFileDescripter < 0) 
		printf("\n!!!!!!!!!!1descriptor not found!!!1!!\n"); 
	else
		printf("\n#########file descriptor %d received########\n", socketFileDescripter); 

	// bind() 
	if (bind(socketFileDescripter, (struct sockaddr*)&addr_con, sizeof(addr_con)) == 0) 
		printf("\nSuccessfully binded!\n"); 
	else
		printf("\nBinding Failed!\n"); 

	while (1) { 
		printf("\n............Wait.................\n"); 

		// receive file name 
		bufferClear(net_buf);
		bufferClear(ip); 

		int nBytes = recvfrom(socketFileDescripter, net_buf, 
						32, flagrc, 
						(struct sockaddr*)&addr_con, &lengthOfAdress); 
		printf("DNS recv\n");

	
		hostname_to_ip(net_buf , ip);//{printf("\n!!!!!!!!!!!!addr not found !!!!!!!!!1\n")
		//printf("%s\n",ip );
		bufferClear(net_buf);
           int i;
          for(i=0;ip[i]!='\0';i++)
            net_buf[i]=ip[i];

		net_buf[i]=EOF;

		
			sendto(socketFileDescripter, net_buf , 32, 
				flagrc, 
				(struct sockaddr*)&addr_con, lengthOfAdress); 
			
			bufferClear(ip);
		
	} 
	return 0; 
} 
