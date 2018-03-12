/*
    Hitender Prakash
    Interface for HTTP server

*/
#ifndef _SERVER_H
#define _SERVER_H

    #include <iostream>
    #include <string>
    #include <vector>
    #include <fstream>
    #include <sstream>
    #include <iterator>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <thread>
    #include <arpa/inet.h>
    using namespace std;

    vector<std::string> split(std::string input, std::string delimit);

    class clientInfo{
    public:
        struct sockaddr_in addr;
        int socketfd;
        clientInfo(struct sockaddr_in addr_,int socketfd_):addr(addr_),socketfd(socketfd_){}
    };

    class Server{
    protected:
        int domain;
        int type;
        int protocol;

        int listen_port;
        int listen_socketfd;
        struct sockaddr_in serv_addr;

        int connection_limit;
    public:
        virtual int Bind()=0;
        virtual int Listen()=0;
        virtual void Run()=0;
        virtual ~Server(){}
        void serveRequest(struct clientInfo client);
    };

    class TCPServer:public Server{
    public:
        TCPServer(int port, int domain, int con_limit);
        int Bind();
        int Listen();
        void Run();
    };
    class UDPServer:public Server{
    public:
        UDPServer(int port, int domain, int con_limit);
        int Bind();
        int Listen();
        void Run();
    };

    class createServerFactory{
    public:
        virtual Server* createTCPServer(int port, int conLimit=100)=0;
        virtual Server* createUPDServer(int port, int conLimit=100)=0;
    };
    class createIPv4ServerFactory:public createServerFactory{
    public:
        Server* createTCPServer(int port, int conLimit=100){
            return new TCPServer(port, AF_INET, conLimit);
        }
        Server* createUPDServer(int port, int conLimit=100){
            return new UDPServer(port, AF_INET, conLimit);
        }
    };
    class createIPv6ServerFactory:public createServerFactory{
    public:
        Server* createTCPServer(int port, int conLimit=100){
            return new TCPServer(port, AF_INET6, conLimit);
        }
        Server* createUPDServer(int port, int conLimit=100){
            return new UDPServer(port, AF_INET6, conLimit);
        }
    };

#endif
