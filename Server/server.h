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

    //this method is used for parsing the HTTP request
    //can also be used to split the string by any deleimiter string
    //implemented in helperFunctions.cpp
    vector<std::string> split(std::string input, std::string delimit);

    //A small class, we need to pass the clientInfo for serveRequest method to
    //service the request in a separate thread
    //pasing a single argument is easier.
    class clientInfo{
    public:
        struct sockaddr_in addr;
        int socketfd;
        clientInfo(struct sockaddr_in addr_,int socketfd_):addr(addr_),socketfd(socketfd_){}
    };

    //general Server Interface. User will never instantiate this class's object
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

    //subclass for TCP server
    class TCPServer:public Server{
    public:
        TCPServer(int port, int domain, int con_limit);
        int Bind();
        int Listen();
        void Run();
    };

    //subclass for UDP server
    class UDPServer:public Server{
    public:
        UDPServer(int port, int domain, int con_limit);
        int Bind();
        int Listen();
        void Run();
    };

    //abstarct factory
    class createServerFactory{
    public:
        virtual Server* createTCPServer(int port, int conLimit)=0;
        virtual Server* createUPDServer(int port, int conLimit)=0;
        virtual ~createServerFactory(){}
    };

    //factory for creating IPv4 family server
    class createIPv4ServerFactory:public createServerFactory{
    public:
        Server* createTCPServer(int port, int conLimit);
        Server* createUPDServer(int port, int conLimit);
    };
    //factory for creating IPv6 family server
    class createIPv6ServerFactory:public createServerFactory{
    public:
        Server* createTCPServer(int port, int conLimit);
        Server* createUPDServer(int port, int conLimit);
    };

#endif
