#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <fstream>
#include <errno.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAXLINE 1024

using namespace std;
 
int main() 
{
	// Declare variables.
	int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int max_clients = 30, sd, activity;
    int client_socket[max_clients] = {0};
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[MAXLINE] = {0}; 

    // Set of socket descriptors.
    fd_set readfds; 
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 

    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }

    // Listening on the given port for connections.
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 

    cout<<"Accepting connections ... "<<endl;

    while(1) 
    {
    	int max_sd;

    	//clear the socket set  
        FD_ZERO(&readfds);   
     
        //add master socket to set  
        FD_SET(server_fd, &readfds);   
        max_sd = server_fd;   
             
        //add child sockets to set  
        for (int i = 0 ; i < max_clients ; i++)   
        {   
            //socket descriptor  
            sd = client_socket[i];   
                 
            //if valid socket descriptor then add to read list  
            if(sd > 0)   
                FD_SET( sd , &readfds);   
                 
            //highest file descriptor number, need it for the select function  
            if(sd > max_sd)   
                max_sd = sd;   
        }   
     
        //wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely  
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   
       
        if ((activity < 0) && (errno!=EINTR))   
        {   
            cout<<"select error";   
        }   
             
        //If something happened on the master socket ,  
        //then its an incoming connection  
        if (FD_ISSET(server_fd, &readfds))   
        {   
            if ((new_socket = accept(server_fd,  
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)   
            {   
                perror("accept");   
                exit(EXIT_FAILURE);   
            }   
             
            //inform user of socket number - used in send and receive commands  
            cout<<"New connection , socket fd is "<<new_socket<<", ip is : "<<inet_ntoa(address.sin_addr)
            <<", port : "<<ntohs(address.sin_port)<<endl;  
                      
            //add new socket to array of sockets  
            for (int i = 0; i < max_clients; i++)   
            {   
                //if position is empty  
                if( client_socket[i] == 0 )   
                {   
                    client_socket[i] = new_socket;   
                    break;   
                }   
            }   
        }   
             
        //else its some IO operation on some other socket 
        for (int i = 0; i < max_clients; i++)   
        {   
            sd = client_socket[i];   
                 
            if (FD_ISSET( sd , &readfds))   
            {   
                //Check if it was for closing , and also read the  
                //incoming message  
                if ((valread = read( sd , buffer, 1024)) == 0)   
                {   
                    //Somebody disconnected , get his details and print  
                    getpeername(sd , (struct sockaddr*)&address, (socklen_t*)&addrlen);   
                    cout<<"Host disconnected , ip "<<inet_ntoa(address.sin_addr)<<
                    ", port "<<ntohs(address.sin_port)<<endl;   
                         
                    //Close the socket and mark as 0 in list for reuse  
                    close( sd );   
                    client_socket[i] = 0;   
                }   
                     
                //Echo back the message that came in  
                else 
                {   
                    //set the string terminating NULL byte on the end  
                    //of the data read  
                    buffer[valread] = '\0'; 
                    char *dummy, *token;
                    dummy = buffer;
                    token = strtok(dummy," ");
                    if(strcmp(token,"cd") == 0)
                    {
                    	char message[MAXLINE] = {0};
                    	// Change the directory on server side.
                    	token = strtok(NULL,"\n");
                    	cout<<"Change directory to path - "<<token<<endl;
                    	if(chdir(token)<0)
                    	{
                    		// Error occurred in changing the directory.
                    		cout<<"Error in changing directory"<<endl;
                    		strcpy(message,"0");
                    	}
                    	else 
                    	{
                    		// Changed the directory successfully.
                    		cout<<"Directory changed successfully"<<endl;
                    		strcmp(message,"1");
                    	}
	                   	send(sd , message , MAXLINE , 0 );   
                    }
                    else if(strcmp(buffer,"dir") == 0)
                    {
                    	FILE *in;
                    	// List the contents of the present directory.
                    	char temp[MAXLINE] = {0};
                    	if(!(in = popen("dir", "r")))
                    	{
                    		cout<<"Error with dir"<<endl;
                    	}
                    	while(fgets(temp,sizeof(temp),in)!=NULL)
                    	{
                    		send(sd,"1",MAXLINE,0);
                    		send(sd,temp,MAXLINE,0);
                    	}
                    	send(sd,"0",MAXLINE,0);
                    	pclose(in);
                    }
                    else if(strcmp(buffer,"curr") == 0)
                    {
                    	// Send the current working directory of the server.
                    	char curr_dir[MAXLINE]={0};
                    	getcwd(curr_dir,MAXLINE-1);
                    	send(sd,curr_dir,MAXLINE,0);
                    }
		    else if(strcmp("quit", buffer) == 0){
			
			cout<< "The client has quitted \n";
			break;
			
		}
                }   
            }   
        }
    }

    return 0;
}
