#include <header.h>

instruction parseInstruction(uint16_t instructionCode, instruction* newInstruction)
{
  //instruction newInstruction;
    int placeholder; // will be error code. 

  /* First thing is first, check against instructions with entirely unique opcodes.
   * Operate Group
   *   HALT, WAIT, RTI, RESET, RTT, NOP
     Program Control Group
       JMP, JSR, RTS, IOT, EMT, TRAP, BPT, MARK, SOB
     That's a lot :(
     Full OPCODE
       IOT, EMT, TRAP, BPT, HALT, WAIT, RTI, RESET, RTT, NOP
     Other
       MARK, SOB, RTS, JSR, JMP
   */

  uint16_t uniqueInstructions[10] = {0x0004, 0x8800, 0x8920, /*BPT,*/ 0x0000, 0x0001, 0x0002, 0x0005, /*RTT,*/ /*NOP*/}

  for (int i = 0; i < 10, i++)
  {
    if (instructionCode == uniqueInstructions[i])
    {
      // unique, full instruction is a unique opcode. Because why not I guess.
      newInstruction->opcode = instructionCode;
      break;
    }
      
  }

  // Check SOB (0 0 7) (TODO maybe double reg?)
  /*if ((instructionCode && 0xFE00) == 0x0E00)
  {

  } */

  /*// Check MARK (0 0 6 4) (maybe single)
  if ((instructionCode && 0xFFC0) == 0x0C00)
  {

  }*/
  
  // Check RTS (0 0 0 2 0)
  if ((instructionCode && 0xFFF8) == 0x0080)
  {
    //TODO what is R?
    newInstruction->opCode = instructionCode & 0xFF80;
    newInstruction->rtsR = instructionCode & 0x0007;
  }
  
  /*// Check JSR (0 0 4) (maybe taken care of by single operand)
  if ((instructionCode && 0xFE00) == 0x0800)
  {

  }*/
  
  /*// Check JMP (Branches?)
  if ((instructionCode && 0x0E00) == 0x0E00)
  {

  } */
  
  uint16_t relevantBits = instructionCode & maskRelevantBits;
  uint16_t bitPattern = relevantBits & maskSingleCondBranchCondCheck;

  if (bitPattern == 0x0000) // Define constans for these maybe.
  {
    bitPattern = relevantBits & maskSingle;
    if (bitPattern == 0x0800)
    {
      // Single operand
      newInstruction->opcode = instructionCode & maskSingleOpcode;
      newInstruction->registerMode = instructionCode & maskSingleRegisterMode;
      newInstruction->reg = instructionCode & maskSingleRegister;
      newInstruction->byteMode = instructionCode & maskByteMode;
    }
    else
    {
      bitPattern = relevantBits & maskCondCheck;
      if (bitPattern == 0x00A0)
      {
        // cond check
        newInstruction->opcode = instructionCode & maskCondCodeOpcode;
        newInstruction->SC = instructionCode & maskCondSC;
        newInstruction->N = instructionCode & maskCondN;
        newInstruction->Z = instructionCode & maskCondZ;
        newInstruction->V = instructionCode & maskCondV;
        newInstruction->C = instructionCode & maskCondC;
      }
      else
      {
        //cond branch
        newInstruction->opcode = instructionCode & maskCondBranchOpcode;
        newInstruction->offset = instructionCode & maskCondBranchOffset;
      }
    } 
  }
  else
  {
    bitPattern = relevantBits & maskRegSource;
    if (bitPattern == 0x7000)
    {
      // register source double operand
      newInstruction->opcode = instructionCode & maskDoubleRegisterOpcode;
      newInstruction->reg = instructionCode & maskDoubleRegisterReg;
      // TODO Defined by wikipedia, this is srouce/dest, how should I handle that?
      newInstruction->src = instructionCode & maskDoubleRegisterSourceDest;
      newInstruction->dest = instructionCode & maskDoubleRegisterSourceDest;
      newInstruction->addressingModeSrc = instructionCode & maskDoubleRegisterSourceDestMode;
      newInstruction->addressingModeDest = instructionCode & maskDoubleRegisterSrcDestMode;
    }
    else
    {
      // double operand
      newInstruction->opcode = instructionCode & maskDoubleOpcode;
      newInstruction->src = instructionCode & maskDoubleSource;
      newInstruction->addressingModeSrc = instructionCode & maskDoubleSourceMode;
      newInstruction->dest = instructionCode & maskDoubleDest;
      newInstruction->addressingModeDest = instructionCode & maskDoubleDestMode;
    }
  }

  return placeholder;
}

// instruction assignType(uint16_t opCode, instruction parsedInstruction)
