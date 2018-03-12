#include "header.h"

// Jump & subroutine instructions

// Jump
int JMP(instruction *inst)
{
  inst->op_text = "JMP";
  uint16_t destination;

  // If dest addressing mode = 0, "illegal instruction"
  if(inst->addressingModeReg == 0) {
    cout << oct << "Illegal Instruction @ " << PC-2 
      << ".  Program control cannot be transferred to a register.\n";
    return -1;
  }

  destination = get_value(inst->addressingModeReg, inst->regBase);
   
  // If value is an odd address, "boundary error trap condition"
  if(destination & 0x0001) {
    cout << oct << "Boundary error @ " << PC-2
      << ". Misaligned jump destination: " << destination << ".\n";
    return -1;
  }

  PC = destination;

  return 0;
}

// Jump to subroutine
int JSR(instruction *inst)
{
  inst->op_text = "JSR";
  // (tmp) <- (dest) (dest might be SP, get this value before pushing reg)
  // -(SP) <- reg
  // reg <- PC
  // PC <- (tmp)
  
  uint16_t dst_tmp, reg_tmp;
  
  // tmp <- dest
  dst_tmp = get_value(inst->addressingModeDest, inst->destBase);
  
  // -(SP) <- reg
  reg_tmp = get_value(00, inst->regBase);
  SP -= 2;
  write_word(01, SP, reg_tmp, true);

  // reg <- PC
  write_word(00, inst->regBase, PC, false);

  // PC <- tmp
  write_word(00, PC, dst_tmp, false);

  return 0;
}

// Return from subroutine
int RTS(instruction *inst)
{
  inst->op_text = "RTS";
  uint16_t tmp;

  // PC <- reg
  tmp = get_value(00, inst->regBase);
  write_word(00, PC, tmp, false);

  // reg <- (SP)+
  tmp = get_value(01, SP);
  write_word(00, inst->regBase, tmp, false);
  SP += 2;

  return 0;
}


