#ifndef _PASS_1_H_
#define _PASS_1_H_

#include<iostream>
#include<list>
#include"symtab.hpp"

extern bool has_match_data_types(std::string, int *);
extern bool has_match_res_types(std::string, int *);
extern bool match_with_registers(std::string, int *);
extern bool match_with_casts(std::string, int *);
extern void clear_buffer(char*);
extern std::string trim(std::string &);
extern std::string trim_2(std::string &);
extern std::string decimal_to_hex(int);
extern int hex_to_decimal(std::string);
extern void remove_comments(std::string, int *);
extern void skip_spaces(std::string &, int *);
extern char *get_char_ptr(std::string);
extern bool seek_colon(std::string &, int);
extern char peek_next_char(std::string, int*);
extern bool check_are_all_white_spaces(std::string);
extern void check_and_display_warnings();
extern void read_number(std::string, int);
extern void read_string(std::string, int);
extern void read_string_number(std::string, int);
extern void read_resType_number(std::string, int);
extern void read_EQU_cal_len(std::string, int);
extern int get_section_type(std::string, int *);
extern void read_symbol_from_cast(std::string, std::string, int *);
extern std::string read_second_operand(std::string, std::string, int *, int *);
extern void calc_inst_address(std::string, std::string, std::string);
extern void do_instructions(std::string, std::string, int *, int);
extern void read_declarations_pass_1(std::string, SymbolTable &, std::ofstream &);
extern void do_pass_1(const std::string, const std::string);

#endif