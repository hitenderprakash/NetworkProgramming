#include "server.h"

void Server::serveRequest(clientInfo client){
    int n;
    char buff[1024];
    cout<<"\nInside threaded func\n";
    bzero(buff,1024);
    int nwsockfd = client.socketfd;
    struct sockaddr_in cli_addr = client.addr;
    struct in_addr ipAddr = cli_addr.sin_addr;
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET,&ipAddr,ip,INET_ADDRSTRLEN);

    cout<<"\n======Connection from: "<<ip<<" "<<ntohs(cli_addr.sin_port)<<"=======\n";

    n=read(nwsockfd,buff,1023);
    if(n<0){cerr<<"\nError reading from socket\n";}
    //get request
    std::string msg="";
    if(buff){msg=std::string(buff);}
    //cout<<msg;
    std::string fname=split(split(split(msg,"\r\n")[0]," ")[1],"/")[1];

    //prepare response
    std::string reply = "";
    string fileBuf="";
    ifstream resFile;
    resFile.open(fname.c_str());
    if(resFile){
        reply="HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n";
        while(!resFile.eof()){
            std::string temp="";
            getline(resFile,temp);
            fileBuf+="\n"+temp;
        }
        resFile.close();
    }
    else{
        reply="HTTP/1.0 404 NOT FOUND\r\nContent-Type: text/html\r\n\r\n";
    }
    send(nwsockfd,reply.c_str(),strlen(reply.c_str()),0);
    write(nwsockfd,fileBuf.c_str(),strlen(fileBuf.c_str())-1);
    close(nwsockfd);
}

vector<std::string> split(std::string input, std::string delimit){
    vector<std::string> splited;
    if (input.size()==0||delimit==""){
        splited.push_back(input);
        return splited;
    }
    while (input.size()>0){
        int found=input.find(delimit);
        if(found==std::string::npos){
            splited.push_back(input);
            return splited;
        }
        else{
            splited.push_back(input.substr(0,found));
            input=input.substr(found+delimit.size(),input.size()-1);
        }
    }
    return splited;
}
