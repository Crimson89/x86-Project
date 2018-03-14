#include "header.h"

int CCC(instruction *inst)
{
  // format of instruction:
  // 0 000 000 010 10N ZVC

  uint16_t op = inst->offset;
  string code_string = "";

  // clear flags based on last 4 bits of opcode
  if(op & 0000010) {
    inst->N = 0;
    code_string += "CLN|";
  }
  if(op & 0000004) {
    inst->Z = 0;
    code_string += "CLZ|";
  }
  if(op & 0000002) {
    inst->V = 0;
    code_string += "CLV|";
  }
  if(op & 0000001) {
    inst->C = 0;
    code_string += "CLC|";
  }
  
  // special case for all bits set
  if((op & 0000017) == 0000017) {
    code_string = "CCC";
  }

  // special case for no bits set (NOP)
  if(!(op & 0000017)) {
    code_string = "NOP";
  }
  
  if(code_string.back() == '|')
    code_string.pop_back();

  inst->op_text = code_string;

  return 0;
}

int SCC(instruction *inst)
{
  // format of instruction:
  // 0 000 000 010 11N ZVC

  uint16_t op = inst->offset;
  string code_string = "";

  // set flags based on last 4 bits of opcode
  if(op & 0000010) {
    inst->N = 1;
    code_string += "SEN|";
  }
  if(op & 0000004) {
    inst->Z = 1;
    code_string += "SEZ|";
  }
  if(op & 0000002) {
    inst->V = 1;
    code_string += "SEV|";
  }
  if(op & 0000001) {
    inst->C = 1;
    code_string += "SEC|";
  }
  
  // special case for all bits set
  if((op & 0000017) == 0000017) {
    code_string = "SCC";
  }

  // special case for no bits set (NOP)
  if(!(op & 0000017)) {
    code_string = "NOP";
  }
  
  if(code_string.back() == '|')
	  code_string.pop_back();
  inst->op_text = code_string;

  return 0;
}
