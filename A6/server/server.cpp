#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;

#define PORT 2000

int main(int argc, char const *argv[])
{
    struct sockaddr_in server, client;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        cout << "Error while creating socket" << endl;
    }
    else
    {
        cout << "Socket Successfully Created" << endl;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        cout << "Error while binding" << endl;
    }
    if (listen(sock, 10) < 0)
    {
        cout << "Error while listening" << endl;
    }

    socklen_t clientlen = sizeof(client);
    int newsocket = accept(sock, (struct sockaddr *)&client, &clientlen);

    int options;
    do
    {
        cout << "\n1.Chat\n2.send filen\n3.receive file \n4.calculator(Arithmetic)\n5.calculator(Trignometric)" << endl;
        cout << "Enter Options :";
        cin >> options;

        switch (options)
        {
        case 1:
        {
            cin.ignore();
            char buffer[256];
            while (1)
            {
                cout << "Awating client response" << endl;
                bzero(buffer, 256);
                recv(newsocket, buffer, 256, 0);
                cout << "Client: " << buffer << endl;
                cout << " > ";
                string data;
                getline(cin, data);
                bzero(buffer, 256);
                strcpy(buffer, data.c_str());
                fflush(stdin);
                send(newsocket, (char *)&buffer, strlen(buffer), 0);
            }
            break;
        }
        case 2:
        {
            long int msg_len;
            char filename[100];

            bzero((char *)filename, strlen(filename));
            cout << "Enter Filename:";
            cin >> filename;
            cin.get();

            fstream fout;
            msg_len = send(newsocket, filename, 100, 0);
            if (msg_len == -1)
                cout << "Error in filename" << endl;

            fout.open(filename, ios::in | ios::out | ios::binary);
            if (!fout.is_open())
            {
                cout << "Error while opening file" << endl;
            }
            fout.seekg(0, ios::end);
            int filesize = fout.tellg();
            msg_len = send(newsocket, (void *)&filesize, sizeof(filesize), 0);

            char *filebuff = new char[filesize];
            fout.seekg(0, ios::beg);
            fout.read(filebuff, filesize);

            msg_len = send(newsocket, filebuff, filesize, 0);
            if (msg_len == -1)
                cout << "Error in file Transmission" << endl;
            else
                cout << "Transmission Successful";
            fout.close();
        }
        break;
        case 3:
        {
            long int msg_len;
            char buffer[256];
            char filename[100];
            bzero((char *)filename, strlen(filename));

            msg_len = recv(newsocket, filename, 100, 0);
            cout << "filename : " << filename << endl;

            int filesize = 0;
            msg_len = recv(newsocket, (void *)&filesize, sizeof(filesize), 0);
            cout << "Filesize: " << filesize << endl;

            char *filebuff = new char[filesize];
            bzero((char *)filebuff, strlen(filebuff));
            msg_len = recv(newsocket, filebuff, filesize, 0);
            ofstream fout;
            fout.open(filename, ios::out | ios::binary);
            if (!fout.is_open())
                cout << "Error in file opening" << endl;
            else
            {
                fout.write(filebuff, filesize);
                fout.close();
                cout << "File received" << endl;
            }
        }

        break;
        case 4:
        {
            float number1, number2, answer;
            char _operator[2], num1[20], num2[20];
            bzero((char *)num1, sizeof(num1));
            bzero((char *)num2, sizeof(num2));

            while (1)
            {
                recv(newsocket, num1, 20, 0);
                cout << "\nThe first number is " << num1 << endl;
                number1 = atof(num1);
                bzero((char *)num1, sizeof(num1));

                recv(newsocket, num2, 20, 0);
                cout << "\nThe second number is " << num2 << endl;
                number2 = atof(num2);
                bzero((char *)num2, sizeof(num2));

                recv(newsocket, _operator, 2, 0);
                cout << "\nThe operator  is " << _operator << endl;

                switch (_operator[0])
                {
                case '+':
                {
                    char ans[20];
                    answer = number1 + number2;
                    bzero((char *)ans, sizeof(ans));
                    sprintf(ans, "%f", answer);
                    send(newsocket, ans, strlen(ans), 0);
                    break;
                }
                case '-':
                {
                    char ans[20];
                    answer = number1 - number2;
                    bzero((char *)ans, sizeof(ans));
                    sprintf(ans, "%f", answer);
                    send(newsocket, ans, strlen(ans), 0);
                    break;
                }
                case '*':
                {
                    char ans[20];
                    answer = number1 * number2;
                    bzero((char *)ans, sizeof(ans));
                    sprintf(ans, "%f", answer);
                    send(newsocket, ans, strlen(ans), 0);
                    break;
                }
                case '/':
                {
                    char ans[20];
                    answer = number1 / number2;
                    bzero((char *)ans, sizeof(ans));
                    sprintf(ans, "%f", answer);
                    send(newsocket, ans, strlen(ans), 0);
                    break;
                }
                }
                bzero((char *)_operator, sizeof(_operator));
            }
            break;
        }
        case 5:
            break;
        }

    } while (options != 0);
    close(sock);
    return 0;
}
