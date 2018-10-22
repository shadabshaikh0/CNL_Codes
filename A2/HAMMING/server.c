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
    int ackBuffer[20];
    int buffer[256];

    int dataatrec[20], c, c1, c2, c3,c4;

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

    bzero(buffer, 256);
    read(connsd, buffer, 255);

    printf("\nMessage from the client : ");
    for (int j = 0; j < 11; j++)
        printf("%d", buffer[j]);

    for (i = 0; i < 11; i++)
        dataatrec[i] = buffer[i];

    c1 = dataatrec[0] ^ dataatrec[2] ^ dataatrec[4] ^ dataatrec[6] ^ dataatrec[8] ^ dataatrec[10];
    c2 = dataatrec[0] ^ dataatrec[1] ^ dataatrec[4] ^ dataatrec[5] ^ dataatrec[8] ^ dataatrec[9];
    c3 = dataatrec[0] ^ dataatrec[1] ^ dataatrec[2] ^ dataatrec[8];
    c4 = dataatrec[0] ^ dataatrec[1] ^ dataatrec[2] ^ dataatrec[4] ^ dataatrec[5] ^ dataatrec[6];

    c = c4 *8 + c3 * 4 + c2 * 2 + c1;

    if (c == 0)
    {
        bzero(ackBuffer, 10);
        ackBuffer[0] = c;
        write(connsd, ackBuffer, sizeof(ackBuffer));
    }
    else
    {
        bzero(ackBuffer, 10);
        ackBuffer[0] = c;
        write(connsd, ackBuffer, sizeof(ackBuffer));
        printf("\nError on position %d", c);

        printf("\nCorrect message is\n");

        if (dataatrec[11 - c] == 0)
            dataatrec[11 - c] = 1;
        else
            dataatrec[11 - c] = 0;

        for (i = 0; i < 11; i++)
            printf("%d", dataatrec[i]);
    }
    close(connsd);
    close(listensocket);
}
