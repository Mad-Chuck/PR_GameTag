#include "constants.h"

#ifdef _WIN32
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#ifdef linux
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#endif

#include <iostream>
#include <cstring>

#include "Socket.h"

Socket::Socket() {

#ifdef _WIN32
    WSADATA wsaData;
	if (WSAStartup(MAKEWORD( 2, 2 ), &wsaData) != NO_ERROR) {
        std::cout << "Initialization error" << std::endl;
		exit(-1);
    }
#endif
	
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_descriptor < 0) { 
        std::cout << "Socket creation error" << std::endl;
        exit(-1);
    }
	
	serv_addr.sin_family = AF_INET;                                    // IPv4
    serv_addr.sin_port = htons(PORT);                            // port - htons ensures byte order (BE/LE)
    //serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");              // localhost
    //inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);              // localhost
    std::memset(serv_addr.sin_zero, '\0', sizeof(serv_addr.sin_zero));    // set to zero

	// connect client socket to server
    if (connect(socket_descriptor, (sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Socket connect error\n");
        exit(-1);
    }

	printf("Connected to server\n");

	in_message = new InMessage();
	out_message = new OutMessage();
}

Socket::~Socket() {
	delete in_message;
}

void Socket::receive_message() {
    if (recv(socket_descriptor, (char*) in_message, sizeof(InMessage), 0) < 0) {
        exit(-1);
    }
}

void Socket::send_message(char key) {
	out_message->direction = key;
	if (send(socket_descriptor, (char*) out_message, sizeof(OutMessage), 0) < 0) {
        exit(-1);
    }
}

InMessage* Socket::get_message() {
	return in_message;
}
