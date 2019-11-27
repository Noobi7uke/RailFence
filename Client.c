#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <netdb.h> 

#include "cypher.h"

void error(const char *buffer,int sockfd)
{
	printf("\nERROR  - %s",buffer);
	if(sockfd!=-1)
		close(sockfd);
	exit(1);
}


void parse_arguments(int argc,char** argv,struct sockaddr_in* addr){
	if(argc<3)
		error("Usage <program_name> <host_address> <port>",-1);
	
	int PORT = atoi(argv[2]);
	struct hostent *server  = gethostbyname(argv[1]);
	bzero((char*)addr,sizeof(struct sockaddr_in));
	addr->sin_family = AF_INET; // Protocol Family
	addr->sin_port = htons(PORT);
	bcopy((char*)server->h_addr,(char*)&(addr->sin_addr.s_addr),server->h_length);
}


int main (int argc, char* argv[])
{

    char buffer[256] ;
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(client_socket==-1)
        error(" Failed to obtain Socket Descriptor.\n",client_socket);
    else
		printf("[Client] Obtaining socket descriptor successfully.\n");
    
        
	struct sockaddr_in server_addr;
	
	/* Fill the client socket address struct */
	parse_arguments(argc, argv,&server_addr);

	/* Bind a special Port */
	if( connect(client_socket, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1 )
	{
	    error("Failed to bind Port.\n",client_socket);
	}
	
	int n;

	char* server_name = argv[1];
	int server_port = ntohs(server_addr.sin_port);

	printf("[Client] Connected tcp port %d in addr [%s] sucessfully.\n",server_port,server_name);


	printf("[Client] Enter data to send to server - ");
	scanf("%[^\n]s",buffer);
    char* encrypted_msg = encrypt(buffer,strlen(buffer),4);
    strcpy(buffer,encrypted_msg);
	n = write(client_socket,buffer,strlen(buffer),0);
    printf("[Client] Encrypted data to sent to server - %s\n",buffer);
	bzero(buffer,256);
	printf("[Client] Waiting for server to send data .........\n");
	n = read(client_socket,buffer,256,0);
    printf("[Client] Encrypted Message recieved from server - %s\n",buffer);
    char* decrypted_msg = decrypt(buffer,strlen(buffer),4);
    strcpy(buffer,decrypted_msg);
    printf("[Client] Decrypted Message recieved from server - %s\n\n",buffer);

	bzero(buffer,256);
	
	getchar();
    
    free(encrypted_msg);
    free(decrypted_msg);
    close(client_socket);
		
    return 0;
}   