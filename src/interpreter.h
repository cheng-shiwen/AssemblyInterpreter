//interpreter.h
#ifndef INTERPRETER_H     //Conditional compilation: to prevent redundant compilations
#define INTERPRETER_H
using namespace std;

//Assembly instructions node (list) each node storage "commands" most "operand 1", "2" and so on the operand three parts of information
struct statement
    {
      char cmd[9], op1[9], op2[9]; // 1. mov reg reg  2. mov reg data // bug_6: op1[3], op2[3]
      statement *next;
    };

//class: interpreter
class interpreter
    {
      private:
        char* file;               //Define file pointer, accept the filename
        statement* head;          //Define head pointer
        int openfile;             //Define variable. file open mark
      public:
        interpreter(char* file0): file(file0) {}   //Define constructor, pass on the filename of file to read
        void read_src();                           //Define function to read code
        void show_code();                          //Define function to show code
        void run_code();                           //Define function to run code
    };

#endif
