#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
const char* port = "3490";
const int backLog = 10;

void* getInAddr(struct sockaddr* sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
int main(int argc, char* argv[]) {
    struct addrinfo hints, *serveInfo, *p;
    int status;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((status = getaddrinfo(nullptr, port, &hints, &serveInfo))) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    // loop through all the results and bind to the first we can
    int sockfd = -1;
    for (p = serveInfo; p != nullptr; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) {
            std::cerr << "Open socket failed: " << strerror(errno) << std::endl;
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            std::cerr << "Bind socket failed: " << strerror(errno) << std::endl;
            continue;
        }

        break;
    }
    freeaddrinfo(serveInfo);

    if (p == nullptr) {
        std::cerr << "Failed to bind" << std::endl;
        return 1;
    }

    if (listen(sockfd, 10)) {
        std::cerr << "Listen socket failed: " << strerror(errno) << std::endl;
        return 1;
    }

    struct sockaddr_storage client_addr;
    socklen_t addr_size = sizeof(sockaddr_storage);
    int client_fd = accept(sockfd, reinterpret_cast<struct sockaddr*>(&client_addr), &addr_size);
    return 0;
}