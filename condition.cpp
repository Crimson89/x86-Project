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
  
  // for testing
  cout << "CCC result: " << code_string << endl;

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
  cout << "SCC " << code_string << endl;
  if(op & 0000004) {
    inst->Z = 1;
    code_string += "SEZ|";
  }
  cout << "SCC " << code_string << endl;
  if(op & 0000002) {
    inst->V = 1;
    code_string += "SEV|";
  }
  cout << "SCC " << code_string << endl;
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
  
  // for testing
  cout << "SCC result: " << code_string << endl;

  return 0;
}


int CLC(instruction *inst) // Clear carry
{
  inst->op_text = "CLC";
  inst->C = 0;
  return 0;
}


int CLV(instruction *inst) // Clear overflow
{
  inst->op_text = "CLV";
  inst->V = 0;
  return 0;
}

int CLZ(instruction *inst) // Clear zero
{
  inst->op_text = "CLZ";
  inst->Z = 0;
  return 0;
}

int CLN(instruction *inst) // Clear negative
{
  inst->op_text = "CLN";
  inst->N = 0;
  return 0;
}
/*
int CCC(instruction *inst) // Clear Condition Codes
{
  inst->op_text = "CCC";
  inst->C = 0;
  inst->V = 0;
  inst->Z = 0;
  inst->N = 0;
  return 0;
}
*/
int SEC(instruction *inst) // Set carry
{
  inst->op_text = "SEC";
  inst->C = 1;
  return 0;
}

int SEV(instruction *inst) // Set overflow
{
  inst->op_text = "SEV";
  inst->V = 1;
  return 0;
}

int SEZ(instruction *inst) // Set zero
{
  inst->op_text = "SEZ";
  inst->Z = 1;
  return 0;
}

int SEN(instruction *inst) // Set negative
{
  inst->op_text = "SEN";
  inst->N = 1;
  return 0;
}
/*
int SCC(instruction *inst) // Set Condition Codes
{
  inst->op_text = "SCC";
  inst->C = 1;
  inst->V = 1;
  inst->Z = 1;
  inst->N = 1;
  return 0;
}
*/
