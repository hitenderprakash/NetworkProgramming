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
using namespace std;

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

int main(int argc, char *argv[]){
    int sockfd;
    int port;
    char buff[1024];
    struct sockaddr_in serv_addr,cli_addr;
    int n;

    if(argc<2){
        std::cerr << "\nError: No port provided\n";
        exit(0);
    }

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        std::cerr << "\nError: in opening socket\n";
        exit(0);
    }
    bzero((char*)&serv_addr,sizeof(serv_addr));
    port=atoi(argv[1]);
    serv_addr.sin_addr.s_addr=INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if(bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
        std::cerr << "\nError in binding to port\n";
        exit(0);
    }
    listen(sockfd,10);

    socklen_t clilen=sizeof(cli_addr);
    while(1){
        int nwsockfd=accept(sockfd, (struct sockaddr *)&cli_addr,&clilen);
        if(nwsockfd<0){
            std::cerr <<"\nError in accept\n";
        }
        cout<<"\n======Connection from port: "<<ntohs(cli_addr.sin_port)<<"=======\n";
        bzero(buff,1024);
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
            reply="HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<html><head><title>HP</title></head><body><b>Content from requested file:<br></b></body></html>";
            while(!resFile.eof()){
                std::string temp="";
                getline(resFile,temp);
                fileBuf+="\n"+temp;
            }
            resFile.close();
        }
        else{
            reply="HTTP/1.0 404 NOT FOUND\r\nContent-Type: text/html\r\n\r\n<html><head><title>HP</title></head><body><b>Requested file not found<br></b></body></html>";
        }
        send(nwsockfd,reply.c_str(),strlen(reply.c_str()),0);
        write(nwsockfd,fileBuf.c_str(),strlen(fileBuf.c_str())-1);
        close(nwsockfd);
    }
    close(sockfd);
    return 0;
}
