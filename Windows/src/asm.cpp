//asm.cpp
#include <iostream>
#include "interpreter.h"
using namespace std;

int main(int argc, char** argv)     //Command line parameters
{
    if (argc != 2)                  //If the input parameters not 2, it is executed the sentences
    {
        cout << "asm.exe asm_source_file" << endl;
        return 1;                   //Program exits
    }

    interpreter inter(argv[1]);     //Define object, meanwhile pass on the filename of file to read
    inter.read_src();               //read code
    inter.show_code();              //show code
    inter.run_code();               //run code

    return 0;
}
