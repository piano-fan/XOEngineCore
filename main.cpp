#include <iostream>
#include "core.h"


int main(int argc, char** argv){
    XO::Core core(argc,argv);

    int code = 0;
    std::string input;
    do{
        if( !std::cin.eof() ) {
            std::getline(std::cin, input);
            code = core.onInput(input);
        }
    } while( !code );

    return code;
}
