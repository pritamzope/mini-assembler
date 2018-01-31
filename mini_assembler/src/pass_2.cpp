#include<iostream>
#include<fstream>
#include<list>
#include<cstdlib>
#include<algorithm>
#include<cctype>
#include<cstring>
#include"symtab.hpp"
#include"errortab.hpp"
#include"optab.hpp"
#include"pass_1.hpp"
#include"pass_2.hpp"


extern SymbolTable sm;
extern ErrorTable er;

extern std::unordered_set<OPCodeTable, Hash_OpTab> opt;
extern char Buffer[255];

static int section_type = -1;
static int addr_count = 0;
std::vector<std::string> lst_spaces={"     ","          " ,
                                    "                                  ",
                                    "         "};

extern std::vector<std::string> registers;
extern std::vector<int> reg_size;
extern std::vector<int> casts_size;

std::vector<int> registers_decm_opcode = {0, 0, 0,
                                          1, 1, 1,
                                          2, 2, 2,
                                          3, 3, 3,
                                          4, 4, 4,
                                          5, 5, 5,
                                          6, 6, 6,
                                          7, 7, 7};


int get_reg_decm_opcode(std::string reg)
{
  int code=0;
  int index=0;
  if(match_with_registers(reg, &index)){
    code = registers_decm_opcode[index];
  }
  return code;
}


int get_cast_size(std::string cast)
{
  int size=0;
  int indx=0;
  if(match_with_casts(cast, &indx)){
    size = casts_size[indx];
  }
  return size;
}


std::string get_8byte_hex_value(std::string hexv)
{
  std::string hex_value;
  for(int i=0; i < (int)(8 - hexv.length()); i++){
    hex_value.push_back('0');
  }
  hex_value = hex_value + hexv;
  return hex_value;
}


std::string get_str_type_hex_values(VAL_LIST_NODE *node)
{
  std::string hex_value;
  if(node != NULL){
    for(int i=0; i < (int)strlen(node->value); i++){
      hex_value = hex_value + decimal_to_hex((node->value)[i]);
    }
    if(node->next != NULL){
      hex_value = hex_value + '0';
      hex_value = hex_value + decimal_to_hex(atoi((node->next)->value));
    }
  }
  return hex_value;
}


std::string get_num_type_hex_values(VAL_LIST_NODE *node, int byte, std::string linenumber)
{
  std::string hex_value, hex_num;
  while(node != NULL){
    hex_num = decimal_to_hex(atoi(node->value));
    switch(byte){
      case 1 :
        if(hex_num.length() < 2){
          hex_num = hex_num + '0';
        }else if(hex_num.length() > 2){
          std::cout<<"warning :"<<linenumber<<": byte data exceeds bounds"<<std::endl;
        }
        break;
      case 2 :
        if(hex_num.length() < 4){
          std::string temp;
          if(hex_num.length() < 2){
            hex_num = '0' + hex_num;
            for(int i=0;i <(int)(4-hex_num.length());i++){
              temp = temp + '0';
            }
            hex_num = hex_num + temp;
          }else{
            for(int i=0;i < (int)(4-hex_num.length());i++){
              temp = temp + '0';
            }
            hex_num = hex_num + temp;
          }
        }else{
          std::cout<<"warning :"<<linenumber<<": word data exceeds bounds"<<std::endl;
        }
        break;
      case 4 :
        if(hex_num.length() < 8){
          std::string temp;
          if(hex_num.length() < 2){
            hex_num = '0' + hex_num;
            for(int i=0;i < (int)(8-hex_num.length());i++){
              temp = temp + '0';
            }
            hex_num = hex_num + temp;
          }else{
            for(int i=0;i < (int)(8-hex_num.length());i++){
              temp = temp + '0';
            }
            hex_num = hex_num + temp;
          }
        }else{
          std::cout<<"warning :"<<linenumber<<": dword data exceeds bounds"<<std::endl;
        }
        break;
    }
    hex_value = hex_value + hex_num;
    node = node->next;
  }
  return hex_value;
}


std::string get_hex_value_by_byte(int byte, SYMTAB_NODE *node, std::string linenumber)
{
  std::string hex_value;
  switch(byte){
    case 1 :
      if((node->values_list)->type == NUM_TYPE){
        hex_value = get_num_type_hex_values(node->values_list, 1, linenumber);
      }else if((node->values_list)->type == STR_TYPE){
        hex_value = get_str_type_hex_values(node->values_list);
      }
      break;
    case 2 :
      if((node->values_list)->type == NUM_TYPE){
        hex_value = get_num_type_hex_values(node->values_list, 2, linenumber);
      }else if((node->values_list)->type == STR_TYPE){
        hex_value = get_str_type_hex_values(node->values_list);
      }
      break;
    case 4 :
      if((node->values_list)->type == NUM_TYPE){
        hex_value = get_num_type_hex_values(node->values_list, 4, linenumber);
      }else if((node->values_list)->type == STR_TYPE){
        hex_value = get_str_type_hex_values(node->values_list);
      }
      break;
  }
  return hex_value;
}


std::string get_data(std::string strbuf, int *index)
{
  std::string data;
  while(*index < (int)strbuf.length()){
    if(isalnum(strbuf[*index])){
      data = data+strbuf[*index];
      (*index)++;
    }else if(strbuf[*index] == '#'){
      break;
    }else{
      data=data+strbuf[*index];
      (*index)++;
    }
  }
  if(*index == 0){
    data = "";
  }
  (*index)++;
  return data;
}


std::string get_spaces(int n)
{
  std::string space;
  for(int i=0;i<n;i++)
    space = space + ' ';
  return space;
}


static bool consume_comma(std::string strbuf, int *indx)
{
  bool result = false;
  int index = *indx;
  while(index < (int)strbuf.length()){
    if(strbuf[index] == ','){
      result = true;
      break;
    }else if(strbuf[index] == ' ' || strbuf[index] == '\t'){
      index++;
      continue;
    }else{
      result = false;
      break;
    }
  }
  index++;
  *indx = index;
  return result;
}


static std::string get_mnemonic(std::string strbuf, int *indx)
{
  int index = *indx;
  std::string mnemonic;
  while(index < (int)strbuf.length()){
    if(isalnum(strbuf[index])){
      mnemonic = mnemonic+strbuf[index];
      index++;
    }else if(strbuf[index] == ' ' || strbuf[index] == '\t'){
      break;
    }else if(strbuf[index] == '#'){
      index++;
      break;
    }
  }
  *indx = index;
  return mnemonic;
}


/*
this function reads .data section address and write it to lst file
*/
void write_datas_to_file(std::ofstream &outfile, std::string linenumber, SYMTAB_NODE *node, std::string address, std::string text)
{
  std::string hex_data = get_hex_value_by_byte(node->size, node, linenumber);
  int len = hex_data.length();
  int index = 0, index_2 = len;
  bool is_text_print = false;
  int addr = hex_to_decimal(address);

  if(len <= 18){
    outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<get_8byte_hex_value(address)<<" ";
    for(int i=index; i<index_2; i++){
      outfile<<hex_data[i];
    }
    if(!is_text_print){
      outfile<<get_spaces((18-len)+5)<<text<<std::endl;
      is_text_print = true;
    }
  }else{
    while(index_2 >=0){
      if(index+18 <= len){
        addr += addr_count*9;
        outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<get_8byte_hex_value(decimal_to_hex(addr))<<" ";
        addr_count=1;
        for(int i=index; i<index+18; i++){
          outfile<<hex_data[i];
        }
        if(index <= index_2 && is_text_print){
          outfile<<"-"<<std::endl;
        }
        if(!is_text_print){
          outfile<<get_spaces(5)<<text<<std::endl;
          is_text_print = true;
        }
      }else{
        if(index >= len){
          outfile<<std::endl;
          break;
        }
        addr += addr_count*9;
        outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<get_8byte_hex_value(address)<<" ";
        addr_count = 1;
        for(int i=index; i<index+4; i++){
          outfile<<hex_data[i];
        }
        outfile<<std::endl;
        break;
      }
      index_2 -= 18;
      index += 18;
    }
  }
}


std::string get_symbol_cast(std::string text, int *indx)
{
  int index = *indx;
  std::string symbol;
  while(index < (int)text.length()){
    if(isalnum(text[index])){
      symbol = symbol+text[index];
      index++;
    }else if(text[index] == ' ' || text[index] == '\t'){
      if(!symbol.empty()){
        break;
      }else{
        index++;
        continue;
      }
    }else if(text[index] == ']'){
      break;
    }
  }
  index++;
  *indx = index;
  return symbol;
}


/*
this function returns the operand read from text
*/
std::string get_operand(std::string text, int *indx, bool *is_cast, int *cast_size)
{
  int index = *indx, cst_sz_indx;
  std::string operand;
  while(index < (int)text.length()){
    if(isalnum(text[index])){
      operand = operand+text[index];
      index++;
    }else if(text[index] == ' ' || text[index] == '\t'){
      if(!operand.empty()){
        *is_cast = false;
        break;
      }else{
        index++;
        continue;
      }
    }else if(text[index] == ','){
      *is_cast = false;
      break;
    }else if(text[index] == '#'){
      *is_cast = false;
      break;
    }else if(text[index] == '['){
      if(match_with_casts(operand, &cst_sz_indx)){
        *cast_size = casts_size[cst_sz_indx];
      }
      *is_cast = true;
      break;
    }else{
      break;
    }
  }
  index++;
  *indx = index;
  return operand;
}


/*
this function assemble the instructions for each operand, size and effective addresses
*/
void assemble_instructions(std::string text, std::string linenumber, std::string address, std::ofstream &outfile)
{
  int index = 0;
  int mnemonic_params = 0;
  std::string mnemonic = get_mnemonic(text, &index);
  std::string operand_1, operand_2, cast_symbol, cast_symbol_2;
  bool is_cast = false, is_cast_2 = false;
  int cast_size, cast_size_2;
  std::unordered_set<OPCodeTable, Hash_OpTab>::const_iterator p = opt.find(OPCodeTable(mnemonic,0,"","",0,0, 0));
  std::unordered_set<OPCodeTable, Hash_OpTab>::const_iterator p2;

  if(p != opt.end()){
    mnemonic_params = (*p).params;
  }
  
  //first check for parameters of mnemonic
  switch(mnemonic_params){
    case 0 :
      {
        operand_1 = get_operand(text, &index, &is_cast, &cast_size);
        if(operand_1.compare("")){
          er.print_errors(6, atoi(get_char_ptr(linenumber)), index+1, true, true);
        }else{
          std::string complt_inst = mnemonic;
          std::string opcode;
          complt_inst = complt_inst + "/r32";
          p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
          if(p2 != opt.end()){
            opcode = opcode + (*p2).opcode;
          }
          outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
          get_8byte_hex_value(address)<<" "<<
          opcode<<get_spaces(32-opcode.length())<<text<<std::endl;
        }
      }
      break;

    case 1 :
      {
        int reg_index;
        operand_1 = get_operand(text, &index, &is_cast, &cast_size);
        if(is_cast){
          cast_symbol = get_symbol_cast(text, &index);
          char *sym = get_char_ptr(cast_symbol);
          SYMTAB_NODE *node = sm.search_node(sym);
          if(node == NULL){
            if(!match_with_registers(operand_1, &reg_index)){
              
            }else{
              er.print_errors(16, atoi(get_char_ptr(linenumber)), index+1, false, false);
              std::cout<<sym<<std::endl;
              er.error_count++;
            }
          }else{
            std::string complt_inst = mnemonic;
            std::string opcode;
            switch(cast_size){
              case 1 :
                {
                  complt_inst = complt_inst + "/m8";
                  p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                  if(p2 != opt.end()){
                    opcode = opcode + (*p2).opcode;
                    opcode = opcode + "[";
                    std::list<int>::iterator lst_p = node->addr_list->begin();
                    opcode = opcode + decimal_to_hex(*lst_p);
                    opcode = opcode + "00]";
                  }
                  outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
                  get_8byte_hex_value(address)<<" "<<
                  opcode<<get_spaces(32-opcode.length())<<text<<std::endl;
                }
                break;
              case 2 :
                {
                  complt_inst = complt_inst + "/m16";
                  p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                  if(p2 != opt.end()){
                    opcode = opcode + (*p2).opcode;
                    opcode = opcode + "[";
                    std::list<int>::iterator lst_p = node->addr_list->begin();
                    opcode = opcode + decimal_to_hex(*lst_p);
                    opcode = opcode + "00]";
                  }
                  outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
                  get_8byte_hex_value(address)<<" "<<
                  opcode<<get_spaces(32-opcode.length())<<text<<std::endl;
                }
                break;
              case 4 :
                {
                  complt_inst = complt_inst + "/m32";
                  p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                  if(p2 != opt.end()){
                    opcode = opcode + (*p2).opcode;
                    opcode = opcode + "[";
                    std::list<int>::iterator lst_p = node->addr_list->begin();
                    opcode = opcode + decimal_to_hex(*lst_p);
                    opcode = opcode + "00]";
                  }
                  outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
                  get_8byte_hex_value(address)<<" "<<
                  opcode<<get_spaces(32-opcode.length())<<text<<std::endl;
                }
                break;
            }
          }
        }else{
          std::string complt_inst = mnemonic;
          std::string opcode;
          if(match_with_registers(operand_1, &reg_index)){  
            switch(reg_size[reg_index]){
              case 1 :
                {
                  complt_inst = complt_inst + "/r8";
                  p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                  if(p2 != opt.end()){
                    opcode = opcode + (*p2).opcode;
                    opcode = opcode + decimal_to_hex(get_reg_decm_opcode(operand_1) + (*p2).start_code_size);
                  }
                  outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
                  get_8byte_hex_value(address)<<" "<<
                  opcode<<get_spaces(32-opcode.length())<<text<<std::endl;
                }
                break;
              case 2 :
                {
                  complt_inst = complt_inst + "/r16";
                  p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                  if(p2 != opt.end()){
                    opcode = opcode + (*p2).opcode;
                    opcode = opcode + decimal_to_hex(get_reg_decm_opcode(operand_1) + (*p2).start_code_size);
                  }
                  outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
                  get_8byte_hex_value(address)<<" "<<
                  opcode<<get_spaces(32-opcode.length())<<text<<std::endl;
                }
                break;
              case 4 :
                {
                  complt_inst = complt_inst + "/r32";
                  p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                  if(p2 != opt.end()){
                    opcode = opcode + (*p2).opcode;
                    opcode = opcode + decimal_to_hex(get_reg_decm_opcode(operand_1) + (*p2).start_code_size);
                  }
                  outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
                  get_8byte_hex_value(address)<<" "<<
                  opcode<<get_spaces(32-opcode.length())<<text<<std::endl;
                }
                break;
            }
          }else{
            SYMTAB_NODE *node = sm.search_node(get_char_ptr(operand_1));
            if(node == NULL){
              er.print_errors(16, atoi(get_char_ptr(linenumber)), index+1, false, false);
              std::cout<<get_char_ptr(operand_1)<<std::endl;
              exit(0);
            }else{
              complt_inst = complt_inst + "/_r8_";
              p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
              if(p2 != opt.end()){
                opcode = opcode + (*p2).opcode;
              }
              outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
              get_8byte_hex_value(address)<<" "<<
              opcode<<get_spaces(32-opcode.length())<<text<<std::endl;
              return;
            }
            complt_inst = complt_inst + "/_r8_";
            p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
            if(p2 != opt.end()){
              opcode = opcode + (*p2).opcode;
            }
            outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
            get_8byte_hex_value(address)<<" "<<
            opcode<<get_spaces(32-opcode.length())<<text<<std::endl;
          }
        }
        is_cast = false;
      }
      break;


    case 2 :
      {
        int reg_index;
        operand_1 = get_operand(text, &index, &is_cast, &cast_size);
        if(is_cast){
          cast_symbol = get_symbol_cast(text, &index);
          consume_comma(text, &index);
          operand_2 = get_operand(text, &index, &is_cast_2, &cast_size_2);

          if(!operand_2.compare("")){
            er.print_errors(21, atoi(get_char_ptr(linenumber)), index+1, true, true);
            er.error_count++;
          }
          if(is_cast_2){
            er.print_errors(6, atoi(get_char_ptr(linenumber)), index+1, true, true);
            er.error_count++;
            return;
          }else{
            char *sym = get_char_ptr(cast_symbol);
            SYMTAB_NODE *node = sm.search_node(sym);
            if(node == NULL){
              if(!match_with_registers(operand_1, &reg_index)){
                
              }else{
                er.print_errors(16, atoi(get_char_ptr(linenumber)), index+1, false, false);
                std::cout<<sym<<std::endl;
                er.error_count++;
              }
            }else{
              std::string complt_inst = mnemonic;
              std::string opcode;
              switch(cast_size){
                case 1 :
                  {
                    complt_inst = complt_inst + "/_moff8_";
                    if(!operand_2.compare("al")){
                      complt_inst = complt_inst + "_al_";
                      p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                      if(p2 != opt.end()){
                        opcode = opcode + (*p2).opcode;
                        opcode = opcode + "[";
                        std::list<int>::iterator lst_p = node->addr_list->begin();
                        opcode = opcode + decimal_to_hex(*lst_p);
                        opcode = opcode + "00]";
                      }
                      outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
                      get_8byte_hex_value(address)<<" "<<
                      opcode<<get_spaces(32-opcode.length())<<text<<std::endl;

                    }else if(isdigit(operand_2[0])){
                      complt_inst = complt_inst + "_imm8_";
                      p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                      if(p2 != opt.end()){
                        opcode = opcode + (*p2).opcode;
                        opcode = opcode + decimal_to_hex(get_reg_decm_opcode(operand_1) + (*p2).start_code_size);
                        opcode.insert(opcode.length(),"[");
                        opcode = opcode + decimal_to_hex(*(node->addr_list->begin()));
                        opcode.insert(opcode.length(),"00]");
                        std::string hexval = decimal_to_hex(atoi(get_char_ptr(operand_2)));
                        if(hexval.length() < 2){
                          hexval.insert(0,"0");
                          hexval.insert(hexval.length(), "000000", 6);
                        }else{
                          hexval.insert(hexval.length(), "000000", 6);
                        }
                        opcode = opcode + hexval;
                      }
                      outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
                      get_8byte_hex_value(address)<<" "<<
                      opcode<<get_spaces(32-opcode.length())<<text<<std::endl;
                    }else{
                      complt_inst = complt_inst + "_r8_";
                      p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                      if(p2 != opt.end()){
                        opcode = opcode + (*p2).opcode;
                        opcode = opcode + "[";
                        std::list<int>::iterator lst_p = node->addr_list->begin();
                        opcode = opcode + decimal_to_hex(*lst_p);
                        opcode = opcode + "00]";
                      }
                      outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
                      get_8byte_hex_value(address)<<" "<<
                      opcode<<get_spaces(32-opcode.length())<<text<<std::endl;
                    }
                  }
                  break;
                case 2 :
                  {
                    complt_inst = complt_inst + "/_moff16_";
                    if(!operand_2.compare("ax")){
                      complt_inst = complt_inst + "_ax_";
                      p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                      if(p2 != opt.end()){
                        opcode = opcode + (*p2).opcode;
                        opcode = opcode + "[";
                        std::list<int>::iterator lst_p = node->addr_list->begin();
                        opcode = opcode + decimal_to_hex(*lst_p);
                        opcode = opcode + "00]";
                      }
                      outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
                      get_8byte_hex_value(address)<<" "<<
                      opcode<<get_spaces(32-opcode.length())<<text<<std::endl;
                    }else if(isdigit(operand_2[0])){
                      complt_inst = complt_inst + "_imm16_";
                      p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                      if(p2 != opt.end()){
                        opcode = opcode + (*p2).opcode;
                        opcode = opcode + decimal_to_hex(get_reg_decm_opcode(operand_1) + (*p2).start_code_size);
                        opcode.insert(opcode.length(),"[");
                        opcode = opcode + decimal_to_hex(*(node->addr_list->begin()));
                        opcode.insert(opcode.length(),"00]");
                        std::string hexval = decimal_to_hex(atoi(get_char_ptr(operand_2)));
                        if(hexval.length() < 2){
                          hexval.insert(0,"0");
                          hexval.insert(hexval.length(), "000000", 6);
                        }else{
                          hexval.insert(hexval.length(), "000000", 6);
                        }
                        opcode = opcode + hexval;
                      }
                      outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
                      get_8byte_hex_value(address)<<" "<<
                      opcode<<get_spaces(32-opcode.length())<<text<<std::endl;
                    }else{
                      complt_inst = complt_inst + "_r16_";
                      p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                      if(p2 != opt.end()){
                        opcode = opcode + (*p2).opcode;
                        opcode = opcode + "[";
                        std::list<int>::iterator lst_p = node->addr_list->begin();
                        opcode = opcode + decimal_to_hex(*lst_p);
                        opcode = opcode + "00]";
                      }
                      outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
                      get_8byte_hex_value(address)<<" "<<
                      opcode<<get_spaces(32-opcode.length())<<text<<std::endl;
                    }
                  }
                  break;
                case 4 :
                  {
                    complt_inst = complt_inst + "/_moff32_";
                    if(!operand_2.compare("eax")){
                      complt_inst = complt_inst + "_eax_";
                      p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                      if(p2 != opt.end()){
                        opcode = opcode + (*p2).opcode;
                        opcode = opcode + "[";
                        std::list<int>::iterator lst_p = node->addr_list->begin();
                        opcode = opcode + decimal_to_hex(*lst_p);
                        opcode = opcode + "00]";
                      }
                      outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
                      get_8byte_hex_value(address)<<" "<<
                      opcode<<get_spaces(32-opcode.length())<<text<<std::endl;
                    }else if(isdigit(operand_2[0])){
                      complt_inst = complt_inst + "_imm32_";
                      p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                      if(p2 != opt.end()){
                        opcode = opcode + (*p2).opcode;
                        opcode = opcode + decimal_to_hex(get_reg_decm_opcode(operand_1) + (*p2).start_code_size);
                        opcode.insert(opcode.length(),"[");
                        opcode = opcode + decimal_to_hex(*(node->addr_list->begin()));
                        opcode.insert(opcode.length(),"00]");
                        std::string hexval = decimal_to_hex(atoi(get_char_ptr(operand_2)));
                        if(hexval.length() < 2){
                          hexval.insert(0,"0");
                          hexval.insert(hexval.length(), "000000", 6);
                        }else{
                          hexval.insert(hexval.length(), "000000", 6);
                        }
                        opcode = opcode + hexval;
                      }
                      outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
                      get_8byte_hex_value(address)<<" "<<
                      opcode<<get_spaces(32-opcode.length())<<text<<std::endl;
                    }else{
                      complt_inst = complt_inst + "_r32_";
                      p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                      if(p2 != opt.end()){
                        opcode = opcode + (*p2).opcode;
                        opcode = opcode + "[";
                        std::list<int>::iterator lst_p = node->addr_list->begin();
                        opcode = opcode + decimal_to_hex(*lst_p);
                        opcode = opcode + "00]";
                      }
                      outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
                      get_8byte_hex_value(address)<<" "<<
                      opcode<<get_spaces(32-opcode.length())<<text<<std::endl;
                    }
                  }
                  break;
              }
            }
          }
        }else{
          if(match_with_registers(operand_1, &reg_index)){
            std::string complt_inst = mnemonic;
            std::string opcode;
            switch(reg_size[reg_index]){
              case 1 :
                {
                  consume_comma(text, &index);
                  index--;
                  operand_2 = get_operand(text, &index, &is_cast_2, &cast_size_2);
                  if(!operand_2.compare("")){
                    er.print_errors(21, atoi(get_char_ptr(linenumber)), index+1, true, true);
                    er.error_count++;
                  }
                  if(is_cast_2){
                    cast_symbol_2 = get_symbol_cast(text, &index);
                    SYMTAB_NODE *node = sm.search_node(get_char_ptr(cast_symbol_2));
                    if(node != NULL){
                      if(!operand_1.compare("al")){
                        complt_inst = complt_inst + "/_al_";
                        complt_inst = complt_inst + "_moff8_";
                        p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                        if(p2 != opt.end()){
                          opcode = opcode + (*p2).opcode;
                          opcode.insert(opcode.length(),"[");
                          opcode = opcode + decimal_to_hex(*(node->addr_list->begin()));
                          opcode.insert(opcode.length(),"00]");
                        }
                      }else{
                        complt_inst = complt_inst + "/_r8_";
                        complt_inst = complt_inst + "_moff8_";
                        p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                        if(p2 != opt.end()){
                          opcode = opcode + (*p2).opcode;
                          opcode.insert(opcode.length(),"[");
                          opcode = opcode + decimal_to_hex(*(node->addr_list->begin()));
                          opcode.insert(opcode.length(),"00]");
                        }
                      }
                    }
                  }else{
                    if(match_with_registers(operand_2, &reg_index)){
                      switch(reg_size[reg_index]){
                        case 1 :
                          {
                            complt_inst = complt_inst + "/_r8_";
                            complt_inst = complt_inst + "_r8_";
                            p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                            if(p2 != opt.end()){
                              opcode = opcode + (*p2).opcode;
                            }
                          }
                          break;
                        case 2 :
                          {
                            complt_inst = complt_inst + "/_r16_";
                            complt_inst = complt_inst + "_r16_";
                            p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                            if(p2 != opt.end()){
                              opcode = opcode + (*p2).opcode;
                            }
                          }
                          break;
                        case 4 :
                          {
                            complt_inst = complt_inst + "/_r32_";
                            complt_inst = complt_inst + "_r32_";
                            p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                            if(p2 != opt.end()){
                              opcode = opcode + (*p2).opcode;
                            }
                          }
                          break;
                      }
                    }else{
                      if(isdigit(operand_2[0])){
                        if(!operand_1.compare("al")){
                          complt_inst = complt_inst + "/_al_";
                          complt_inst = complt_inst + "_imm8_";
                          p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                          if(p2 != opt.end()){
                            opcode = opcode + (*p2).opcode;
                            opcode = opcode + decimal_to_hex(get_reg_decm_opcode(operand_1) + (*p2).start_code_size);
                            std::string hexval = decimal_to_hex(atoi(get_char_ptr(operand_2)));
                            if(hexval.length() < 2){
                              hexval.insert(0,"0");
                            }
                            opcode = opcode + hexval;
                          }
                        }else{
                          complt_inst = complt_inst + "/_r8_";
                          complt_inst = complt_inst + "_imm8_";
                          p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                          if(p2 != opt.end()){
                            opcode = opcode + (*p2).opcode;
                            opcode = opcode + decimal_to_hex(get_reg_decm_opcode(operand_1) + (*p2).start_code_size);
                            std::string hexval = decimal_to_hex(atoi(get_char_ptr(operand_2)));
                            if(hexval.length() < 2){
                              hexval.insert(0,"0");
                            }
                            opcode = opcode + hexval;
                          }
                        }
                        
                      }
                    }
                  }
                  outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
                  get_8byte_hex_value(address)<<" "<<
                  opcode<<get_spaces(32-opcode.length())<<text<<std::endl;
                }
                break;
              case 2 :
                {
                  consume_comma(text, &index);
                  index--;
                  operand_2 = get_operand(text, &index, &is_cast_2, &cast_size_2);
                  if(is_cast_2){
                    cast_symbol_2 = get_symbol_cast(text, &index);
                    SYMTAB_NODE *node = sm.search_node(get_char_ptr(cast_symbol_2));
                    if(node != NULL){
                      if(!operand_1.compare("ax")){
                        complt_inst = complt_inst + "/_ax_";
                        complt_inst = complt_inst + "_moff16_";
                        p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                        if(p2 != opt.end()){
                          opcode = opcode + (*p2).opcode;
                          opcode.insert(opcode.length(),"[");
                          opcode = opcode + decimal_to_hex(*(node->addr_list->begin()));
                          opcode.insert(opcode.length(),"00]");
                        }
                      }else{
                        complt_inst = complt_inst + "/_r16_";
                        complt_inst = complt_inst + "_moff16_";
                        p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                        if(p2 != opt.end()){
                          opcode = opcode + (*p2).opcode;
                          opcode.insert(opcode.length(),"[");
                          opcode = opcode + decimal_to_hex(*(node->addr_list->begin()));
                          opcode.insert(opcode.length(),"00]");
                        }
                      }
                    }
                  }else{
                    if(match_with_registers(operand_2, &reg_index)){
                      switch(reg_size[reg_index]){
                        case 1 :
                          {
                            complt_inst = complt_inst + "/_r8_";
                            complt_inst = complt_inst + "_r8_";
                            p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                            if(p2 != opt.end()){
                              opcode = opcode + (*p2).opcode;
                            }
                          }
                          break;
                        case 2 :
                          {
                            complt_inst = complt_inst + "/_r16_";
                            complt_inst = complt_inst + "_r16_";
                            p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                            if(p2 != opt.end()){
                              opcode = opcode + (*p2).opcode;
                            }
                          }
                          break;
                        case 4 :
                          {
                            complt_inst = complt_inst + "/_r32_";
                            complt_inst = complt_inst + "_r32_";
                            p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                            if(p2 != opt.end()){
                              opcode = opcode + (*p2).opcode;
                            }
                          }
                          break;
                      }
                    }else{
                      if(isdigit(operand_2[0])){
                        if(!operand_1.compare("ax")){
                          complt_inst = complt_inst + "/_ax_";
                          complt_inst = complt_inst + "_imm16_";
                          p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                          if(p2 != opt.end()){
                            opcode = opcode + (*p2).opcode;
                            opcode = opcode + decimal_to_hex(get_reg_decm_opcode(operand_1) + (*p2).start_code_size);
                            std::string hexval = decimal_to_hex(atoi(get_char_ptr(operand_2)));
                            if(hexval.length() < 2){
                              hexval.insert(hexval.length(),"0");
                            }else{
                              hexval = hexval + "0";
                            }
                            opcode = opcode + hexval;
                          }
                        }else{
                          complt_inst = complt_inst + "/_r16_";
                          complt_inst = complt_inst + "_imm16_";
                          p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                          if(p2 != opt.end()){
                            opcode = opcode + (*p2).opcode;
                            opcode = opcode + decimal_to_hex(get_reg_decm_opcode(operand_1) + (*p2).start_code_size);
                            std::string hexval = decimal_to_hex(atoi(get_char_ptr(operand_2)));
                            if(hexval.length() < 2){
                              hexval.insert(0,"0");
                            }
                            opcode = opcode + hexval;
                          }
                        }
                      }
                    }
                    outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
                    get_8byte_hex_value(address)<<" "<<
                    opcode<<get_spaces(32-opcode.length())<<text<<std::endl;
                  }
                }
                break;
              case 4 :
                {
                  consume_comma(text, &index);
                  index--;
                  operand_2 = get_operand(text, &index, &is_cast_2, &cast_size_2);
                  if(is_cast_2){
                    cast_symbol_2 = get_symbol_cast(text, &index);
                    SYMTAB_NODE *node = sm.search_node(get_char_ptr(cast_symbol_2));
                    if(node != NULL){
                      if(!operand_1.compare("eax")){
                        complt_inst = complt_inst + "/_eax_";
                        complt_inst = complt_inst + "_moff32_";
                        p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                        if(p2 != opt.end()){
                          opcode = opcode + (*p2).opcode;
                          opcode.insert(opcode.length(),"[");
                          opcode = opcode + decimal_to_hex(*(node->addr_list->begin()));
                          opcode.insert(opcode.length(),"00]");
                        }
                      }else{
                        complt_inst = complt_inst + "/_r32_";
                        complt_inst = complt_inst + "_moff32_";
                        p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                        if(p2 != opt.end()){
                          opcode = opcode + (*p2).opcode;
                          opcode.insert(opcode.length(),"[");
                          opcode = opcode + decimal_to_hex(*(node->addr_list->begin()));
                          opcode.insert(opcode.length(),"00]");
                        }
                      }
                    }
                  }else{
                    if(match_with_registers(operand_2, &reg_index)){
                      switch(reg_size[reg_index]){
                        case 1 :
                          {
                            complt_inst = complt_inst + "/_r8_";
                            complt_inst = complt_inst + "_r8_";
                            p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                            if(p2 != opt.end()){
                              opcode = opcode + (*p2).opcode;
                            }
                          }
                          break;
                        case 2 :
                          {
                            complt_inst = complt_inst + "/_r16_";
                            complt_inst = complt_inst + "_r16_";
                            p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                            if(p2 != opt.end()){
                              opcode = opcode + (*p2).opcode;
                            }
                          }
                          break;
                        case 4 :
                          {
                            complt_inst = complt_inst + "/_r32_";
                            complt_inst = complt_inst + "_r32_";
                            p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                            if(p2 != opt.end()){
                              opcode = opcode + (*p2).opcode;
                            }
                          }
                          break;
                      }
                    }else{
                      if(isdigit(operand_2[0])){
                        if(!operand_1.compare("eax")){
                          complt_inst = complt_inst + "/_eax_";
                          complt_inst = complt_inst + "_imm32_";
                          p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                          if(p2 != opt.end()){
                            opcode = opcode + (*p2).opcode;
                            opcode = opcode + decimal_to_hex(get_reg_decm_opcode(operand_1) + (*p2).start_code_size);
                            std::string hexval = decimal_to_hex(atoi(get_char_ptr(operand_2)));
                            if(hexval.length() < 2){
                              hexval.insert(0,"0");
                              hexval.insert(hexval.length(), "000000", 6);
                            }else{
                              hexval.insert(hexval.length(), "000000", 6);
                            }
                            opcode = opcode + hexval;
                          }
                        }else{
                          complt_inst = complt_inst + "/_r32_";
                          complt_inst = complt_inst + "_imm32_";
                          p2 = opt.find(OPCodeTable(complt_inst, 0,"","", 0, 0, 0));
                          if(p2 != opt.end()){
                            opcode = opcode + (*p2).opcode;
                            opcode = opcode + decimal_to_hex(get_reg_decm_opcode(operand_1) + (*p2).start_code_size);
                            std::string hexval = decimal_to_hex(atoi(get_char_ptr(operand_2)));
                            if(hexval.length() < 2){
                              hexval.insert(0,"0");
                              hexval.insert(hexval.length(), "000000", 6);
                            }else{
                              hexval.insert(hexval.length(), "000000", 6);
                            }
                            opcode = opcode + hexval;
                          }
                        }
                      }
                    }
                  }
                  outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<
                  get_8byte_hex_value(address)<<" "<<
                  opcode<<get_spaces(32-opcode.length())<<text<<std::endl;
                }
                break;
            }
          }else{
            er.print_errors(16, atoi(get_char_ptr(linenumber)), index+1, false, false);
            std::cout<<operand_1<<std::endl;
            er.error_count++;
          }
        }
        is_cast = false;
      }
      break;
  }
}


/*
this function reads only instruction from intermediate file
*/
void read_declarations_pass_2(std::string strbuf, std::ofstream &outfile)
{
  int index = 0;
  std::string linenumber, address, symbol, text;

  linenumber = get_data(strbuf, &index);  //read line number form file
  address = get_data(strbuf, &index);     //read address from file
  symbol = get_data(strbuf, &index);      //read symbol from file
  text = get_data(strbuf, &index);        //read original text from file

  if(!address.compare("")){
    if(text.find(".bss",0,4) != std::string::npos && !address.compare("")){
      section_type = BSS_SECTION;
    }else if(text.find(".data",0,4) != std::string::npos && !address.compare("")){
      section_type = DATA_SECTION;
    }if(text.find(".text",0,4) != std::string::npos && !address.compare("")){
      section_type = TEXT_SECTION;
      outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<lst_spaces[2]<<text<<std::endl;
    }else{
      outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<lst_spaces[2]<<text<<std::endl;
    }
  }else{
    switch(section_type){
      //if bss section found then, write it to lst file
      case BSS_SECTION :
        {
          if(!address.empty() && !text.empty() && !symbol.empty()){
            char *labstr = get_char_ptr(symbol);
            SYMTAB_NODE *node = sm.search_node(labstr);
            if(node != NULL){
              if(node->addr_list != NULL){
                node->addr_list->clear();
                (node->addr_list)->push_back(hex_to_decimal(address));
              }
              if(node->is_array){
                outfile<<get_spaces(5-linenumber.length()+1)<<linenumber<<" "<<get_8byte_hex_value(address)<<" <res "<<
                get_8byte_hex_value(decimal_to_hex(node->array_size))<<">"<<lst_spaces[1]<<text<<std::endl;
              }
            }
          }
        }
        break;
      //if data section found then, write it to lst file by calling a function for it 
      case DATA_SECTION :
        {
          if(!address.empty() && !text.empty() && !symbol.empty()){
            char *labstr = get_char_ptr(symbol);
            SYMTAB_NODE *node = sm.search_node(labstr);
            if(node != NULL){
              if(node->addr_list != NULL){
                node->addr_list->clear();
                (node->addr_list)->push_back(hex_to_decimal(address));
              }
              write_datas_to_file(outfile, linenumber, node, address, text);
            }
            node = NULL;
          }
        }
        break;
      //if text section found then, assemble instructions, generate opcodes & write it to lst file
      case TEXT_SECTION :
        {
          if(!address.empty() && !text.empty()){
            if(!text.find("global") || !text.find("extern")){
            }else{
              assemble_instructions(text, linenumber, address, outfile);
            }
          }
        }
        break;
      }
    }
}


//reads each line from intermediate file and generate the lst file
void gen_lst_file(const std::string interm_file, const std::string lst_file)
{
  std::string strbuf;

  std::ifstream infile(interm_file, std::ios::in);
  std::ofstream outfile(lst_file, std::ios::out);
  
  clear_buffer(Buffer);

  while(infile.getline(Buffer, 255)){
    strbuf.assign(Buffer, strlen(Buffer));

    if(!strbuf.empty()){
      read_declarations_pass_2(strbuf, outfile);
    }
  }

  if(er.error_count > 0)
    exit(0);

  infile.close();
  outfile.close();
}

