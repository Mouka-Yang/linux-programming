#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <iostream>

int main() {
    struct sockaddr_in sa;
    struct sockaddr_in6 sa6;

    char ip4[INET_ADDRSTRLEN], ip6[INET6_ADDRSTRLEN];

    inet_pton(AF_INET, "10.1.1.1", &(sa.sin_addr));
    inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr));

    std::cout << "IPv4: " << (sa.sin_addr.s_addr) << std::endl;
    std::cout << "IPv6: " << sa6.sin6_addr.__u6.__s6_addr32 << std::endl;

    inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);
    inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN);

    std::cout << "Original IPv4: " << ip4 << std::endl;
    std::cout << "Original IPv6: " << ip6 << std::endl;
}