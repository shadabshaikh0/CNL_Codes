#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

#define PORT 2000
#define SERVER_ADDRESS "127.0.0.1"

int main(int argc, char const *argv[])
{
    struct sockaddr_in server;

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
    server.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

    int status = connect(sock, (struct sockaddr *)&server, sizeof(server));
    if (status == 0)
    {
        cout << "Connection Successful" << endl;
    }
    else
    {
        cout << "Error while creating Connection " << endl;
    }
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
                bzero((char *)buffer, 256);
                cout << " > ";
                string data;
                getline(cin, data);
                if (data == "bye")
                    break;
                strcpy(buffer, data.c_str());
                send(sock, buffer, strlen(buffer), 0);
                bzero(buffer, 256);
                cout << "Awaiting server response..." << endl;
                recv(sock, (char *)&buffer, sizeof(buffer), 0);
                cout << "Server: " << buffer << endl;
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
            msg_len = send(sock, filename, 100, 0);
            if (msg_len == -1)
                cout << "Error in filename" << endl;

            fout.open(filename, ios::in | ios::out | ios::binary);
            if (!fout.is_open())
            {
                cout << "Error while opening file" << endl;
            }
            fout.seekg(0, ios::end);
            int filesize = fout.tellg();
            msg_len = send(sock, (void *)&filesize, sizeof(filesize), 0);

            char *filebuff = new char[filesize];
            fout.seekg(0, ios::beg);
            fout.read(filebuff, filesize);

            msg_len = send(sock, filebuff, filesize, 0);
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

            msg_len = recv(sock, filename, 100, 0);
            cout << "filename : " << filename << endl;

            int filesize = 0;
            msg_len = recv(sock, (void *)&filesize, sizeof(filesize), 0);
            cout << "Filesize: " << filesize << endl;

            char *filebuff = new char[filesize];
            bzero((char *)filebuff, strlen(filebuff));
            msg_len = recv(sock, filebuff, filesize, 0);
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
            int n;
            char a[20], b[20], c[20], ans[20];
            while (1)
            {
                cout << "\nEnter First Number\n";
                cin >> a;
                send(sock, a, strlen(a), 0);
                bzero((char *)a, sizeof(a));

                cout << "\nEnter Second Number\n";
                cin >> b;
                send(sock, b, strlen(b), 0);
                bzero((char *)b, sizeof(b));

                cout << "\nEnter Operator\n";
                cin >> c;
                send(sock, c, strlen(c), 0);
                bzero((char *)c, sizeof(c));

                recv(sock, ans, 20, 0);
                cout << "Result:" << (float)atof(ans);
                bzero((char *)ans, sizeof(ans));
            }
            break;
        }
        case 5:
            break;
        }

    } while (options != 0);

    return 0;
}
