/*
Command Line Arguments:

filename server_ipAddress portNumber

argv[0] = filename 
argv[1] = server_ipAddress
argv[2] = portNumber
 

*/




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // for read(), write() and close()
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>  //for hostenet structure

int sockfd, portno;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[255];
	


void error(const char *msg) {
	perror(msg);
	exit(1);
}

void chat(){

	while (1) {
		
		// Clearing chat buffer
		bzero(buffer, 255);
		
		int n = read(sockfd, buffer, 255);	
		if (n < 0) {
			error("Error on reading");
		}
		
		if (strncmp("curr", buffer, 4) == 0)	{
			if(getcwd(buffer, sizeof(buffer)) != NULL){
			write(sockfd, buffer, strlen(buffer));
		 }								
			
		}
		else if(strncmp("quit", buffer, 4) == 0){
			
			
			break;
			
		}
		else if(strncmp("who", buffer, 3) == 0){
			if(getenv("USER") !=NULL)
   			write(sockfd,getenv("USER"), strlen(getenv("USER")));
		}
		else if(strncmp("cd", buffer, 2) == 0){
			char path[255];
			int p=3,d=0;
			while(p<sizeof(buffer)){
			path[d++] = buffer[p++];
			}		
			
			int o = chdir(path);
			write(sockfd, path, strlen(path));			
		}
		else {
		
			char *error = (char *)"Command not found";
			write(sockfd, error, strlen(error));		
		}
		bzero(buffer, 255);
		
}
}


int main(int argc, char *argv[]) {
	
	
	if (argc < 3) {
		fprintf(stderr,"usage %s hostname port\n",argv[0]);
		exit(1);
	}

	portno = atoi(argv[2]);
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);  // telling socket to use IPv4 protocols
	if (sockfd < 0) {
		error("Error opening Socket.");
	}

	// Getting server address from raw address
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr, "Error, no such host");
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	// sending request to accept the connection to the server
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		error("Connetion Failed");
	} else {
		printf("Connection Established!\n");
	}
	
	chat();

	// Closing the socket
	close(sockfd);
	return 0;

}
