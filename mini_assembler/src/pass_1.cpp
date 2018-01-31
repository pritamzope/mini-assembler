#include<iostream>
#include<fstream>
#include<list>
#include<cstdlib>
#include<algorithm>
#include<cctype>
#include<cstring>
#include<unordered_set>
#include"symtab.hpp"
#include"errortab.hpp"
#include"optab.hpp"
#include"pass_1.hpp"

//declare objects of classes
SymbolTable sm;
ErrorTable er;

extern std::unordered_set<OPCodeTable, Hash_OpTab> opt;

char Buffer[255];
int lineNumber = 0;

extern SYMTAB_NODE *currInstNode;

std::string gblabel;
static int type_decl_size = 0, param_type = 0, lab_decl_type = 0;
static int last_section_type = -1;

static bool data_section_found = false, bss_section_found = false, text_section_found = false;

int data_address = 0x00000000, bss_address = 0x00000000, inst_address = 0x00000000;
int inst_add_cnt = 0, last_inst_size = 0;

std::string last_inst = "";

std::list<std::string> param_list;

int operand_1_size = -1, operand_2_size = -1;

std::string datatypes[5] = {"db", "dw", "dd", "dq", "dt"};
std::string restypes[5] = {"resb", "resw", "resd", "resq", "rest"};
int sizes[] = {1, 2, 4, 8, 10};

std::vector<std::string> registers = {"al","ax","eax",
                                      "cl","cx","ecx",
                                      "dl","dx","edx",
                                      "bl","bx","ebx",
                                      "ah","sp","esp",
                                      "ch","bp","ebp",
                                      "dh","si","esi",
                                      "bh","di","edi"};

std::vector<int> reg_size = { 1,2,4,
                              1,2,4,
                              1,2,4,
                              1,2,4,
                              1,2,4,
                              1,2,4,
                              1,2,4,
                              1,2,4};

std::vector<std::string> casts = {"byte","word","dword"};
std::vector<int> casts_size = {1,2,4};


bool has_match_data_types(std::string symbol, int *index)
{
    for(int i=0;i<5;i++){
        if(symbol.compare(datatypes[i]) == 0){
            *index = i;
            return true;
        }
    }
    return false;
}


bool has_match_res_types(std::string symbol, int *index)
{
    for(int i=0;i<5;i++){
        if(symbol.compare(restypes[i]) == 0){
            *index = i;
            return true;
        }
    }
    return false;
}


bool match_with_registers(std::string reg, int *index)
{
  for(int i=0;i<24;i++){
    if(!reg.compare(registers[i])){
      *index = i;
      return true;
    }
  }
  return false;
}


bool match_with_casts(std::string cast, int *index)
{
  for(int i=0;i<3;i++){
    if(!cast.compare(casts[i])){
      *index = i;
      return true;
    }
  }
  return false;
}


void clear_buffer(char *buffer)
{
    for(int i=0;i<255;i++)
        buffer[i]='\0';
}


std::string trim(std::string &str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first){
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}
std::string trim_2(std::string &str)
{
    size_t first = str.find_first_not_of('\t');
    if (std::string::npos == first){
        return str;
    }
    size_t last = str.find_last_not_of('\t');
    return str.substr(first, (last - first + 1));
}


std::string decimal_to_hex(int n)
{
  std::string hex = new char[100];
  if(n == 0){
    hex = "0";
    return hex;
  }
  char* hexchr = new char[100];
  int index = 0;
  while(n!=0){
    int temp  = 0;
    temp = n % 16;
    if(temp < 10){
      hexchr[index] = temp + 48;
      index++;
    }else{
      hexchr[index] = temp + 55;
      index++;
    }
    n = n/16;
  }
  hexchr[index] = '\0';
  hex.assign(hexchr,strlen(hexchr));
  reverse(hex.begin(), hex.end());
  return hex;
}


int hex_to_decimal(std::string hexnum)
{   
  int len = hexnum.length();
  int base = 1;
  int decm_val = 0;
  for(int i=len-1; i>=0; i--){   
    if(hexnum[i]>='0' && hexnum[i]<='9'){
      decm_val += (hexnum[i] - 48)*base;
      base = base * 16;
    }else if(hexnum[i]>='A' && hexnum[i]<='F'){
      decm_val += (hexnum[i] - 55)*base;
      base = base*16;
    }
  }     
  return decm_val;
}



void remove_comments(std::string strbuf, int *index)
{
    while(*index < (int)strbuf.length()){
        if(strbuf[*index] == '\n')
            break;
        else
            *index = (*index) + 1;
    }
}


//ignores the spaces in buffer
void skip_spaces(std::string &strbuf, int *index)
{
    while(*index < (int)strbuf.length()){
        if(strbuf[*index] == ' ' || strbuf[*index] == '\t')
            (*index)++;
        else
            break;
    }
}


//returns char* of string
char *get_char_ptr(std::string str)
{
  char *strch = new char[str.size()+1];
  memcpy(strch, str.c_str(), str.size() + 1);
  return  strch;
}


//seek colon in buffer
bool seek_colon(std::string &strbuf, int index)
{
    for(;index<(int)strbuf.length(); index++){
        if(strbuf[index] == ':')
            return true;
        else if(strbuf[index] == ' ')
            continue;
        else
            break;
    }
    return false;
}


//peeak the character in buffer other then white space
char peek_next_char(std::string strbuf, int* indx)
{
  int index = *indx;
  char ch;
  while(index < (int)strbuf.length()){
    if(strbuf[index] == ' ' || strbuf[index] == '\t'){
      index++;
      continue;
    }else{
      ch = strbuf[index];
      break;
    }
  }
  *indx = index+1;
  return ch;
}


//check for entries of ' ' or '\t' in buffer
bool check_are_all_white_spaces(std::string strbuf)
{
  return strbuf.empty() || strbuf.find_first_not_of(strbuf[0]) == std::string::npos;
}


/*
returns the symbol read after global or extern found
in text section
*/
std::string get_global_extern_symbol(std::string strbuf, int *indx)
{
  int index = *indx;
  std::string symbol;
  while(index < (int)strbuf.length()){
    if(isalnum(strbuf[index])){
      symbol = symbol+strbuf[index];
      index++;
    }else if(strbuf[index] == '\n'){
      break;
    }else{
      er.print_errors(6, lineNumber, 1, true, true);
      er.error_count++;
    }
  }
  return symbol;
}


//it compares the sizes of operand 1 & 2
//and display error on it
void check_and_display_errors()
{
  if(operand_1_size < operand_2_size){
    er.print_errors(22, lineNumber, 1, false, true);
    er.error_count++;
    return;
  }else if(operand_1_size > operand_2_size){
    return;
  }else{
    return;
  }
}



/* read each digit from strbuf if num found push it in the list */
void read_number(std::string strbuf, int index)
{
    std::string num;
    bool has_comma_found = false, has_dot_found = false;

    while(index < (int)strbuf.length()){
        //if isdigit then store
        if(isalnum(strbuf[index])){
            num = num + strbuf[index];
            index++;
        // if . then first check for has_dot_found otherwise store
        }else if(strbuf[index] == '.'){
            if(has_dot_found){
                er.print_errors(0, lineNumber, index+1, false, true);
                er.error_count++;
                if(er.error_count > 0){
                    return;
                }
            }else{
                has_dot_found = true;
                num = num + strbuf[index];
                index++;
            }
        // if space found store it also
        }else if(strbuf[index] == ' ' || strbuf[index] == '\t'){
            num = num + strbuf[index];
            index++;
        // if comma found, first check has_comma_found, then trim the num and check if any
        // space in it again then error otherwise push it in list 
        }else if(strbuf[index] == ','){
            if(has_comma_found){
                er.print_errors(6, lineNumber, index+1, false, true);
                er.error_count++;
                if(er.error_count > 0){
                    return;
                }
            }else{
                has_comma_found = true;
            }
            if(! num.empty()){
                num = trim(num);
                num = trim_2(num);
                if(num.find(" ", 0) != std::string::npos || num.find("\t", 0) != std::string::npos){
                    er.print_errors(6, lineNumber, index+1, false, true);
                    er.error_count++;
                    if(er.error_count > 0){
                        return;
                    }
                }
                param_list.push_back(num);
                param_type = NUM_TYPE;
                num.clear();
                has_comma_found = false;
                has_dot_found = false;
            }
            index++;
        // if \n, then same action as for ,
        }else if(strbuf[index] == '\n'){
            if(! num.empty()){
                num = trim(num);
                num = trim_2(num);
                if(num.find(" ", 0) != std::string::npos || num.find("\t", 0) != std::string::npos){
                    er.print_errors(6, lineNumber, index+1, false, true);
                    er.error_count++;
                    if(er.error_count > 0){
                        return;
                    }
                }
                param_list.push_back(num);
                param_type = NUM_TYPE;
                num.clear();
                has_comma_found = false;
                has_dot_found = false;
            }
            break;
        }else if(strbuf[index] == ';'){
            remove_comments(strbuf, &index);
        }
    }
}



/* read each character from strbuf who is between "" 
and concat it with str */
void read_string(std::string strbuf, int index)
{
    //allocate memory to sttt by using new and assign it to str
    std::string *sttt = new std::string();
    std::string str = *sttt;
    while(index < (int)strbuf.length()){
        // if is \n and not \" then error
        if(strbuf[index] == '\n' && strbuf[index] != '\"'){
                er.print_errors(1, lineNumber, index+1, true, true);
                er.error_count++;
        }
        // if \" found push it in the list
        if(strbuf[index] == '\"'){
            param_list.push_back(str);
            param_type = STR_TYPE;
            break;
        }else{
            str.push_back(strbuf[index]);
            index++;
        }
    }
    index++;
    skip_spaces(strbuf, &index);
    if(strbuf[index] == ','){
        read_number(strbuf, index+1);
    }else if(isalnum(strbuf[index])){
        er.print_errors(2, lineNumber, index+1, false, true);
        er.error_count++;
    }
    param_type = STR_TYPE;
}



// function that call read_string() when " found 
// & read_number() function when digit found
void read_string_number(std::string strbuf, int index)
{
    while(index < (int)strbuf.length()){
        if(strbuf[index] == '\"'){
            read_string(strbuf, index+1);
            break;
        }else if(isdigit(strbuf[index])){
            read_number(strbuf, index);
            break;
        }else if(strbuf[index] == ' ' || strbuf[index] == '\t'){
            index++;
            continue;
        }else if(strbuf[index] == ';'){
            remove_comments(strbuf, &index);
        }else{
            er.print_errors(3, lineNumber,index+1, false, false);
            std::cout<<strbuf[index]<<std::endl;
            er.error_count++;
        }
        index++;
    }
}



// function that reads the resv type declared variables
void read_resType_number(std::string strbuf, int index)
{
    std::string constnum;
    while(index < (int)strbuf.length()){
        if(isdigit(strbuf[index])){
            constnum = constnum + strbuf[index];
            index++;
        }else if(strbuf[index] == ' ' || strbuf[index] == '\t'){
            if(!constnum.empty()){
                param_list.push_back(constnum);
                constnum.clear();
            }
            index++;
            continue;
        }else if(strbuf[index] == '\n'){
            if(!constnum.empty()){
                param_list.push_back(constnum);
                constnum.clear();
            }
            break;
        }else if(strbuf[index] == ';'){
            remove_comments(strbuf, &index);
        }else{
            er.print_errors(4, lineNumber, index+1,false, true);
            std::cout<<strbuf[index]<<std::endl;
            er.error_count++;
        }
    }
}


//function that checks $- sign and the symbol defined after it
// if found, search that symbol in symbol table, if found in symbol table
// calculate the length of the inserted values
void read_EQU_cal_len(std::string strbuf, int index)
{
    std::string str;
    // first remove the extra not needed white spaces
    while(strbuf[index] != '\n'){
        if(strbuf[index] == ' ' || strbuf[index] == '\t'){
            index++;
            continue;
        }else{
            break;
        }
    }
    index++;
    //first chekc for - 
    if(strbuf[index] == '-'){
        index++;
        // if next is alphabet/digit then go next
        if(isalnum(strbuf[index])){
            //continue loop till you found the symbol
            while(index < (int)strbuf.length()){
                if(isalnum(strbuf[index]) || strbuf[index] == ' ' || strbuf[index] == '\t'){
                    str = str + strbuf[index];
                    index++;
                }else if(strbuf[index] == '\n'){
                    str = trim(str);
                    str = trim_2(str);
                    // search the node in symbol table and get node
                    SYMTAB_NODE *srchnode = sm.search_node(get_char_ptr(str));
                    //if not found
                    if(!srchnode){
                        er.print_errors(16, lineNumber, index+1, false, false);
                        std::cout<<str<<std::endl;
                        er.error_count++;
                    }else{
                        // if found cast it to char* to string and push it in list
                        char lnst[32];
                        int len = sm.getLength_values_list(srchnode);
                        sprintf(lnst, "%d" , len);
                        std::string temp;
                        temp.assign(lnst, len);
                        param_list.push_back(temp);
                        param_type = NUM_TYPE;
                    }
                    break;
                }else{
                    er.print_errors(5, lineNumber, index+1, true, false);
                    std::cout<<strbuf[index]<<std::endl;
                    er.error_count++;
                }
            }
        }else{
            er.print_errors(5, lineNumber,index+1, true, false);
            std::cout<<strbuf[index]<<std::endl;
        }
    }else{
        er.print_errors(7, lineNumber, index+1, true, false);
        std::cout<<strbuf[index]<<std::endl;
    }
}


/*
this function reaturns the section type be reading string
as .data or .bss or .text from buffer
*/
int get_section_type(std::string strbuf, int *index)
{
    int type = -1;
    new std::string();
    std::string temp;
    while(*index < (int)strbuf.length()){
        if(isalpha(strbuf[*index]) || strbuf[*index] == '.'){
            temp = temp + strbuf[*index];
            (*index)++;
        }else if(strbuf[*index] == ' ' || strbuf[*index] == '\t'){
            (*index)++;
            continue;
        }else if(strbuf[*index] == '\n'){
            if(!temp.compare(".data")){
                type = DATA_SECTION;
                break;
            }else if(!temp.compare(".bss")){
                type = BSS_SECTION;
                break;
            }else if(!temp.compare(".text")){
                type = TEXT_SECTION;
                break;
            }else{
                er.print_errors(8, lineNumber, (*index)+1,true, false);
                std::cout<<strbuf[*index]<<std::endl;
            }
        }else{
            er.print_errors(9, lineNumber, (*index)+1, false, true);
            return -1;
        }
    }
    return type;
}


/*
this function reads symbol from [] brackets 
*/
void read_symbol_from_cast(std::string strbuf, std::string cast, int *indx)
{
  int index = *indx;
  index++;
  std::string symbol;
  int reg_index, indx_cst;
  bool is_paren_close = false;
  while(index < (int)strbuf.length()){
    if(isalnum(strbuf[index]) || strbuf[index] == '_'){
      symbol = symbol + strbuf[index];
      index++;
    }else if(strbuf[index] == ']'){
      is_paren_close = true;
      break;
    }else if(strbuf[index] == ' ' || strbuf[index] == '\t'){
      index++;
      continue;
    }else if(strbuf[index] == '\n'){
      er.print_errors(19, lineNumber, index+1, false, false);
      std::cout<<strbuf[index]<<std::endl;
      return;
    }else{
      er.print_errors(3, lineNumber, index+1, false, false);
      std::cout<<strbuf[index]<<std::endl;
      return;
    }
  }
  *indx=index+1;
  if(is_paren_close){
    //search the found symbol into symbol table
    SYMTAB_NODE *node = sm.search_node(get_char_ptr(symbol));
    if(node == NULL){
      if(match_with_registers(symbol, &reg_index)){
        if(match_with_casts(cast, &indx_cst)){
            if(reg_size[reg_index] == casts_size[indx_cst]){
              return;
            }else{
              er.print_errors(22, lineNumber, index+1, false, false);
              std::cout<<symbol<<std::endl;
              er.error_count++;
              return;
            }
        }else{
          er.print_errors(18, lineNumber, index+1, false, false);
          std::cout<<symbol<<std::endl;
          er.error_count++;
          return;
        }
      }else{
        er.print_errors(20, lineNumber, index+1, false, false);
        std::cout<<symbol<<std::endl;
        er.error_count++;
        return;
      }
    }else{
      return;
    }
  }
}


/*
this function returns second operand,means data after comma(,) and return it
it is same as read instructions function
*/
std::string read_second_operand(std::string strbuf, std::string first_oprand, int *indx, int *oprnd_count)
{
  int index = *indx;
  std::string oprand_2;
  int reg_index;
  while(index < (int)strbuf.length()){
    if(isalnum(strbuf[index]) || strbuf[index] == '_'){
      oprand_2 = oprand_2 + strbuf[index];
      index++;
    }else if(strbuf[index] == '['){
      if(!oprand_2.compare("byte")){
        operand_2_size = 1;
        read_symbol_from_cast(strbuf, oprand_2, &index);
        (*oprnd_count)++;
        break;
      }else if(!oprand_2.compare("word")){
        operand_2_size = 2;
        read_symbol_from_cast(strbuf, oprand_2, &index);
        (*oprnd_count)++;
        break;
      }else if(!oprand_2.compare("dword")){
        operand_2_size = 4;
        read_symbol_from_cast(strbuf, oprand_2, &index);
        (*oprnd_count)++;
        break;
      }else{
        er.print_errors(18, lineNumber, index+1, false, false);
        std::cout<<strbuf[index]<<std::endl;
        return "";
      }
    }else if(strbuf[index] == ' ' || strbuf[index] == '\t'){
      index++;
      continue;
    }else if(strbuf[index] == '\n'){
      if(match_with_registers(oprand_2, &reg_index)){
        operand_2_size = reg_size[reg_index];
        (*oprnd_count)++;
        return oprand_2;
      }else if(isdigit(oprand_2[0])){
        switch(oprand_2.length()){
          case 1 :
          case 2 :
            operand_2_size = 1;
            break;
          case 3 :
          case 4 :
            operand_2_size = 2;
            break;
          default :
            operand_2_size = 4;
            break;
        }
        (*oprnd_count)++;
        return oprand_2;
      }else{
        SYMTAB_NODE *node = sm.search_node(get_char_ptr(oprand_2));
        if(node == NULL){
          er.print_errors(16, lineNumber, index+1, false, false);
          std::cout<<oprand_2<<std::endl;
          er.error_count++;
          return "";
        }else{
          (*oprnd_count)++;
        }
      }
      break;
    }else{
      er.print_errors(3, lineNumber, index+1, false, false);
      std::cout<<strbuf[index]<<std::endl;
      er.error_count++;
      return "";
    }
  }
  *indx = index;
  return oprand_2;
}


/*
this function searches mnemonic in opcode table for getting size
*/
void calc_inst_address(std::string mnemonic, std::string mnemonic_2, std::string regmem)
{
  std::unordered_set<OPCodeTable, Hash_OpTab>::const_iterator p;
  p = opt.find(OPCodeTable(mnemonic_2, 0, regmem, "", 0, 0, 0));
  if(p != opt.end()){
    if(!last_inst.compare(mnemonic_2)){
      inst_address = (inst_address + (*p).size)*inst_add_cnt;
      inst_add_cnt=1;
  }else{
      if(last_inst.find(get_char_ptr(mnemonic), 0, mnemonic.length()) != std::string::npos){
        if(last_inst.compare(mnemonic_2)){
          inst_address = (inst_address + (*p).size)*inst_add_cnt;
          inst_add_cnt=1;
        }else{
          inst_address = (inst_address + (*p).cmp_size)*inst_add_cnt;
          inst_add_cnt=1;
        }
      }else{
          inst_address = (inst_address + (*p).cmp_size)*inst_add_cnt;
          inst_add_cnt = 1;
          last_inst = mnemonic_2;
      }             
    }
  }
}


/*
this function reads instructions from buffer and validate them
by searhing them into opcode table and other*/
void do_instructions(std::string strbuf, std::string mnemonic, int *indx, int params)
{
  int index = *indx;
  std::string oprand_1, oprand_2;
  int oprnd_count = 0;
  std::unordered_set<OPCodeTable, Hash_OpTab>::const_iterator p;

  if(index == (int)strbuf.length()){
    er.print_errors(21, lineNumber, index+1, true, false);
    std::cout<<strbuf<<std::endl;
  }

  //this loop checks all instructions 
  //if found write them into intermediate file
  //this loop reads and operand_1, and get operand_2 from
  //another function for validation
  while(index < (int)strbuf.length()){
    //if white space, skip it
    if(strbuf[index] == ' ' || strbuf[index] == '\t'){
      index++;
      continue;
    //if any valid char, store it in operand_1
    }else if(isalnum(strbuf[index]) || strbuf[index] == '_'){
      oprand_1 = oprand_1 + strbuf[index];
      index++;
    //if ,
    }else if(strbuf[index] == ','){
      oprnd_count++;
      index++;
      if(oprand_1.empty()){
          er.print_errors(21, lineNumber, index+1, true, false);
          break;
      }else{
        //match operand 1 of mnemonic with registers according to reg size
        //or search it in symbol table
        if(!oprand_1.empty()){
              int reg_index;
              if(match_with_registers(oprand_1, &reg_index)){
                switch(reg_size[reg_index]){
                  case 1 :
                    {
                      std::string mnemonic_2 = mnemonic;
                      mnemonic_2=mnemonic_2+"/r8";
                      operand_1_size = 1;
                      calc_inst_address(mnemonic, mnemonic_2, "/r8");
                    }
                    break;
                  case 2 :
                    {
                      std::string mnemonic_2 = mnemonic;
                      mnemonic_2=mnemonic_2+"/r16";
                      operand_1_size = 2;
                      calc_inst_address(mnemonic, mnemonic_2, "/r16");
                    }
                    break;
                  case 4 :
                    {
                      std::string mnemonic_2 = mnemonic;
                      mnemonic_2=mnemonic_2+"/r32";
                      operand_1_size = 4;
                      calc_inst_address(mnemonic, mnemonic_2, "/r32");
                    }
                    break;
                }
              }else{
                SYMTAB_NODE *node = sm.search_node(get_char_ptr(oprand_1));
                if(node == NULL){
                  er.print_errors(16, lineNumber, index+1, false, false);
                  std::cout<<oprand_2<<std::endl;
                  er.error_count++;
                  return;
                }
              }
            }else{
              if(params != oprnd_count){
                er.print_errors(21, lineNumber, index+1, true, true);
                return;
              }
            }

        //read seconf operand
        oprand_2 = read_second_operand(strbuf, oprand_1, &index, &oprnd_count);
        if(params == oprnd_count){
          check_and_display_errors();
          return;
        }else{
          er.print_errors(21, lineNumber, index+1, true, true);
          return;
        }
      }
      break;
    //if [, then compare for effective address casting
    // by searching symbol into symbol table
    }else if(strbuf[index] == '['){
      //if byte found int operand_1
      if(!oprand_1.compare("byte")){
        operand_1_size = 1;
        oprnd_count++;
        std::string mnemonic_2 = mnemonic;
        mnemonic_2=mnemonic_2+"/m8";
        p = opt.find(OPCodeTable(mnemonic_2,0,"m8","",0,0, 0));
        if(p != opt.end()){
          if(last_inst.empty()){
              inst_address = (inst_address + (*p).size)*inst_add_cnt;
              inst_add_cnt=1;
              last_inst = mnemonic_2;
            }else if(last_inst.find_first_of(get_char_ptr(mnemonic_2), 0, mnemonic.length()) != std::string::npos){
              inst_address = (inst_address + (*p).size)*inst_add_cnt;
              inst_add_cnt=1;
            }else{
              inst_address = (inst_address + (*p).cmp_size)*inst_add_cnt;
              inst_add_cnt = 1;
          }
        }

        //get the symbol from []
        read_symbol_from_cast(strbuf, oprand_1, &index);
        //peek next craracter, if it is , then get second operand
        //otherwise check validation
        char next_char = peek_next_char(strbuf, &index);
        switch(next_char){
          case ',' :
            oprand_2 = read_second_operand(strbuf, oprand_1, &index, &oprnd_count);
            if(params == oprnd_count){
              check_and_display_errors();
              return;
            }else{
              er.print_errors(21, lineNumber, index+1, true, true);
              return;
            }
            break;

          case '\n' :
            if(params == oprnd_count){
              check_and_display_errors();
              return;
            }else{
              er.print_errors(21, lineNumber, index+1, true, true);
              return;
            }
            break;
        }
        index++;
        continue;
      //if word found int operand_1
      }else if(!oprand_1.compare("word")){
        operand_1_size = 2;
        oprnd_count++;
        std::string mnemonic_2 = mnemonic;
        mnemonic_2=mnemonic_2+"/m16";
        p = opt.find(OPCodeTable(mnemonic_2,0,"m16","",0,0, 0));
        if(p != opt.end()){
          if(last_inst.empty()){
            inst_address = (inst_address+(*p).size+2)*inst_add_cnt;
            inst_add_cnt = 1;
            last_inst = mnemonic_2;
          }else if(last_inst.find_first_of(get_char_ptr(mnemonic_2), 0, mnemonic.length()) != std::string::npos){
            inst_address = (inst_address+(*p).size+2)*inst_add_cnt;
            inst_add_cnt = 1;
          }else{
            inst_address = (inst_address + (*p).cmp_size)*inst_add_cnt;
            inst_add_cnt = 1;
          }
        }
        read_symbol_from_cast(strbuf, oprand_1, &index);
        char next_char = peek_next_char(strbuf, &index);
        switch(next_char){
          case ',' :
            oprand_2 = read_second_operand(strbuf, oprand_1, &index, &oprnd_count);
            if(params == oprnd_count){
              check_and_display_errors();
              return;
            }else{
              er.print_errors(21, lineNumber, index+1, true, true);
              return;
            }
            break;

          case '\n' :
            if(params == oprnd_count){
              check_and_display_errors();
              return;
            }else{
              er.print_errors(21, lineNumber, index+1, true, true);
              return;
            }
            break;
        }
        index++;
        continue;
      //if dword found int operand_1
      }else if(!oprand_1.compare("dword")){
        operand_1_size = 4;
        oprnd_count++;
        std::string mnemonic_2 = mnemonic;
        mnemonic_2=mnemonic_2+"/m32";
        p = opt.find(OPCodeTable(mnemonic_2,0,"m32","",0,0, 0));
        if(p != opt.end()){
          if(last_inst.empty()){
            inst_address = (inst_address+(*p).size)*inst_add_cnt;
            inst_add_cnt = 1;
            last_inst = mnemonic_2;
          }else if(last_inst.find_first_of(get_char_ptr(mnemonic_2), 0, mnemonic.length()) != std::string::npos){
            inst_address = (inst_address+(*p).size)*inst_add_cnt;
            inst_add_cnt = 1;
          }else{
            inst_address = (inst_address + (*p).cmp_size)*inst_add_cnt;
            inst_add_cnt = 1;
          }
        }
        read_symbol_from_cast(strbuf, oprand_1, &index);
        char next_char = peek_next_char(strbuf, &index);
        switch(next_char){
          case ',' :
            oprand_2 = read_second_operand(strbuf, oprand_1, &index, &oprnd_count);
            if(params == oprnd_count){
              check_and_display_errors();
              return;
            }else{
              er.print_errors(21, lineNumber, index+1, true, true);
              return;
            }
            break;

          case '\n' :
            if(params == oprnd_count){
              check_and_display_errors();
              return;
            }else{
              er.print_errors(21, lineNumber, index+1, true, true);
              return;
            }
            break;
        }
        index++;
        continue;
      }else{
        er.print_errors(18, lineNumber, index+1, false, false);
        er.error_count++;
        std::cout<<strbuf[index]<<std::endl;
        break;
      }
    //if \n found then calculate address of each instruction
    }else if(strbuf[index] == '\n'){
      if(!oprand_1.empty()){
        int reg_index;
        if(match_with_registers(oprand_1, &reg_index)){
          switch(reg_size[reg_index]){
            case 1 :
              {
                std::string mnemonic_2 = mnemonic;
                mnemonic_2=mnemonic_2+"/r8";
                operand_1_size = 1;
                oprnd_count++;
                calc_inst_address(mnemonic, mnemonic_2, "/r8");
              }
              break;
            case 2 :
              {
                std::string mnemonic_2 = mnemonic;
                mnemonic_2=mnemonic_2+"/r16";
                operand_1_size = 2;
                oprnd_count++;
                calc_inst_address(mnemonic, mnemonic_2, "/r16");
              }
              break;
            case 4 :
              {
                std::string mnemonic_2 = mnemonic;
                mnemonic_2=mnemonic_2+"/r32";
                operand_1_size = 4;
                oprnd_count++;
                calc_inst_address(mnemonic, mnemonic_2, "/r32");
              }
              break;
          }
        }else{
          std::string mnemonic_2 = mnemonic;
          mnemonic_2=mnemonic_2+"/r32";
          operand_1_size = 4;
          calc_inst_address(mnemonic, mnemonic_2, "/r32");
        }
      }else{
        if(params != oprnd_count){
          er.print_errors(21, lineNumber, index+1, true, true);
          return;
        }
      }
      break;
    }else if(strbuf[index] == ';'){
      remove_comments(strbuf, &index);
    }else{
      er.print_errors(3, lineNumber, index+1, true, false);
      std::cout<<strbuf[index]<<std::endl;
      std::cout<<"\n";
      break;
    }
  }
}


/*
this function reads each declaration from buffer,
parse it according to their meaning such as data/reserve declaraion etc
and call appropriate methods for validation and to read them
*/
void read_declarations_pass_1(std::string strbuf, SymbolTable &s, std::ofstream &outfile)
{
    int index = 0, indx = 0, sect_type;;
    int datatype_index, restype_index;
    std::string label;
    //variable to check if no more than colon and lable found
    bool has_colon_found = false, has_label_found = false;

    if(strbuf.empty())
        return;

    // this loops first reads the declared label or symbol
    while(index < (int)strbuf.length()){
        // if space or \t, then set gblabel to label
        if(strbuf[index] == ' ' || strbuf[index] == '\t'){
            if(! strbuf.empty()){
                //if lable contain a section string then set last_section_type to 
                //appropriate section, this block of code checks only for sections defined 
                if(!label.compare("section")){
                    index++;
                    sect_type = get_section_type(strbuf, &index);
                    switch(sect_type){
                        case DATA_SECTION :
                            last_section_type = DATA_SECTION;
                            data_section_found = true;
                            bss_section_found = false;
                            text_section_found = false;
                            if(currInstNode!=NULL){
                              currInstNode->type = 0;
                            }else{
                              outfile<<lineNumber<<"###"<<Buffer<<"#"<<std::endl;
                            }
                            break;
                        case BSS_SECTION :
                            last_section_type = BSS_SECTION;
                            bss_section_found = true;
                            data_section_found = false;
                            text_section_found = false;
                            if(currInstNode!=NULL){
                              currInstNode->type = 0;
                            }else{
                              outfile<<lineNumber<<"###"<<Buffer<<"#"<<std::endl;
                            }
                            break;
                        case TEXT_SECTION :
                            last_section_type = TEXT_SECTION;
                            text_section_found = true;
                            bss_section_found = false;
                            data_section_found = false;
                            data_address = 0x00000000;
                            bss_address = 0x00000000;
                            if(currInstNode!=NULL){
                              currInstNode->type = 0;
                            }else{
                              currInstNode->type = 1;
                            }
                            break;
                        default :
                            break;
                    }
                    goto exit_loop_1;
                }

                //this block of code checks the declared labels
                //according to their section type where they belong otherwise print error
                if(data_section_found){
                    gblabel = label;
                    has_label_found = true;
                }else if(bss_section_found){
                    gblabel = label;
                    has_label_found = true;
                }else if(text_section_found){
                    //seek for colon
                    if(seek_colon(strbuf, index)){
                        gblabel = label;
                        lab_decl_type = LAB_DECL;
                        has_label_found = true;
                    }else{
                        if(!label.empty()){
                          //search read mnemonic in OPCodeTable
                          std::unordered_set<OPCodeTable, Hash_OpTab>::const_iterator p = opt.find(OPCodeTable(label,0,"","",0,0, 0));
                          if(p == opt.end()){
                            er.print_errors(17, lineNumber, index+1, false, false);
                            std::cout<<label<<std::endl;
                            er.error_count++;
                            return;
                          }else{
                            //if section was text, then reads instructions
                            do_instructions(strbuf, label, &index, (*p).params);
                            //if label contains extern then add it to symbol table
                            //otherwise write an intermediate file
                            if(!label.compare("extern")){
                              skip_spaces(strbuf, &index);
                              gblabel = get_global_extern_symbol(strbuf, &index);
                            }else{
                              strbuf.replace(strbuf.length()-1, 1, "#");
                              outfile<<lineNumber<<"#"<<decimal_to_hex(inst_address)<<"##"<<strbuf<<std::endl;
                            }                            
                            return;
                          }
                        }
                    }
                }else{
                    er.print_errors(10, lineNumber, index+1, true, true);
                }
                label.clear();
                indx = index;
                break;
            }

        // if : , then set gblabel to label
        }else if(strbuf[index] == ':'){
            if(! strbuf.empty()){
                gblabel = label;
                lab_decl_type = LAB_DECL;
                has_colon_found = true;
                label.clear();
                indx = index;
                if(last_section_type == TEXT_SECTION){
                  std::string line = strbuf;
                  line.replace(line.length()-1, 1,"#");
                  outfile<<lineNumber<<"###"<<line<<std::endl;
                }
                break;
            }
        // if is alphabet/digit then store it in label
        }else if(isalnum(strbuf[index]) || strbuf[index] == '_'){
            label = label + strbuf[index];
            index++;
        // if \n , then set gblabel to label
        }else if(strbuf[index] == '\n'){
            if(data_section_found || bss_section_found){
                    gblabel = label;
                }else if(text_section_found){
                  //search read mnemonic in OPCodeTable
                  if(!label.empty()){
                    std::unordered_set<OPCodeTable, Hash_OpTab>::const_iterator p2 = opt.find(OPCodeTable(label,0,"","",0,0, 0));
                    if(p2 == opt.end()){
                      er.print_errors(17, lineNumber, index+1, false, false);
                      std::cout<<label<<std::endl;
                      er.error_count++;
                      return;
                    }else{
                      //if section was text, then reads instructions
                      do_instructions(strbuf, label, &index, (*p2).params);
                      //write instructions buffer to intermediate file
                      std::string line = strbuf;
                      line.replace(line.length()-1, 1,"#");
                      outfile<<lineNumber<<"#"<<decimal_to_hex(inst_address)<<"##";
                      outfile<<line<<std::endl;
                      return;
                    } 
                  }
              }else{
                  er.print_errors(10, lineNumber, index+1, true, true);
              }
            break;
        //if ; then remove comments
        }else if(strbuf[index] == ';'){
            remove_comments(strbuf, &index);
        }else{
            er.print_errors(3, lineNumber, index+1, false, false);
            std::cout<<strbuf[index]<<std::endl;
            er.error_count++;
            return;
        }
    }

//job of reading labels and instructions is done

exit_loop_1:

    if(sect_type > 0)
        return;

    // get next character after reading label
    indx = index+1;
    std::string type;
    
    skip_spaces(strbuf, &indx);
    
    // this loop reads the declared type db/dw/dd/resd/resb etc
    // if found, call associated function for it
    while(indx < (int)strbuf.length()){
        if(strbuf[indx] == ':'){
            skip_spaces(strbuf, &indx);
            if(has_colon_found){
                er.print_errors(11, lineNumber, index+1, false, true);
                er.error_count++;
                return;
            }else{
                has_colon_found  = true;
                if(last_section_type == TEXT_SECTION){
                  std::string line = strbuf;
                  line.replace(line.length()-1, 1,"#");
                  outfile<<lineNumber<<"###"<<line<<std::endl;
                }
                if(!has_label_found && !has_colon_found){
                    er.print_errors(3, lineNumber, index+1, false, false);
                    std::cout<<strbuf[indx]<<std::endl;
                    er.error_count++;
                    return;
                }
            }
        }else if(isalnum(strbuf[indx])){
            type = type + strbuf[indx];
            
        // if space , match type with datatypes, if matched, call function
        // otherwise matched with resv types, call function
        // otherwise 
        }else if((strbuf[indx]) == ' ' || strbuf[indx] == '\t'){
            if(!type.empty()){
                //match next declaration with data types or print error
                if(has_match_data_types(type, &datatype_index)){
                    if(last_section_type == DATA_SECTION){
                        type_decl_size = sizes[datatype_index];
                        read_string_number(strbuf, indx+1);
                        lab_decl_type = VAR_DECL;
                    }else{
                        if(last_section_type == BSS_SECTION){
                            er.print_errors(12, lineNumber, index+1, true, true);
                        }else if(last_section_type == TEXT_SECTION){
                            er.print_errors(13, lineNumber, index+1, true, true);
                        }
                    }
                    break;
                //match next declaration with reserved types or print error
                }else if(has_match_res_types(type, &restype_index)){
                    if(last_section_type == BSS_SECTION){
                        type_decl_size = sizes[restype_index];
                        read_resType_number(strbuf, indx);
                        lab_decl_type = RES_DECL;
                    }else{
                        if(last_section_type == DATA_SECTION){
                            er.print_errors(14, lineNumber, index+1, true, true);
                            er.error_count++;
                            return;
                        }else if(last_section_type == TEXT_SECTION){
                            er.print_errors(13, lineNumber, index+1, true, true);
                        }
                    }
                    break;
                }else if(type.compare("equ") == 0){
                    read_EQU_cal_len(strbuf, index+4);
                    lab_decl_type = VAR_DECL;
                    break;
                }else{
                    gblabel.clear();
                    lab_decl_type = LAB_DECL;
                }
            }else{
                indx++;
                continue;
            }
            type.clear();
        }else if(strbuf[indx] == ';'){
            remove_comments(strbuf, &indx);
        }else if(strbuf[indx] == '\n'){
          break;
        }else{
            if(!gblabel.empty()){
              return;
            }else{
              er.print_errors(3, lineNumber, indx+1, false, false);
              std::cout<<strbuf[indx]<<std::endl;
              er.error_count++;
              return;
            }
        }
        indx++;
    }
}



/*
This function performs the pass 1 operation of the assembler
function takes two strings of filenames, one is source and other is intermediate file
*/
void do_pass_1(const std::string filename, const std::string interm_file)
{
    /*index = for column in errors display
    lst_size = for size of defined data
    strbuf = buffer for storing data
    */
    int index = 0, lst_size;
    std::string strbuf;

    //open files in read/write mode
    std::ifstream infile(filename, std::ios::in);
    std::ofstream outfile(interm_file, std::ios::out);

    //initialize all mnemonic codes from file optab.cpp
    init_mnemonics_opcodes();

loop_start:

    // read each line from a file
    while(infile.getline(Buffer, 255)){
        lineNumber++;

        //assign it to strbuf
        strbuf.assign(Buffer, strlen(Buffer));

        if(!strbuf.empty()){

            //if buffer is full of whitespaces then write #### to intermediate file
            if(check_are_all_white_spaces(strbuf)){
              outfile<<lineNumber<<"####"<<std::endl;
            }

            //add new line to buffer
            strbuf = strbuf+'\n';
            
            // first trim the buffer
            strbuf = trim(strbuf);
            strbuf = trim_2(strbuf);

            //even if buffer contain any white spaces then goto loop_start
            if(check_are_all_white_spaces(strbuf)){
              goto loop_start;
            }

            //call all methods on the buffer
            read_declarations_pass_1(strbuf, sm, outfile);

            //storing the information in the symbol table here
            if(! gblabel.empty()){
                char *slb = get_char_ptr(gblabel);
                //first check symbol already exists in symbol table or not
                if(sm.search(slb)){
                    er.print_errors(15, lineNumber, index+1, false, false);
                    std::cout<<"' "<<gblabel<<" '"<<std::endl;
                    er.error_count++;
                }else{
                    // otherwise insert symbol into symbol table
                    sm.insert(slb, lineNumber, 0, false, 0, 0);
                    
                    //set other options such as size and type
                    if(currInstNode!=NULL){
                        currInstNode->size = type_decl_size;
                        currInstNode->type = lab_decl_type;
                        lab_decl_type = 0;
                    }
                }
            }

            // get each values from param_list and insert in into symbol table
            // associated with current label
            std::list<std::string>::iterator p = param_list.begin();
            while(p != param_list.end()){
                if(currInstNode!=NULL){
                    sm.insert_value(&(currInstNode->values_list), get_char_ptr(*p), param_type);
                }
                p++;
            }

            // sets the values for is_array and array_size by calculating it
            // for string type and num type
            if(currInstNode != NULL){
                if(! param_list.empty()){
                    lst_size = param_list.size();
                    if(lst_size > 1){
                        currInstNode->is_array = true;
                        currInstNode->array_size = lst_size * type_decl_size;
                        if((currInstNode->values_list)->type == STR_TYPE){
                            currInstNode->is_array = true;
                            currInstNode->array_size = sm.getLength_values_list(currInstNode) * type_decl_size;                            
                        }
                    }else if(lst_size == 1){
                        if((currInstNode->values_list)->type == STR_TYPE){
                            currInstNode->is_array = true;
                            currInstNode->array_size = sm.getLength_values_list(currInstNode) * type_decl_size;
                        }
                        if(currInstNode->type == RES_DECL){
                            currInstNode->is_array = true;
                            currInstNode->array_size = std::stoi(*param_list.begin()) * type_decl_size;
                        }
                    }
                }
            }

            //insert addresses into symbol associated to the symbol
            //according to their types
            if(currInstNode != NULL){
              currInstNode->addr_list = new std::list<int>;
              switch(currInstNode->type){
                case VAR_DECL :
                  {
                    if(currInstNode->addr_list != NULL){
                      if(currInstNode->is_array){
                        (currInstNode->addr_list)->push_back(data_address);
                        data_address += currInstNode->array_size;
                      }else{
                        (currInstNode->addr_list)->push_back(data_address);
                        data_address += currInstNode->size;
                      }
                      outfile<<lineNumber<<"#"<<decimal_to_hex(*((currInstNode->addr_list)->begin()))<<"#"
                      <<gblabel<<"#"<<Buffer<<"#"<<std::endl;
                    }
                  }
                  break;

                case RES_DECL :
                  {
                      if(currInstNode->addr_list != NULL){
                        if(bss_address == 0){
                          (currInstNode->addr_list)->push_back(bss_address);
                        }else{
                          int bss_addr = bss_address;
                          bss_addr = bss_addr - currInstNode->array_size;
                          if(bss_addr >= 0){
                            bss_address = bss_addr;
                            (currInstNode->addr_list)->push_back(bss_address);
                          }else{
                            (currInstNode->addr_list)->push_back(bss_address);
                          }
                        }
                        outfile<<lineNumber<<"#"<<decimal_to_hex(*((currInstNode->addr_list)->begin()))<<"#"<<
                        gblabel<<"#"<<Buffer<<"#"<<std::endl;
                        bss_address += currInstNode->array_size;
                      }
                  }
                  break;

                case 0 :
                  {
                    outfile<<lineNumber<<"###"<<Buffer<<"#"<<std::endl;
                  }
                  break;
              }
            }

            type_decl_size = 0;

            param_list.clear();
            gblabel.clear();
            strbuf.clear();
            clear_buffer(Buffer);
        }else{
          outfile<<lineNumber<<"###"<<Buffer<<"#"<<std::endl;
        }
    }

    //check count of error_count variable in er object
    if(er.error_count > 0)
        exit(0);
    
    infile.close();
    outfile.close();

}


