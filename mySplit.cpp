#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

std::vector<std::string> mySplit(std::string s, std::string delim);

int main(){
    string str("this is a good news");
    std::vector<std::string> vs = mySplit(str, " ");
    for(int i=0; i<vs.size(); ++i){
        cout<<vs[i]<<endl;
    };
}

std::vector<std::string> mySplit(std::string s, std::string delim){
    std::vector<std::string> v;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delim)) != std::string::npos) {
        token = s.substr(0, pos);
        v.push_back(token);
        s.erase(0, pos + delim.length() );
    }
    v.push_back(s);
    return v;
}