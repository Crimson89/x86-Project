#include "header.h"

// Jump & subroutine instructions

// Jump
int JMP(instruction *inst)
{
  inst->op_text = "JMP";
  inst->is_branch = true;
  inst->branch_taken = true;
  uint16_t destination;

  // If dest addressing mode = 0, "illegal instruction"
  if(inst->addressingModeReg == 0) {
    cout << oct << "Illegal Instruction @ " << PC-2 
      << ".  Program control cannot be transferred to a register.\n";
    return -1;
  }

  destination = get_address(inst->addressingModeReg, inst->regBase);
   
  // If value is an odd address, "boundary error trap condition"
  if(destination & 0x0001) {
    cout << oct << "Boundary error @ " << PC-2
      << ". Misaligned jump destination: " << destination << ".\n";
    return -1;
  }

  PC = destination;
  inst->branch_target = PC;

  return 0;
}

// Jump to subroutine
int JSR(instruction *inst)
{
  inst->op_text = "JSR";
  inst->is_branch = true;
  inst->branch_taken = true;
  // (tmp) <- (dest) (dest might be SP, get this value before pushing reg)
  // -(SP) <- reg
  // reg <- PC
  // PC <- (tmp)
  
  uint16_t dst_tmp, reg_tmp;
  // tmp <- dest
  dst_tmp = get_address(inst->addressingModeDest, inst->destBase);
  // -(SP) <- reg
  if (inst->regBase == PC_REG_INDEX)
  {
    reg_tmp = PC + 2;
  }
  else
  {
    reg_tmp = get_value(REGISTER_MODE, inst->regBase);
  }
  SP -= 2;
  write_word(REGISTER_DEFR_MODE, SP, reg_tmp, true);

  // reg <- PC
  write_word(REGISTER_MODE, inst->regBase, PC, false);

  // PC <- tmp
  write_word(REGISTER_MODE, PC_REG_INDEX, dst_tmp, false);

  inst->branch_target = PC;
  return 0;
}

// Return from subroutine
int RTS(instruction *inst)
{
  inst->op_text = "RTS";
  inst->is_branch = true;
  inst->branch_taken = true;
  uint16_t tmp;

  // PC <- reg
  if (inst->rtsReg == PC_REG_INDEX)
  {
    tmp = PC;
  }
  else
  {
    tmp = get_value(REGISTER_MODE, inst->rtsReg) + 2;
  }
  write_word(REGISTER_MODE, PC_REG_INDEX, tmp, false);

  // reg <- (SP)+
  tmp = get_value(REGISTER_DEFR_MODE, SP_REG_INDEX);
  write_word(REGISTER_MODE, inst->rtsReg, tmp, false);
  SP += 2;
  inst->branch_target = PC;

  return 0;
}


