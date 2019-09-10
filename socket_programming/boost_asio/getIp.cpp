#include <iostream>
#include <boost/asio.hpp>
using namespace boost::asio;

int main(){
    io_service service;
    ip::tcp::resolver resolver(service);
    ip::tcp::resolver::query query("www.baidu.com", "80");
    auto iter = resolver.resolve(query);
    auto ep = static_cast<ip::tcp::endpoint>(*iter);
    std::cout << ep.address().to_string() << std::endl;
}