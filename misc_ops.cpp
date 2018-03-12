#include "header.h"

int SPL(instruction *inst) // Set priority level
{
  inst->op_text = "SPL";
  cerr << "Not implemented in PDP-11/60" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}


int NOP(instruction *inst){ // No Operation
  inst->op_text = "NOP";
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int EMT(instruction *inst) // Emulator trap
{
  inst->op_text = "EMT";
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}
