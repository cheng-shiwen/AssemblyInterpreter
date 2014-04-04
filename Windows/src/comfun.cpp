//comfun.cpp
#include <iostream>
#include <string>    //String judging function
#include <stdlib.h>  //String judging function
#include <ctype.h>   //String judging function
#include "interpreter.h"
using namespace std;

extern int reg[10];  //Statement external variables
enum IDX { DATA, AH, AL, BH, BL, DH, DL, CX, ZF, SF };

//According to registers name, find it corresponds reg array subscript position. 0 says don't find.
int reg_index(char* r)
{
    char* reg_name[] = {"data", "ah", "al", "bh", "bl", "dh", "dl", "cx", "zf", "sf"};

    for (int i=sizeof(reg)/sizeof(reg[0]) - 1; i>=0; --i)    //Repeat, judge
    {
        if (strcmp(reg_name[i], r) == 0)
            return i;     //return the corresponding reg array subscript position
    }
    return 0;             //0 says don't find.
}

//Debugging in print out the CPU when the value of the register.
void show_reg()
{
    for (int i=1; i<sizeof(reg)/sizeof(reg[0]); ++i)
        cout << reg[i] << ' ';
    cout << endl;
}

//Release all occupied memory list node
void release(statement *head)
{
    statement* p = head;
    while (p)
    {
        head = p->next;
        delete p;               //Release the memory list node in proper order
        p = head;
    }
}

// In assembler code in search of "line" label, return the node pointer store label. If not find it, then return NULL
statement* find_label(statement* head, char* label)
{
    while (head)
    {
        if (strcmp(head->cmd, label) == 0)
             return head;       //Return the node pointer store label
        else
             head = head->next;
    }
    return NULL;       //Not find it, return NULL
}

// string in HEX to int
int atoh(char* str)
{
    int len = strlen(str);    //Calculate the length of the str
    int sum = 0;
    for (int i=0; i<len; i++)
    {
        sum <<= 4;
        if((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'A' && str[i] <= 'F'))
          sum += (isdigit(str[i]) ? (str[i]-'0') : (str[i]-'A'+10));    //Every four back to front characters in HEX to int
        else return -1;     //String for non-standard hexadecimal number to return -1
    }
    return sum;
}
