#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <netdb.h>

#include "cypher.h"

void error(const char *msg)
{
	printf("\nERROR  - %s",msg);
	exit(1);
}
void error_close(const char *msg,int sockfd){
	close(sockfd);
	error(msg);
}

void parse_arguments(int argc,char** argv, struct sockaddr_in* addr){
	if(argc!=2)
		error("Usage <program_name> <port>");
	
	bzero((char*)addr,sizeof(struct sockaddr_in));
	addr->sin_family = AF_INET; // Protocol Family
	int port = atoi(argv[1]);
	addr->sin_port = htons(port);
	addr->sin_addr.s_addr = INADDR_ANY; // AutoFill local address
}

int main (int argc, char* argv[])
{
	/* Defining Variables */

	char buffer[256];
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(server_socket==-1)
        error(" Failed to obtain Socket Descriptor.\n");
    else
		printf("[Server] Obtaining socket descriptor successfully.\n");
    

	struct sockaddr_in server_addr, client_addr;

	/* Fill the client socket address struct */
	parse_arguments(argc,argv,&server_addr);


	/* Bind a special Port */
	if( bind(server_socket, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1 )
	{
	    error("Failed to bind Port.\n");
	}
	else 
		printf("[Server] Binded tcp port %d sucessfully.\n",ntohs(server_addr.sin_port));

	/* Listen remote connect/calling */
	if(listen(server_socket,1) == -1)
	{
	    error_close("ERROR: Failed to listen Port.\n",server_socket);
	}
	else{
		printf ("[Server] Listening the port %d successfully.\n", ntohs(server_addr.sin_port));
	

		bzero((char*)&client_addr,sizeof(client_addr));
    	int client_socket,sin_size,n;
		printf("[Server] Waiting for client to connect........\n");

		/* Wait a connection, and obtain a new socket file despriptor for single connection */		
		client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &sin_size);

		if (client_socket < 0)
		    error("[Server] ERROR: Obtaining new Socket Despcritor.\n");
		
		char* client_name;
		if(client_addr.sin_addr.s_addr==0)
			client_name = "localhost";
		else
			client_name = gethostbyaddr((void *)&client_addr.sin_addr.s_addr,sizeof(client_addr.sin_addr.s_addr),AF_INET)->h_name;
		int client_port = ntohs(client_addr.sin_port);

		printf("[Server] Connected to [%s:%d]\n",client_name,client_port);
		printf("[Server] Waiting for [%s:%d] to send data........\n",client_name,client_port);
		n = read(client_socket,buffer,255,0);
		printf("[Server] Encrypted Data from [%s:%d] - %s\n",client_name,client_port,buffer);
        char* decrypted_msg = decrypt(buffer,strlen(buffer),4);
        strcpy(buffer,decrypted_msg);
        printf("[Server] Data after decryption from [%s:%d] - %s\n",client_name,client_port,buffer);
        
		printf("[Server] Enter data to send to [%s:%d] - ",client_name,client_port);
		scanf("%[^\n]s",buffer);
        char* encrypted_msg = encrypt(buffer,strlen(buffer),4);
        strcpy(buffer,encrypted_msg);
    	n = write(client_socket,buffer,sizeof(buffer),0);
        printf("[Server] Encrypted data sent to [%s:%d] - %s\n\n",client_name,client_port,buffer);

        free(encrypted_msg);
        free(decrypted_msg);
		close(client_socket);
	}
	close(server_socket);	
    return 0;
}   