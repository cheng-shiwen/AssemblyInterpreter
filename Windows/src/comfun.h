//comfun.h
#ifndef COMFUN_H
#define COMFUN_H
#include "interpreter.h" 

//Function declarations 
int reg_index(char* r);
void show_reg();
void release(statement *head);
statement * find_label(statement* head, char* label);
int atoh(char* str);

#endif
