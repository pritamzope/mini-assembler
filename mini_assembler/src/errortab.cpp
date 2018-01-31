#include<iostream>
#include "errortab.hpp"

void ErrorTable::print_errors(int error_type, int linenumber, int col, bool should_exit, bool print_newline)
{
    if(should_exit && print_newline){
        std::cout<<"error: "<<linenumber<<":"<<col<<errors_list[error_type]<<std::endl;
        exit(0);
    }else if(should_exit && !print_newline){
        std::cout<<"error: "<<linenumber<<":"<<col<<errors_list[error_type];
        exit(0);
    }else if(!should_exit && print_newline){
        std::cout<<"error: "<<linenumber<<":"<<col<<errors_list[error_type]<<std::endl;
    }else{
        std::cout<<"error: "<<linenumber<<":"<<col<<errors_list[error_type];
    }
}
