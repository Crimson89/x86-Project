#include <header.h>

instruction parseType(u16_t opCode)
{
  instruction newInstruction;

  
  u16_t relvenatBits = opCode & maskRelevantBits;
  u16_t bitPattern = relevantBits & maskSingleCondBranchCondCheck;

  if (bitPattern == 0x0000) // Define constans for these maybe.
  {
    bitPattern = relevantBits & maskSingle;
    if (bitPattern == 0x0800)
    {
      // single operand
      // assignType
    }
    else
    {
      bitPattern = relevantBits & maskCondCheck;
      if (bitPattern == 0x00A0)
      {
        // cond check
        // assignType
      }
      else
      {
        //cond branch
        // assignType
      }
    } 
  }
  else
  {
    bitPattern = relevantBits & maskRegSource;
    if (bitPattern == 0x7000)
    {
      // register source double operand
      // assignType
    }
    else
    {
      // double operand
      // assignType
    }
  }

  return newInstruction;
}

// instruction assignType(u16_t opCode, instruction parsedInstruction)
