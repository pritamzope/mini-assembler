#ifndef _PASS_2_H_
#define _PASS_2_H_

#include<iostream>
#include<list>
#include"symtab.hpp"

extern int get_reg_decm_opcode(std::string);
extern int get_cast_size(std::string);
extern std::string get_8byte_hex_value(std::string);
extern std::string get_str_type_hex_values(VAL_LIST_NODE *);
extern std::string get_num_type_hex_values(VAL_LIST_NODE *, int, std::string);
extern std::string get_hex_value_by_byte(int, SYMTAB_NODE *, std::string);
extern std::string get_data(std::string, int *);
extern std::string get_spaces(int);
extern void write_datas_to_file(std::ofstream &, std::string, SYMTAB_NODE *, std::string, std::string);
extern std::string get_symbol_cast(std::string, int *);
extern std::string get_operand(std::string, int *, bool *, int *);
extern void assemble_instructions(std::string, std::string, std::string, std::ofstream &);
extern void read_declarations_pass_2(std::string, std::ofstream &);
extern void gen_lst_file(const std::string, const std::string);


#endif
