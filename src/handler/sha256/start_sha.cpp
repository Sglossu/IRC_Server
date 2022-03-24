#include <iostream>
#include "Sha256.hpp"
 
using std::string;
using std::cout;
using std::endl;
 
int start_sha(int argc, char *argv[])
{
    std::string input = "123";
    std::string output1 = sha256(input);
 
    cout << "sha256('"<< input << "'):" << output1 << endl;
    return 0;
}