#ifndef _OPTAB_H_
#define _OPTAB_H_

#include<iostream>
#include<algorithm>
#include<unordered_set>


//opcode table
class OPCodeTable{
  public:
    std::string mnemonic;
    int params;
    std::string reg_mem_size;
    std::string opcode;
    int size;
    int cmp_size;
    int start_code_size;
    
    OPCodeTable(std::string mnem, int parm, std::string regmsz, std::string opcd, int sz, int cmpsize, int scs){
      mnemonic = mnem;
      params = parm;
      reg_mem_size = regmsz;
      opcode = opcd;
      size = sz;
      cmp_size = cmpsize;
      start_code_size = scs;
    }

  public:
    std::string get_mnemonic() const{
      return mnemonic;
    }

	  bool operator==(const OPCodeTable & n) const {
		  return (mnemonic == n.mnemonic);
	  }
};


//override the hash function from unordered_set template
namespace std
{
  template<> struct hash<OPCodeTable>
    {
      size_t operator()(const OPCodeTable & obj) const
      {
        return hash<string>()(obj.get_mnemonic());
      }
    };
}


//defining out custom hash function
struct Hash_OpTab
{
  size_t operator()(const OPCodeTable & obj) const
  {
    return std::hash<std::string>()(obj.get_mnemonic());
  }
};


extern void init_mnemonics_opcodes();

#endif
