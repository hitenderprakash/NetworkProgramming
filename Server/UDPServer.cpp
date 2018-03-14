/*
    Hitender Prakash
    subclass of Server for UDPServers
*/
#include "server.h"


UDPServer::UDPServer(int port, int domain, int con_limit){
    //init common parameters of server
    Server::initCommonServerParam(port,domain,con_limit);
    //set type
    type=SOCK_DGRAM;
    //create UDP socket
    listen_socketfd=socket(this->domain,type,protocol);
}

int UDPServer::Bind(){
    return Server::Bind();
}

int UDPServer::Listen(){
    //UDP Server does not call listen but to keep the interface generic for all server we will return success
    return 1;
}


void UDPServer::Run(){
    while(1){
        ConnectionInfo conInfo(domain,type,1024);
        socklen_t clilen=sizeof(conInfo.ClientAddr);
        bzero(conInfo.buff,conInfo.buffSize);

        //UDP server will not accept but directly receive
        if((conInfo.numBytes=recvfrom(listen_socketfd, conInfo.buff,sizeof(conInfo.buff),0,(struct sockaddr *)&conInfo.ClientAddr,&clilen))>0){
            mtx.lock();
            if(active_connections < connection_limit){
                std::thread servThread(&UDPServer::serveRequest,this,conInfo);
                active_connections++;
                servThread.detach();
            }
            else{
                //std::cout<<"\nConnection Limit over, can't accept new connection";
                //std::cout<<"\nDropping connection from: ";
                close(conInfo.socketfd);
            }
            mtx.unlock();
        }
    }
    close(listen_socketfd);
}

//method for serving the request of the client
//to be executed by a new thread
void UDPServer::serveRequest(ConnectionInfo conInfo){
    std::string ClientIP = conInfo.getIPAddress();
    int clientPort = conInfo.getPort();
    socklen_t clilen=sizeof(conInfo.ClientAddr);
    std::cout<<"\nServing new connection from: "<<ClientIP<<" :"<<clientPort<<"\n";
    std::string msg="";
    if(conInfo.buff){msg=std::string(conInfo.buff);}
    std::string fname = msg;

    //since files are kept at different file path depending on their types
    //create the full file path
    //currently hard coding all the directory to "Files"
    std::string absFileName= "./Files/"+fname;
    //prepare response
    std::string response = "";
    string fileBuf="";
    ifstream resFile;
    resFile.open(absFileName.c_str());
    if(resFile){
        response="\n==================File Content:===================\n";
        while(!resFile.eof()){
            std::string temp="";
            getline(resFile,temp);
            fileBuf+="\n"+temp;
        }
        resFile.close();
    }
    else{
        response="\nRequested File not found!\n";
    }
    fileBuf=response+fileBuf;
    conInfo.numBytes = sendto(listen_socketfd, fileBuf.c_str(), strlen(fileBuf.c_str())-1, 0,(struct sockaddr *)&conInfo.ClientAddr, clilen);
    mtx.lock();
    active_connections--;
    mtx.unlock();
}
