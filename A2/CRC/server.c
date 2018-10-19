#include <stdlib.h>
#include <netinet/in.h> 
#include <time.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>      
#include <sys/types.h>  
#include <sys/socket.h> 

int main(int argc, char *argv[])
{
	int listensocket, i, connsd, size;
	struct sockaddr_in serversocket, clientsocket;
	char recvbuff[100];
	int j, keylen, msglen;
	char input[100] = {"100000001"}, key[30], temp[30], quot[100], rem[30], key1[30];
	char input1[100];

	if (argc < 2)
	{
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}

	listensocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listensocket < 0)
		printf("Error while creating socket");
	else
		printf("Socket Created Succesfully");

	bzero((char *)&serversocket, sizeof(serversocket));

	serversocket.sin_family = AF_INET;
	serversocket.sin_port = htons(atoi(argv[1]));
	serversocket.sin_addr.s_addr = INADDR_ANY;

	i = bind(listensocket, (struct sockaddr *)&serversocket, sizeof(serversocket));
	
	if (i == 0)
		printf("\nBound socket Succesfully\n");
	else
		printf("\nError while binding to socket\n");

	i = listen(listensocket, 1);
	if (i == 0)
		printf("Successfully listened\n");
	else
		printf("Error while listening\n");

	size = sizeof(struct sockaddr_in);

	connsd = accept(listensocket, (struct sockaddr *)&clientsocket, &size);
	bzero(recvbuff, 100);
	read(connsd, recvbuff, 100);

	printf("\nMessage from the client : %s\t", input);

	strcpy(input1, recvbuff);

	printf("\nEnter Key: ");
	gets(key);

	keylen = strlen(key);
	msglen = strlen(input);

	strcpy(key1, key);

	for (i = 0; i < keylen; i++)
		temp[i] = input[i];

	for (i = 0; i < msglen - keylen + 1; i++)
	{
		quot[i] = temp[0];
		if (quot[i] == '0')
			for (j = 0; j < keylen; j++)
				key[j] = '0';
		else
			for (j = 0; j < keylen; j++)
				key[j] = key1[j];

		for (j = keylen - 1; j > 0; j--)
		{
			if (temp[j] == key[j])
				rem[j - 1] = '0';
			else
				rem[j - 1] = '1';
		}
		printf("\nRemainder : %s\t", rem);

		rem[keylen - 1] = input[i + keylen];
		strcpy(temp, rem);
	}

	strcpy(rem, temp);
	printf("\nQuotient is ");
	for (i = 0; i < msglen; i++)
		printf("%c", quot[i]);

	printf("\nRemainder is ");
	for (i = 0; i < keylen - 1; i++)
		printf("%c", rem[i]);

	close(connsd);
	close(listensocket);
}
