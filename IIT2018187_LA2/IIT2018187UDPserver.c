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

#define IP_PROTOCOL 0 
#define PORT_NO 5000 
#define cipherKey 'S' 
#define flagrc 0 

void bufferClear(char* b) 
{ 
	int i; 
	for (i = 0; i < 32; i++) 
		b[i] = '\0'; 
} 

// function to encrypt 
char Cipher(char ch) 
{ 
	return ch ^ cipherKey; 
} 

// function sending file 
int sendFile(FILE* fp, char* buf, int s) 
{ 
	int i, len; 
	if (fp == NULL) { 
		strcpy(buf, "########please give me correct name########"); 
		len = strlen("########please give me correct name########"); 
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

// driver code 
int main() 
{ 
	
	struct sockaddr_in addr_con; 
	int lengthOfAdress = sizeof(addr_con); 

	addr_con.sin_family = AF_INET; 

	addr_con.sin_port = htons(PORT_NO); 

	addr_con.sin_addr.s_addr = INADDR_ANY; 

	char net_buf[32]; 
	
	FILE* fp; 

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

		int nBytes = recvfrom(socketFileDescripter, net_buf, 
						32, flagrc, 
						(struct sockaddr*)&addr_con, &lengthOfAdress); 

		fp = fopen(net_buf, "r"); 
		printf("\nFile : %s\n", net_buf); 
		if (fp == NULL) 
			printf("\n!!!!!!!!!!!!1File can't open !!!!!!!!!1\n"); 
		else
			printf("\nFile Successfully extracted!\n"); 

		while (1) { 

			// process 
			if (sendFile(fp, net_buf, 32)) { 
				sendto(socketFileDescripter, net_buf, 32, 
					flagrc, 
					(struct sockaddr*)&addr_con, lengthOfAdress); 
				break; 
			} 

			// send 
			sendto(socketFileDescripter, net_buf, 32, 
				flagrc, 
				(struct sockaddr*)&addr_con, lengthOfAdress); 
			bufferClear(net_buf); 
		} 
		if (fp != NULL) 
			fclose(fp); 
	} 
	return 0; 
} 
