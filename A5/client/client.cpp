#include <iostream>
#include <unistd.h>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;

#define PORT 1234
#define SERVER_ADDRESS "127.0.0.1"

int main(int argc, char const *argv[])
{
    struct sockaddr_in server;
    cout << "\nUDP Client\nNote:-For receiving data, First Select Receive Option And then Select Send Option" << endl;

    int sock;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        cout << "\n Error while socket creating\n";
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

    socklen_t serlen = sizeof(server);
    int serverlen = sizeof(server);

    char buffer[1000];
    cout << endl;
    cout << "Enter Msg to server: ";
    cin.getline(buffer, sizeof(buffer));

    int msg_len = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server, serverlen);
    if (msg_len == -1)
    {
        cout << "\n Error while sending message\n";
        exit(1);
    }

    int options;
    do
    {
        cout << "\n1.send file \n2.receive file \n0.exit" << endl;
        cout << "Enter option:";
        cin >> options;
        switch (options)
        {
        case 1:
        {
            char filename[100];
            int msg_len;
            bzero((char *)filename, strlen(buffer));
            cout << "Enter filename: ";
            cin >> filename;
            cout << "Your selected filename : " << filename << endl;
            msg_len = sendto(sock, filename, strlen(filename), 0, (struct sockaddr *)&server, serverlen);
            if (msg_len == -1)
            {
                cout << "\n Error while sending file\n";
                exit(1);
            }

            fstream file;
            file.open(filename, ios::in | ios::out | ios::binary);
            if (!file.is_open())
            {
                cout << "Error while opening filename" << endl;
                exit(1);
            }
            file.seekg(0, ios::end);
            int filesize = file.tellg();

            cout << "Your Filesize  : " << filesize << endl;
            msg_len = sendto(sock, (void *)&filesize, sizeof(filesize), 0, (struct sockaddr *)&server, serverlen);
            if (msg_len == -1)
            {
                cout << "\n Error while sending filesize\n";
                exit(1);
            }

            if (filesize > 65536)
            {
                int interval = 0;
                interval = filesize / 65000;
                ++interval;
                char *filebuffer;
                for (int i = 0; i < interval; i++)
                {
                    filebuffer = new char[65000];
                    bzero((char *)filebuffer, sizeof(filebuffer));
                    file.seekg(i * 65000, ios::beg);
                    file.read(filebuffer, 65000 - 1);
                    msg_len = sendto(sock, filebuffer, strlen(filebuffer), 0, (struct sockaddr *)&server, serverlen);
                    cout << "Msg length :" << msg_len << endl;
                    if (msg_len == -1)
                    {
                        cout << "\n Error while sending file\n";
                        exit(1);
                    }
                }
                file.close();
            }
            else
            {
                char *filebuffer = new char[filesize];
                bzero((char *)filebuffer, sizeof(filebuffer));
                file.seekg(0, ios::beg);
                file.read(filebuffer, filesize);
                file.close();
                msg_len = sendto(sock, filebuffer, filesize, 0, (struct sockaddr *)&server, serverlen);
                if (msg_len == -1)
                {
                    cout << "\n Error while sending file\n";
                    exit(1);
                }
            }
            break;
        }
        case 2:
        {
            char filename[100];
            int msg_len;
            bzero((char *)filename, strlen(filename));
            msg_len = recvfrom(sock, filename, 100, 0, (struct sockaddr *)&server, &serlen);
            if (msg_len > 0)
            {
                cout << "filename : " << filename << endl;
            }
            else
            {
                cout << "Error in filename block" << endl;
            }
            int filesize = 0;
            msg_len = recvfrom(sock, (void *)&filesize, sizeof(filesize), 0, (struct sockaddr *)&server, &serlen);
            if (msg_len > 0)
            {
                cout << "filesize : " << filesize << endl;
            }
            else
            {
                cout << "Error in filesize block" << endl;
            }

            if (filesize > 65536)
            {
                char *tempfilebuffer = new char[filesize];
                bzero((char *)tempfilebuffer, sizeof(tempfilebuffer));

                int interval = filesize / 65000;
                ++interval;

                char *filebuffer;
                for (int i = 0; i < interval; i++)
                {
                    filebuffer = new char[65000];
                    bzero((char *)filebuffer, sizeof(filebuffer));
                    msg_len = recvfrom(sock, filebuffer, 65000, 0, (struct sockaddr *)&server, &serlen);
                    cout << "Msg Length : " << msg_len << endl;
                    sprintf(tempfilebuffer, "%s%s", tempfilebuffer, filebuffer);
                }

                ofstream fout;
                fout.open(filename, ios::out | ios::binary);
                if (!fout.is_open())
                {
                    cout << "Error while opening filename" << endl;
                    exit(1);
                }
                fout.write(tempfilebuffer, filesize);
                fout.close();
                cout << "file received" << endl;
            }
            else
            {
                char *filebuffer = new char[filesize];

                msg_len = recvfrom(sock, filebuffer, filesize, 0, (struct sockaddr *)&server, &serlen);
                if (msg_len > 0)
                {
                    ofstream fout;
                    fout.open(filename, ios::out | ios::binary);
                    if (!fout.is_open())
                    {
                        cout << "Error while opening filename" << endl;
                        exit(1);
                    }
                    fout.write(filebuffer, filesize);
                    fout.close();
                    cout << "file received" << endl;
                }
                else
                {
                    cout << "Error in file content" << endl;
                }
            }
            break;
        }
        case 3:
            exit(0);
            break;
        default:
            cout << "You Entered Wrong Option" << endl;
        }

    } while (options != 0);

    return 0;
}
