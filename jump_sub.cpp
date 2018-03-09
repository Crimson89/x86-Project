#include "header.h"

int JMP(instruction *inst) // Jump
{
  uint16_t destination;

  // If dest addressing mode = 0, "illegal instruction"
  if(inst->addressingModeReg == 0)
    return -1;

  destination = get_value(inst->addressingModeRed, inst->reg);
   
  // If value is an odd address, "boundar error trap condition"
  if(destination & 0x0001)
    return -1;

  PC = destination;

  return 0;
}

int JSR(instruction *inst) // Jump to subroutine
{
}
