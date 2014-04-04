//interpreter.cpp
#include <iostream>
#include <fstream>
#include <string>    //String judging function
#include <stdlib.h>  //String judging function
#include <ctype.h>   //String judging function
#include "interpreter.h"
#include "comfun.h"
using namespace std;

//Part of the CPU registers. Starting values set to 0.
int reg[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // see "reg_name" in reg_index(..)

//For the convenience of write a program to implement assembly code, define enum type as array subscript.
enum IDX { DATA, AH, AL, BH, BL, DH, DL, CX, ZF, SF };

//From designated assembler source file reading list, return code code.
void interpreter::read_src()
{
    statement* tail = NULL;
    head = NULL;

    ifstream f(file);          //Open files by reading
    if(!f) openfile = 0;       //If opening the file is not successful, then file open mark set to 0
    else
    {
    openfile = 1;              //If the file opens normally, then file open mark set to 1
    while (f)
    {
        // create a new node for current statement
        statement* p = new statement;
        f >> p->cmd;
        if (p->cmd[0] == ';') //If this line is a semicolon beginning, says is comments lines, to skip trip.
        {
	      char line[80];
	      f.getline(line, 80);  //getline used to read the whole text

	      delete p;
	      continue; // skip current line (comment)
        }

        // we should check file status here!
        if (f.eof())
        {
            delete p;
            break; // while ends
        }

        //Check whether the line label lines (no assembly statement)
        if (p->cmd[strlen(p->cmd)-1] == ':') // label:
        {
            p->cmd[strlen(p->cmd)-1] = '\0'; // remove ':'
            strcpy(p->op1, ":");
        }
        else
        {
            switch (p->cmd[0]) //According to judge what the initials assembly instructions
            {
            case 'm': // m: MOV, MUL, MOD, MAX, MIN;  m reg1 reg2, m reg data
            case 'a': // add reg1 reg2, add reg data
                f >> p->op1 >> p->op2;
                break;
            case 'i': // int num, or inc reg
            case 'l': // loop label
            case 'j': // jmp label, jz, je, jnz, jne, jl, jg
                f >> p->op1;
                break;
            case 'd': // dec reg, div reg1 reg2, div reg data
                if (strcmp(p->cmd, "dec") == 0) f >> p->op1;
                else f >> p->op1 >> p->op2;
                break;
            case 'c': // chg reg1 reg2, clr
                if (strcmp(p->cmd, "chg") == 0) f >> p->op1 >> p->op2;
                break;
            case 's': // sub reg1 reg2, sub reg data, shw
                if (strcmp(p->cmd, "sub") == 0) f >> p->op1 >> p->op2;
                break;
            }
        }
        p->next = NULL;

        // append the new node to the list
        if (tail)
        {
            tail->next = p;     //Add a memory list node in order
            tail = p;
        }
        else
        {
            head = tail = p;
        }
    }//_WHILE_

    f.close();          //close file
    }//IF_ELSE_
}

//Perform show all code. Output format is regulate the results after.
void interpreter::show_code()
{
    statement *p = head;
    while (p)
    {
        cout << "--> " << p->cmd ;
        if (p->op1[0] == ':')   //label :
        {
            cout << endl;
            p = p->next;        //Turn to the next node
            continue;
        }

        switch (p->cmd[0])
        {
        case 'm': // MOV, MUL, MOD, MAX, MIN
        case 'a': // ADD
            cout << ' ' << p->op1 << ' ' << p->op2 << endl;
            break;
        case 'i': // INT NUM, INC REG
        case 'l': // LOOP LABEL
        case 'j': // JMP LABEL, JG, JL, JE, JZ, JNE, JNZ
            cout << ' ' << p->op1 << endl;
            break;
        case 'd': // DEC, DIV
            if (strcmp(p->cmd, "dec") == 0) cout << endl;
            else cout << ' ' << p->op2 << endl;
            break;
        case 'c': //  CHG, CLR
            if (strcmp(p->cmd, "chg") == 0) cout << ' ' << p->op1 << ' ' << p->op2 << endl;
            else cout << endl;
            break;
        case 's': //  SUB, SHW
            if (strcmp(p->cmd, "sub") == 0) cout << ' ' << p->op1 << ' ' << p->op2 << endl;
            else cout << endl;
            break;
        }
        p = p->next;
    }
}

//According to the instructions of the list contents order, explain each executive assembly instructions.
void interpreter::run_code()
{
    if(!openfile) cout << "File does not exist!" << endl;
    else if(head == NULL) cout << "File only comment or statements do not read!" << endl;
    else
    {
    int x = 1;     //Variable x: judge whether int 20 is process ended mark
    int y = 0;     //Variable y: judge whether the store data is HEX data
    statement *p = head;
    while (p)
    {
        //Judge whether for marking the line? Have turned to the next instruction
        if (p->op1[0] == ':')
        { // should be checked at first
            p = p->next;
            continue;
        }

        //Different instructions, perform different actions
        int op2_idx;
        switch (p->cmd[0])
        {
        case 'm': // MOV, MUL, MOD
            if (strcmp(p->cmd, "mov") == 0)   // MOV
            {
              op2_idx = reg_index(p->op2);
              if (op2_idx == 0) // op2 is NOT a reg!
                 if(atoh(p->op2) == -1) {y = 1; break;}  //If p->op2 not a standard of HEX data, y = 1
                 else reg[reg_index(p->op1)] = atoh(p->op2); //atoi(p->op2); mov reg data
              else
                 reg[reg_index(p->op1)] = reg[op2_idx]; //mov reg1 reg2
            }
            else if (strcmp(p->cmd, "mul") == 0)   // MUL
            {
              op2_idx = reg_index(p->op2);
              if (op2_idx == 0) // op2 is NOT a reg!
                 if(atoh(p->op2) == -1) {y = 1; break;}  //If p->op2 not a standard of HEX data, y = 1
                 else reg[reg_index(p->op1)] *= atoh(p->op2); //atoi(p->op2); mul reg data
              else
                 reg[reg_index(p->op1)] *= reg[op2_idx]; //mul reg1 reg2
            }
            else if (strcmp(p->cmd, "mod") == 0)  // MOD
            {
              op2_idx = reg_index(p->op2);
              if (op2_idx == 0) // op2 is NOT a reg!
                 if(atoh(p->op2) == -1) {y = 1; break;}  //If p->op2 not a standard of HEX data, y = 1
                 else reg[reg_index(p->op1)] %= atoh(p->op2); //atoi(p->op2); mod reg data
              else
                 reg[reg_index(p->op1)] %= reg[op2_idx]; //mod reg1 reg2
            }
            else if (strcmp(p->cmd, "max") == 0)  // MAX
            {
              op2_idx = reg_index(p->op2);
              if (op2_idx == 0) // op2 is NOT a reg!
                 if(atoh(p->op2) == -1) {y = 1; break;}  //If p->op2 not a standard of HEX data, y = 1
                 else reg[reg_index(p->op1)] = (reg[reg_index(p->op1)] > atoh(p->op2))? reg[reg_index(p->op1)] : atoh(p->op2); // max reg data
              else
                 reg[reg_index(p->op1)] = (reg[reg_index(p->op1)] > reg[op2_idx])? reg[reg_index(p->op1)] : reg[op2_idx]; //max reg1 reg2
            }
            else  // MIN
            {
              op2_idx = reg_index(p->op2);
              if (op2_idx == 0) // op2 is NOT a reg!
                 if(atoh(p->op2) == -1) {y = 1; break;}  //If p->op2 not a standard of HEX data, y = 1
                 else reg[reg_index(p->op1)] = (reg[reg_index(p->op1)] < atoh(p->op2))? reg[reg_index(p->op1)] : atoh(p->op2); // min reg data
              else
                 reg[reg_index(p->op1)] = (reg[reg_index(p->op1)] < reg[op2_idx])? reg[reg_index(p->op1)] : reg[op2_idx]; //min reg1 reg2
            }
            break;

        case 'a': // ADD
            op2_idx = reg_index(p->op2);
            if (op2_idx == 0) // op2 is NOT a reg!
                 if(atoh(p->op2) == -1) {y = 1; break;}  //If p->op2 not a standard of HEX data, y = 1
                 else reg[reg_index(p->op1)] += atoh(p->op2); //atoi(p->op2); add reg data
            else
                 reg[reg_index(p->op1)] += reg[op2_idx]; //add reg data
            break;

        case 's':
            if (strcmp(p->cmd, "sub") == 0)  // SUB
            {
              op2_idx = reg_index(p->op2);
              if (op2_idx == 0) // op2 is NOT a reg!
                 if(atoh(p->op2) == -1) {y = 1; break;}  //If p->op2 not a standard of HEX data, y = 1
                 else reg[reg_index(p->op1)] -= atoh(p->op2); //atoi(p->op2); sub reg data
              else
                 reg[reg_index(p->op1)] -= reg[op2_idx]; //sub reg data

              //Setting the register is a sign corresponding
              if (reg[reg_index(p->op1)] > 0)
                reg[ZF] = 0, reg[SF] = 0;
              else if (reg[reg_index(p->op1)] < 0)
                reg[ZF] = 0, reg[SF] = 1;
              else
                reg[ZF] = 1, reg[SF] = 0;
            }
            else  //SHW
            {
              cout << endl;
              for (int i=1; i<sizeof(reg)/sizeof(reg[0]); ++i)  //Show data in all registers
                cout << reg[i] << ' ';
            }
            break;

        case 'i': // INT 21, INT 20, INC REG
            if (reg_index(p->op1))   // INC REG
            {
                reg[reg_index(p->op1)]++;
            }
            else
            {
                switch (atoi(p->op1)) //atoi(p->op1)
                {
                case 21:
                	switch (reg[AH])
                    {
                	case 1: // input char
                	  char ch;
                	  cin >> ch;
                	  reg[AL] = int(ch);      // Store the data in the register AL
                	  break;
                	case 2: // output char
                	  cout << char(reg[DL]);  // output the data in the register DL
                	  break;
		            case 3: // output value
		              cout << reg[DL] << ' '; // add a space to separate numbers
	                  break;
                	}
                	break;
                case 20:
                	release(head);  //Release all occupied memory list node
                	x = 0;   //Determine whether int 20 is process ended mark
                	break;
                }
            } // if_else
            break;

        case 'l':  // LOOP
            // look for the label (p->op1) in the code-list
            reg[CX]--;
            if (reg[CX])
                p = find_label(head, p->op1);  //find the label
            break;

        case 'j':
            if ( (strcmp(p->cmd, "jmp") == 0) ||  // JMP LABEL
               ( (strcmp(p->cmd, "jg") == 0) && (reg[ZF] == 0) && (reg[SF] == 0) ) ||
               ( (strcmp(p->cmd, "jl") == 0) && (reg[ZF] == 0) && (reg[SF] == 1) ) ||
               ( (strcmp(p->cmd, "je") == 0) && (reg[ZF] == 1) ) ||
               ( (strcmp(p->cmd, "jz") == 0) && (reg[ZF] == 1) ) ||
               ( (strcmp(p->cmd, "jne") == 0) &&(reg[ZF] == 0) ) ||
               ( (strcmp(p->cmd, "jnz") == 0) &&(reg[ZF] == 0) ) )
            {
	           p = find_label(head, p->op1);   //find the label
            }
            break;

        case 'd':
             if (strcmp(p->cmd, "dec") == 0)  // dec
               {if (reg_index(p->op1))
                  { // DEC REG
                    reg[reg_index(p->op1)]--;
                    //Setting the register is a sign corresponding
                    if (reg[reg_index(p->op1)] > 0)
                      {
                      // cout << "DEC +" << endl;
		              reg[ZF] = 0, reg[SF] = 0;
                      }
                    else if (reg[reg_index(p->op1)] < 0)
                           {
                           // cout << "DEC -" << endl;
	 	                   reg[ZF] = 0, reg[SF] = 1;
                           }
                         else
                           {
                           // cout << "DEC 0" << endl;
		                   reg[ZF] = 1, reg[SF] = 0;
	                       }
                  }
                }
            else   //div
              {
                op2_idx = reg_index(p->op2);
                if (op2_idx == 0) // op2 is NOT a reg!
                  if(atoh(p->op2) == -1) {y = 1; break;}  //If p->op2 not a standard of HEX data, y = 1
                  else reg[reg_index(p->op1)] /= atoh(p->op2); //atoi(p->op2); div reg data
                else
                  reg[reg_index(p->op1)] /= reg[op2_idx]; //div reg1 reg2
              }
            break;

        case 'c':
            if (strcmp(p->cmd, "chg") == 0)  //chg
              {
                int temp = reg[reg_index(p->op1)];           //Two Numbers to swap
                reg[reg_index(p->op1)] = reg[reg_index(p->op2)];
                reg[reg_index(p->op2)] = temp;
              }
            else    // clr
              {
                for (int i=1; i<sizeof(reg)/sizeof(reg[0]); ++i)
                  reg[i] = 0;                                //All data reset to 0
              }
            break;
        } // _SWITCH_

        p = p->next;
    } // _WHILE_

    if(x) release(head);  //If x = 1 there is something wrong with "int 20" and all occupied memory list nodes are not released.

    if(!x && !y) cout << endl << "Program normally ends." << endl;  //The end of the move, display information
    else cout << endl << "Program has Bug!" << endl;  //Show to run error messages

    }//_IF_ELSE_
}
