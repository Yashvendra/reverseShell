#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <fstream>

#define PORT 8080
#define MAXLINE 1024

using namespace std;

int main(int argc, char const *argv[]) 
{ 
	// Declaring the variables.
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char buffer[MAXLINE] = {0}; 
    char new_buffer[MAXLINE] = {0};
    char message[MAXLINE] = {0};
    string data;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 

    while(1)
    {
        cout<<"@yashBash>> ";

        getline(cin,data);

        if(data.empty()) 
        {
            continue;
        }

        strcpy(new_buffer,data.c_str());
        char *dummy, *token;
        dummy = new_buffer;
        token = strtok(dummy," ");
   	    send(sock, data.c_str(), strlen(data.c_str()), 0);
        if(strcmp(token,"cd") == 0) 
        {
    	   valread = read(sock,message,MAXLINE);
    	   if(strcmp(message,"1") == 0)
    	   {
    	        cout<<"Directory successfully changed"<<endl;
    	   }
    	   else if(strcmp(message,"0") == 0)
    	   {
    	        cout<<"Error in changing directory"<<endl;
    	   }
           cout.flush();
        }
        else if(strcmp(new_buffer,"dir") == 0)
        {
    	   while(1) 
    	   {
    	       valread = read(sock,message,MAXLINE);
    	       if(strcmp(message,"1") == 0)
    	       { 
    	           read(sock,message,MAXLINE);
    	           cout<<message;
    	       }
               else break;
    	   }
        }
        else if(strcmp(token,"curr") == 0) 
        {
    	   valread = read(sock,message,MAXLINE);
    	   cout<<"Current working directory is : "<<message<<endl;
        }
        else 
        {
    	   cout<<"Please enter a valid command !"<<endl;
        }
    }
    return 0; 
} 
