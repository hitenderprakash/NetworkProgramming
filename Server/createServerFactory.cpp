/*
    Hitender Prakash
    Interface for server creation factory
*/
#include "server.h"
//create IPv4 TCPServer
Server* createIPv4ServerFactory::createTCPServer(int port, int conLimit=100){
    return new TCPServer(port, AF_INET, conLimit);
}
//create IPv4 UDPServer
Server* createIPv4ServerFactory::createUDPServer(int port, int conLimit=100){
    return new UDPServer(port, AF_INET, conLimit);
}

//create IPv6 TCPServer
Server* createIPv6ServerFactory::createTCPServer(int port, int conLimit=100){
    return new TCPServer(port, AF_INET6, conLimit);
}
//create IPv6 UDPServer
Server* createIPv6ServerFactory::createUDPServer(int port, int conLimit=100){
    return new UDPServer(port, AF_INET6, conLimit);
}
