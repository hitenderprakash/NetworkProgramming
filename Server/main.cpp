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

    createServerFactory *factory = new createIPv4ServerFactory();
    Server *s = factory->createTCPServer(port, 10);
    std::cout<<"\n========HTTP Server starting on port: "<<port<<" =============\n";
    std::cout<<"\nTo stop the server, please enter \"Ctrl+C\"\n";
    Init(s);

}
