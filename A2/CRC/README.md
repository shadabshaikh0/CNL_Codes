# A cyclic redundancy check
A cyclic redundancy check (CRC) is an error-detecting code commonly used in digital networks and storage devices to detect accidental changes to raw data.

Reference: 

__Geeksforgeeks__ [click here](https://www.geeksforgeeks.org/modulo-2-binary-division/)

__SocketsTutorial__ [click here](http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html)

# code explanation
## programming language: c
## program name : client.c

__Header files and their use in program:__
> [#include <stdlib.h>](https://www.tutorialspoint.com/c_standard_library/stdlib_h.htm)

It is require for [atoi](https://www.tutorialspoint.com/c_standard_library/c_function_atoi.htm) function used for conversion purpose.
      
> [#include <netinet/in.h>](http://pubs.opengroup.org/onlinepubs/009619199/netinetinh.htm)

To make available this structure __struct sock_addr__ which includes __sin_family,sin_port,sin_addr__.

> [#include <arpa/inet.h>](http://pubs.opengroup.org/onlinepubs/7908799/xns/arpainet.h.html)

To use __htons(),htonl()__ function.

> [#include <string.h>](https://www.tutorialspoint.com/c_standard_library/string_h.htm)

To perform various operations on strings.In this code, we use __strcpy()__.

> [#include <unistd.h>](https://en.wikipedia.org/wiki/Unistd.h)

To make available  __close(),write()__ function for closing,writing to the socket.

> [#include <stdio.h>](https://www.tutorialspoint.com/c_standard_library/stdio_h.htm)

It makes available console I/O functions such as __printf() , scanf()__.

> [#include <sys/socket.h>](http://pubs.opengroup.org/onlinepubs/009696699/basedefs/sys/socket.h.html)

This header file make available __socket(),connect(),accept(),listen()__ functions. 

# Line wise code explanation

> [struct sockaddr_in serversocket;](https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html)

This structure includes 

struct sockaddr_in {

    short            sin_family;   // e.g. AF_INET
    
    unsigned short   sin_port;     // e.g. htons(3490)
    
    struct in_addr   sin_addr;     // see struct in_addr, below
    
    char             sin_zero[8];  // zero this if you want to
    
};

struct in_addr {

    unsigned long s_addr;  // load with inet_aton()
    
};

> [bzero](http://pubs.opengroup.org/onlinepubs/009696699/functions/bzero.html)((char *)&serversocket, sizeof(serversocket));

The bzero() function shall place size of structure zero-valued bytes in the area pointed to by server structure variable.

>	listensocket = [socket(AF_INET, SOCK_STREAM, 0);](http://pubs.opengroup.org/onlinepubs/009695399/functions/socket.html)

The socket() function shall create an unbound socket in a communications domain, and return a file descriptor that can be used in later function calls that operate on sockets.

> serversocket.sin_family = [AF_INET](https://stackoverflow.com/questions/1593946/what-is-af-inet-and-why-do-i-need-it);

> serversocket.sin_port = htons(atoi(argv[2]));

The second field of __server__ is unsigned short sin_port , which contain the port number. However, instead of simply copying the port number to this field, it is necessary to convert this to __network byte order__ using the function __htons()__ which converts a port number in __host byte order__ to a port number in __network byte order__.

> serversocket.sin_addr.s_addr = INADDR_ANY;

The third field of __sockaddr_in__ is a structure of type __struct in_addr__ which contains only a single field unsigned long s_addr. This field contains the IP address of the host. For server code, this will always be the IP address of the machine on which the server is running, and there is a symbolic constant __INADDR_ANY__ which gets this address.

> [write](https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.3.0/com.ibm.zos.v2r3.bpxbd00/rtwri.htm)(listensocket, sendbuff, strlen(sendbuff));

## program name : server.c

> Read in detail from [here](http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html)
