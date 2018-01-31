#ifndef _ERRORTAB_H_
#define _ERRORTAB_H_

#include<iostream>
#include<vector>


class ErrorTable{

    public:
    int error_count = 0;
    std::vector<std::string> errors_list = {" multiple declaration of period . ",   //0
                        " \" expected, incomplete string",                        //1
                        " invalid string declaration, ',' expected",                //2
                        " invalid character ",                                      //3
                        " invalid character res ",                                  //4
                        " invalid character equ ",                                  //5
                        " invalid expression ",                                     //6
                        " invalid expression, - expected after $",                  //7
                        "  invalid use of section",                                 //8
                        " invalid character in section definition ",                 //9
                        "  data/bss/text section required",                         //10
                        " multiple declaration of : ",                                     //11
                        " bss section must contain resb/resw/resd/resq declarations", //12
                        " text section must not contain any declarations",              //13
                        " data section must contain db/dw/dd/dq declarations",          //14
                        " redeclaration of lables/variables ",                                 //15
                        " symbol not found ",                                            //16
                        " mnemonic not found or invalid mnemonic ",                      //17
                        " invalid type cast ",                                           //18
                        " ] parenthesis expected ",                                      //19
                        " undeclared variables is used ",                                //20
                        " operand expected ",                                            //21
                        " mismatch in operand sizes ",                                   //22
                        " invalid combination of opcode and operands "                  //23
                        };

        void print_errors(int, int, int, bool, bool);
};


#endif