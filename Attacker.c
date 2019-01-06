#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<arpa/inet.h>
#include<netdb.h>
#include <sys/types.h>  //to include number of data types to include in system calls
#include <sys/socket.h>  //includes number of definitions of structures needed for sockets
#include <netinet/in.h>  // contains constants and structures needed for internet domain address



//Declaring needed variables
int sockfd, newsockfd, portno;  // socket file descriptor, new file descriptor, Port number
char buffer[255]; 
int flag; 		//to store messages

struct sockaddr_in serv_addr, cli_addr;  // sockaddr_in is a structure containing an internet address. This structure is defined in <netinet/in.h>.


//The variable serv_addr will contain the address of the server, and cli_addr will contain the address of the client which connects to the server.




socklen_t clilen;

    

void error(const char *);
void listening();
void shell();




//Passively listening for active connections, and if accepts return 0 then calls the chat(); function
void listening() {
	



	/*

	backlogs - the manimum number of connections that a system can handle at a single time 

	*/
	listen(sockfd, 4);                       // maximum number of backlogs is 4
	char sZhostName[255];
        gethostname(sZhostName,255);
        struct hostent *host_entry;
        host_entry = gethostbyname(sZhostName);


        printf("Listening on ip %s and port %d\n", inet_ntoa (*(struct in_addr *)*host_entry->h_addr_list), ntohs(serv_addr.sin_port));

	clilen = sizeof(cli_addr);

	// Accepting incoming connections
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if (newsockfd < 0) {
		error("Error on accept");
	} else 
		printf("<=============================================>\n");

		printf("Connection Established! Shell has been gained\n");

	shell();
}

// Transmitting messages one at a time
void shell() {

	flag =0;
	while (1) {
		printf(">> ");
		// Clearing chat buffer
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);
		int n;
		n = write(newsockfd, buffer, strlen(buffer));

		if(strncmp("quit",buffer,4) == 0){
		break;
				
		}
		bzero(buffer,255);
		// recieving the incoming message
		n = read(newsockfd, buffer, 255);
		if (n < 0) {
			error("Error on reading");
		}
		
		printf("%s\n", buffer);		
			
			
	}
}
// Created a fucntion just to print custom error messages.
void error(const char *msg) {
	perror(msg);   //perror - interprets the value of error(which is returned in integer) and prints message provided in the argument	
	exit(1);
}




int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr,"Fail to enter Port Number. Program Terminated\n");
		exit(1);
	}
	
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);  // socket is initialized to use IPv4 TCP protocols
	if (sockfd < 0) {
		error("Error opening Socket.");
	}

	bzero((char *) &serv_addr, sizeof(serv_addr)); //bzero- clears anything which is in reference to (here serv_addr), so that we dont have any error
	
	portno = atoi(argv[1]); //atoi- converts the string provided in the argument into integer

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	/*

	bind() - assigns the address specified by the addr to the socket by the file descriptor


	*/
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)  
		error("Binding Failed");

	// Initial call to listen
	listening();

	// Closing all the sockets
	close(newsockfd);
	close(sockfd);
	return 0;

}
