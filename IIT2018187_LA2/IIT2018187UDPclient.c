/*
submitted bby Abhishek(IIT2018187)
*/
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 

#define IP_PROTOCOL 0 
#define IP_ADDRESS "127.0.0.1" // localhost 
#define PORT_NO 5000 
#define KeyC 'S' 
#define flagrc 0 

// function to clear buffer 
void bufferClear(char* b) 
{ 
	int i; 
	for (i = 0; i < 32; i++) 
		b[i] = '\0'; 
} 

// function for decryption 
char Cipher(char ch) 
{ 
	return ch ^ KeyC; 
} 

// function to receive file 
int recvFile(char* buf, int s) 
{ 
	int i; 
	char ch; 
	for (i = 0; i < s; i++) { 
		ch = buf[i]; 
		ch = Cipher(ch); 
		if (ch == EOF) 
			return 1; 
		else
			printf("%c", ch); 
	} 
	return 0; 
} 

// driver code 
int main() 
{ 
	 
	struct sockaddr_in addr_con; 

	int lengthOfAdress = sizeof(addr_con); 

	addr_con.sin_family = AF_INET; 

	addr_con.sin_port = htons(PORT_NO); 

	addr_con.sin_addr.s_addr = inet_addr(IP_ADDRESS); 

	char buffer_net[32]; 

	FILE* filepointer; 

	// socket() 
	int socketFileDescripter = socket(AF_INET, SOCK_DGRAM, 
					0); 

	if (socketFileDescripter < 0) 
		printf("\ndescriptor not found!!!\n"); 
	else
		printf("\ndescripter  %d found \n", socketFileDescripter); 

	while (1) { 
		printf("\ngive me name of file\n"); 
		scanf("%s", buffer_net); 
		sendto(socketFileDescripter, buffer_net, 32, 
			flagrc, (struct sockaddr*)&addr_con, 
			lengthOfAdress); 

		printf("\n##############  Data Extracting  #############\n"); 

		while (1) { 
			// receive 
			bufferClear(buffer_net); 
			int nBytes = recvfrom(socketFileDescripter, buffer_net, 32, 
							flagrc, (struct sockaddr*)&addr_con, 
							&lengthOfAdress); 

			// process 
			if (recvFile(buffer_net, 32)) { 
				break; 
			} 
		} 
		printf("\n!!!!!!!!!!!!!!!!    end  !!!!!!!!!!!!!!!!!!!!!!!\n"); 
	} 
	return 0; 
} 
