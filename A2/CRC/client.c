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
	struct sockaddr_in serversocket;
	int listensocket, i, connsd, size;
	char sendbuff[100];
	int j, keylen, msglen;
	char input[100], key[30], temp[30], quot[100], rem[30], key1[30];

	if (argc < 3)
	{
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(0);
	}

	bzero((char *)&serversocket, sizeof(serversocket));
	listensocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listensocket < 0)
		printf("Error while creating Socket ");
	else
		printf("Socket Created Succesfully");

	serversocket.sin_family = AF_INET;
	serversocket.sin_port = htons(atoi(argv[2]));
	serversocket.sin_addr.s_addr = INADDR_ANY;
	i = connect(listensocket, (struct sockaddr *)&serversocket, sizeof(serversocket));

	if (i == 0)
		printf("\nConnected successfully\n");
	else
		printf("\nError while creating connection\n");

	printf("Enter Data: ");
	gets(input);

	printf("Enter Key: ");
	gets(key);

	keylen = strlen(key);
	msglen = strlen(input);

	strcpy(key1, key);

	for (i = 0; i < keylen - 1; i++)
	{
		input[msglen + i] = '0';
	}

	for (i = 0; i < keylen; i++)
		temp[i] = input[i];

	for (i = 0; i < msglen; i++)
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

	bzero(sendbuff, 100);
	printf("\nFinal data is: ");
	for (i = 0, j = 0; i < msglen; j++, i++)
	{
		printf("%c", input[i]);
		sendbuff[j] = input[i];
	}
	for (i = 0, j = msglen; i < keylen - 1; j++, i++)
	{
		printf("%c", rem[i]);
		sendbuff[j] = rem[i];
	}

	printf("\nWriting the Final data...\n");
	write(listensocket, sendbuff, strlen(sendbuff));

	close(listensocket);
}
