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

int main(int argc, char* argv[]) {
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr, "usage: showip hostname\n");
        return 1;
    }
    char* ip = argv[1];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(ip, "http", &hints, &res))) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    printf("IP address for %s\n\n", ip);

    for (auto p = res; p != nullptr; p = p->ai_next) {
        void* addr = nullptr;
        const char* ipver = nullptr;

        // get the pointer to the address itself
        // different fields in IPv4 and IPv6
        if (p->ai_family == AF_INET) {
            addr = &(reinterpret_cast<struct sockaddr_in*>(p->ai_addr)->sin_addr);
            ipver = "IPv4";
        } else {
            addr = &(reinterpret_cast<struct sockaddr_in6*>(p->ai_addr)->sin6_addr);
            ipver = "IPv6";
        }

        // convert the ip to a string and print it
        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        printf("%s: %s\n", ipver, ipstr);
    }

    freeaddrinfo(res);
    return 0;
}