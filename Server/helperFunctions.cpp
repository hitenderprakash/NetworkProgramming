/*
    Hitender Prakash
    this file contain all the helper functions used in server program
*/
#include "server.h"

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
