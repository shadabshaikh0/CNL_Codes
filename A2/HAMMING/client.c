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

        int data[20] = {0} , ackData[20] = {0};
        int dataatrec[10], c, c1, c2, c3;

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

        bzero(data, 20);
        printf("Enter 7 bits of data one by one\n");
        scanf("%d", &data[0]);
        scanf("%d", &data[1]);
        scanf("%d", &data[2]);
        scanf("%d", &data[4]);
        scanf("%d", &data[5]);
        scanf("%d", &data[6]);
        scanf("%d", &data[8]);

        data[10] = data[0] ^ data[2] ^ data[4] ^ data[6] ^ data[8] ^ data[10];
        data[9] = data[0] ^ data[1] ^ data[4] ^ data[5] ^ data[8] ^ data[9];
        data[7] = data[0] ^ data[1] ^ data[2] ^ data[8];
        data[3] = data[0] ^ data[1] ^ data[2] ^ data[4] ^ data[5] ^ data[6];

        printf("\nEncoded data is\n");
        for (i = 0; i < 11; i++)
                printf("%d", data[i]);

        write(listensocket, data, sizeof(data));

        read(listensocket, ackData, 10);
        printf("\nackData :%d", ackData[0]);

        if (ackData[0] == 0)
        {
                printf("SUCCESSFUL! No Error!");
        }
        else
        {
                printf("Error!");
        }

        close(listensocket);
}
