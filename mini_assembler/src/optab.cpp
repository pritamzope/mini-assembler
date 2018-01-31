#include<iostream>
#include<algorithm>
#include<unordered_set>
#include"optab.hpp"

std::unordered_set<OPCodeTable, Hash_OpTab> opt;

void init_mnemonics_opcodes()
{
  opt.insert(OPCodeTable("inc", 1, "", "", 0, 5, 0));
  opt.insert(OPCodeTable("inc/r8", 1, "r8", "FEC", 2, 5, 0));
  opt.insert(OPCodeTable("inc/r16", 1, "r16", "4", 2, 5, 0));
  opt.insert(OPCodeTable("inc/r32", 1, "r32", "664", 2, 5, 0));
  opt.insert(OPCodeTable("inc/m8", 1, "m8", "FE06", 2, 5, 0));
  opt.insert(OPCodeTable("inc/m16", 1, "m16", "FF06", 2, 5, 0));
  opt.insert(OPCodeTable("inc/m32", 1, "m32", "66FF06", 4, 5, 0));

  opt.insert(OPCodeTable("dec", 1, "", "", 0, 5, 8));
  opt.insert(OPCodeTable("dec/r8", 1, "r8", "FEC", 1, 5, 8));
  opt.insert(OPCodeTable("dec/r16", 1, "r16", "4", 2, 5, 8));
  opt.insert(OPCodeTable("dec/r32", 1, "r32", "664", 2, 5, 8));
  opt.insert(OPCodeTable("dec/m8", 1, "m8", "FE0E", 2, 5, 8));
  opt.insert(OPCodeTable("dec/m16", 1, "m16", "FF0E", 2, 5, 8));
  opt.insert(OPCodeTable("dec/m32", 1, "m32", "66FF0E", 4, 5, 8));
  

  opt.insert(OPCodeTable("mov", 2, "", "", 0, 5, 0));
  opt.insert(OPCodeTable("mov/r8", 2, "r8", "", 2, 5, 0));
  opt.insert(OPCodeTable("mov/r16", 2, "r16", "", 2, 5, 0));
  opt.insert(OPCodeTable("mov/r32", 2, "r32", "", 4, 5, 0));
  opt.insert(OPCodeTable("mov/m8", 2, "m8", "", 2, 5, 0));
  opt.insert(OPCodeTable("mov/m16", 2, "m16", "", 2, 5, 0));
  opt.insert(OPCodeTable("mov/m32", 2, "m32", "", 4, 5, 0));
  opt.insert(OPCodeTable("mov/_al__moff8_", 2, "r8", "A0", 4, 5, 0));
  opt.insert(OPCodeTable("mov/_ax__moff16_", 2, "r16", "A1", 4, 5, 0));
  opt.insert(OPCodeTable("mov/_eax__moff32_", 2, "r32", "66A1", 4, 5, 0));
  opt.insert(OPCodeTable("mov/_r8__moff8_", 2, "r8", "8A1E", 4, 5, 0));
  opt.insert(OPCodeTable("mov/_r16__moff16_", 2, "r16", "8B1E", 4, 5, 0));
  opt.insert(OPCodeTable("mov/_r32__moff32_", 2, "r32", "668B1E", 4, 5, 0));
  opt.insert(OPCodeTable("mov/_moff8__al_", 2, "r8", "A2", 4, 5, 0));
  opt.insert(OPCodeTable("mov/_moff16__ax_", 2, "r16", "A3", 4, 5, 0));
  opt.insert(OPCodeTable("mov/_moff32__eax_", 2, "r32", "66A3", 4, 5, 0));
  opt.insert(OPCodeTable("mov/_r8__imm8_", 2, "r8", "B", 4, 5, 0));
  opt.insert(OPCodeTable("mov/_r16__imm16_", 2, "r8", "B8", 4, 5, 0));
  opt.insert(OPCodeTable("mov/_r32__imm32_", 2, "r8", "66B", 4, 5, 8));
  opt.insert(OPCodeTable("mov/_r8__r8_", 2, "r8", "89", 4, 5, 8));
  opt.insert(OPCodeTable("mov/_r16__r16_", 2, "r8", "89", 4, 5, 8));
  opt.insert(OPCodeTable("mov/_r32__r32_", 2, "r8", "6689", 4, 5, 8));
  opt.insert(OPCodeTable("mov/_m8__r8_", 2, "r8", "89", 4, 5, 8));
  opt.insert(OPCodeTable("mov/_m16__r16_", 2, "r16", "89", 4, 5, 8));
  opt.insert(OPCodeTable("mov/_m32__r32_", 2, "r32", "66891E", 4, 5, 8));
  opt.insert(OPCodeTable("mov/_moff8__r8_", 2, "r8", "880E", 4, 5, 0));
  opt.insert(OPCodeTable("mov/_moff16__r16_", 2, "r16", "A3", 4, 5, 0));
  opt.insert(OPCodeTable("mov/_moff32__r32_", 2, "r32", "66891E", 4, 5, 0));
  opt.insert(OPCodeTable("mov/_moff8__imm8_", 2, "r8", "C6", 4, 5, 0));
  opt.insert(OPCodeTable("mov/_moff16__imm16_", 2, "r16", "C706", 4, 5, 0));
  opt.insert(OPCodeTable("mov/_moff32__imm32_", 2, "r32", "66C706", 4, 5, 0));
  opt.insert(OPCodeTable("mov/_al__imm8_", 2, "r8", "B", 4, 5, 0));
  opt.insert(OPCodeTable("mov/_ax__imm16_", 2, "r16", "B8", 4, 5, 0));
  opt.insert(OPCodeTable("mov/_eax__imm32_", 2, "r32", "66B", 4, 5, 0));


  opt.insert(OPCodeTable("add", 2, "", "", 0, 2, 0));
  opt.insert(OPCodeTable("add/r8", 2, "r8", "", 2, 2, 0));
  opt.insert(OPCodeTable("add/r16", 2, "r16", "", 2, 2, 0));
  opt.insert(OPCodeTable("add/r32", 2, "r32", "", 4, 2, 0));
  opt.insert(OPCodeTable("add/m8", 2, "m8", "", 2, 2, 0));
  opt.insert(OPCodeTable("add/m16", 2, "m16", "", 2, 2, 0));
  opt.insert(OPCodeTable("add/m32", 2, "m32", "", 4, 2, 0));
  opt.insert(OPCodeTable("add/_al__imm8_", 2, "r8", "04", 4, 2, 0));
  opt.insert(OPCodeTable("add/_ax__imm16_", 2, "r16", "05", 4, 2, 0));
  opt.insert(OPCodeTable("add/_eax__imm32_", 2, "r32", "6683C", 4, 2, 0));
  opt.insert(OPCodeTable("add/_r8__imm8_", 2, "r32", "80", 4, 2, 0));
  opt.insert(OPCodeTable("add/_r16__imm16_", 2, "r32", "81", 4, 2, 0));
  opt.insert(OPCodeTable("add/_r32__imm32_", 2, "r32", "6681", 4, 2, 0));
  opt.insert(OPCodeTable("add/_r8__r8_", 2, "r32", "01D8", 4, 2, 8));
  opt.insert(OPCodeTable("add/_r16__r16_", 2, "r32", "01CB", 4, 2, 8));
  opt.insert(OPCodeTable("add/_r32__r32_", 2, "r32", "6601D8", 4, 2, 8));


  opt.insert(OPCodeTable("je", 1, "r8", "", 0, 5, 0));
  opt.insert(OPCodeTable("je/_r8_", 1, "r8", "74", 0, 5, 0));

  opt.insert(OPCodeTable("jmp", 1, "r8", "", 0, 5, 0));
  opt.insert(OPCodeTable("jmp/_r8_", 1, "r8", "EB", 0, 5, 0));
  
  opt.insert(OPCodeTable("ja", 1, "r8", "", 0, 5, 0));
  opt.insert(OPCodeTable("ja/_r8_", 1, "r8", "77", 0, 5, 0));

  opt.insert(OPCodeTable("jb", 1, "r8", "", 0, 5, 0));
  opt.insert(OPCodeTable("jb/_r8_", 1, "r8", "72", 0, 5, 0));
  
  opt.insert(OPCodeTable("jae", 1, "r8", "", 0, 5, 0));
  opt.insert(OPCodeTable("jae/_r8_", 1, "r8", "73", 0, 5, 0));

  opt.insert(OPCodeTable("jbe", 1, "r8", "", 0, 5, 0));
  opt.insert(OPCodeTable("jbe/_r8_", 1, "r8", "76", 0, 5, 0));

  opt.insert(OPCodeTable("jne", 1, "r8", "", 0, 5, 0));
  opt.insert(OPCodeTable("jne/_r8_", 1, "r8", "75", 0, 5, 0));

  opt.insert(OPCodeTable("jle", 1, "r8", "", 0, 5, 0));
  opt.insert(OPCodeTable("jle/_r8_", 1, "r8", "7E", 0, 5, 0));


  opt.insert(OPCodeTable("pusha", 0, "", "", 0, 0, 0));
  opt.insert(OPCodeTable("pushad", 0, "", "", 0, 0, 0));
  opt.insert(OPCodeTable("pusha/r32", 0, "r32", "60", 1, 1, 0));
  opt.insert(OPCodeTable("pushad/r32", 0, "r32", "60", 2, 2, 0));

  opt.insert(OPCodeTable("popa", 0, "", "", 0, 1, 0));
  opt.insert(OPCodeTable("popad", 0, "", "", 2, 2, 0));
  opt.insert(OPCodeTable("popa/r32", 0, "r32", "61", 1, 1, 0));
  opt.insert(OPCodeTable("popad/r32", 0, "r32", "61", 2, 2, 0));

  opt.insert(OPCodeTable("call", 1, "r32", "", 4, 5, 0));
  opt.insert(OPCodeTable("call/_r8_", 1, "r8", "E8(0000)", 4, 5, 0));

  opt.insert(OPCodeTable("push", 1, "", "", 0, 0, 0));
  opt.insert(OPCodeTable("push/_r8_", 1, "r8", "68", 0, 0, 0));

  opt.insert(OPCodeTable("pop", 1, "", "", 0, 0, 0));
  opt.insert(OPCodeTable("pop/_r8_", 1, "", "8F", 0, 0, 0));

  opt.insert(OPCodeTable("loop", 1, "r32", "", 5, 5, 0));
  opt.insert(OPCodeTable("loop/_r8_", 1, "r32", "E2", 5, 5, 0));

  opt.insert(OPCodeTable("cmp", 2, "", "", 0, 0, 0));
  opt.insert(OPCodeTable("cmp/_al__imm8_", 2, "r8", "3C", 2, 5, 0));
  opt.insert(OPCodeTable("cmp/_ax__imm16_", 2, "r16", "83F8", 2, 5, 0));
  opt.insert(OPCodeTable("cmp/_eax__imm32_", 2, "r32", "6683F8", 2, 5, 0));
  opt.insert(OPCodeTable("cmp/_eax__r32_", 2, "r32", "66833E", 2, 5, 0));
  opt.insert(OPCodeTable("cmp/_moff8__imm8_", 2, "r8", "803E", 2, 5, 0));
  opt.insert(OPCodeTable("cmp/_moff16__imm16_", 2, "r16", "833E", 2, 5, 0));
  opt.insert(OPCodeTable("cmp/_moff32__imm32_", 2, "r32", "66833E", 2, 5, 0));

  opt.insert(OPCodeTable("global", 1, "", "", 0, 0, 0));
  opt.insert(OPCodeTable("extern", 1, "", "", 0, 0, 0));

}
