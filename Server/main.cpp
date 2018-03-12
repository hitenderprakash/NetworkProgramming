/*
    Hitender Prakash
    Driver code for Server
*/
#include "server.h"

void Init(Server *s){
    int res= s->Bind();
    if(res<0){exit(0);}
    res= s->Listen();
    if(res<0){exit(0);}
    s->Run();
}

int main(int argc, char *argv[]){
    int port;
    if(argc < 2){
        std::cerr<<"\nUsage:  ./server <port>\n";
        exit(0);
    }
    try{
        port = stoi(argv[1]);
    }
    catch(...){
        std::cerr<<"\nError: Port is not valid";
        exit(0);
    }

    //factory is a general object
    createServerFactory *factory = new createIPv4ServerFactory(); //to create IPv4 fanily
    //to create IPv6 family server call
    //createServerFactory factory2 = new createIPv6ServerFactory();

    //create TCP Server
    Server *s = factory->createTCPServer(port, 10);

    //to make UDP cerver
    // Server *s2 = factory->createUPDServer(port, 10)

    std::cout<<"\n========HTTP Server starting on port: "<<port<<" =============\n";
    std::cout<<"\nTo stop the server, please enter \"Ctrl+C\"\n";
    Init(s);
    //keep running in loop
    //to stop the server enter Crtl+c

}
