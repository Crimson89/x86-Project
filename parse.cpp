#include "header.h"

int parseInstruction(uint16_t instructionCode, instruction* newInstruction)
{
    //instruction newInstruction;
    int err; // will be error code. 

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
  
  //TODO add masks to some of these (like EMT and TRAP) maybe pull out if needed
  uint16_t uniqueInstructions[7] = {0000004, 0000003, 0000000, 0000001, 0000002, 0000005, 0000006};

  for (int i = 0; i < 10; i++)
  {
    if (instructionCode == uniqueInstructions[i])
    {
      // unique, full instruction is a unique opcode. Because why not I guess.
      current_instruction->opcode = instructionCode;
      break;
    }
      
  }

  // TODO fill these out
  // EMT
  if ((instructionCode & 0177400) == 0104000)
  {

  }

  // TRAP?
  //if ((instructionCode & ) == 0104440)
  //{

  //}

  // Check SOB (0 0 7) (TODO maybe double reg?)
  /*if ((instructionCode && 0xFE00) == 0x0E00)
  {

  } */

  /*// Check MARK (0 0 6 4) (maybe single)
  if ((instructionCode && 0xFFC0) == 0x0C00)
  {

  }*/
  
  // Check RTS (0 0 0 2 0)
  /*if ((instructionCode && 0xFFF8) == 0x0080)
  {
    //TODO what is R?
    newInstruction->opcode = instructionCode & 0xFF80;
    newInstruction->rtsR = instructionCode & 0x0007;
  }
  
  // Check JSR (0 0 4) (maybe taken care of by single operand)
  if ((instructionCode && 0xFE00) == 0x0800)
  {

  }*/
  
  /*// Check JMP (Branches?)
  if ((instructionCode && 0x0E00) == 0x0E00)
  {

  } */
  
  uint16_t relevantBits = instructionCode & maskRelevantBits;
  uint16_t bitPattern = relevantBits & maskSingleCondBranchCondCheck;
  uint16_t tempLocation;

  // TODO may need to shift certain amounts for source, dest, reg bases.
  // Didn't think about how to actually interface with registers and memory space, 
  // so working that out now.
  if (bitPattern == 0000000) // Define constans for these maybe.
  {
    bitPattern = relevantBits & maskSingle;
    if (bitPattern == 0004000)
    {
      // Single operand
      current_instruction->opcode = instructionCode & maskSingleOpcode;
      current_instruction->addressingModeReg = (instructionCode & maskSingleMode) >> 3;
      tempLocation = instructionCode & maskSingleRegister;
      current_instruction->regBase = REGS[tempLocation];
      err = addressDecode(current_instruction->addressingModeReg, current_instruction->regBase, current_instruction->reg);
      current_instruction->byteMode = (instructionCode & maskByteMode) >> 15;
      cout << "SINGLE " << "\n";
    }
    else
    {
      bitPattern = relevantBits & maskCondCheck;
      if ((bitPattern == 0000240) || (bitPattern == 0000260))
      {
        //TODO fix these
        // cond check
        /*
        newInstruction->opcode = instructionCode & maskCondCodeOpcode;
        newInstruction->SC = instructionCode & maskCondSC;
        newInstruction->N = instructionCode & maskCondN;
        newInstruction->Z = instructionCode & maskCondZ;
        newInstruction->V = instructionCode & maskCondV;
        newInstruction->C = instructionCode & maskCondC;*/
        cout << "COND " << "\n";
      }
      else
      {
        //cond branch
        current_instruction->opcode = instructionCode & maskCondBranchOpcode;
        current_instruction->offset = instructionCode & maskCondBranchOffset;
        cout << "COND BRANCH " << "\n";
      }
    } 
  }
  else
  {
    bitPattern = relevantBits & maskRegSource;
    if (bitPattern == 0070000)
    {
      // register source double operand
      current_instruction->opcode = instructionCode & maskDoubleRegisterOpcode;
      current_instruction->regBase = (instructionCode & maskDoubleRegisterReg) >> 6;
      // TODO Defined by wikipedia, this is srouce/dest, how should I handle that?
      current_instruction->srcBase = instructionCode & maskDoubleRegisterSourceDest;
      current_instruction->destBase = instructionCode & maskDoubleRegisterSourceDest;
      current_instruction->addressingModeSrc = (instructionCode & maskDoubleRegisterSourceDestMode) >> 3;
      current_instruction->addressingModeDest = (instructionCode & maskDoubleRegisterSourceDestMode) >> 3;
      cout << "DOUBLE REG " << "\n";

      err = addressDecode(current_instruction->addressingModeReg, current_instruction->regBase, current_instruction->reg);

      err = addressDecode(current_instruction->addressingModeSrc, current_instruction->srcBase, current_instruction->src);

      err = addressDecode(current_instruction->addressingModeDest, current_instruction->destBase, current_instruction->dest);
    }
    else
    {
      // double operand
      current_instruction->opcode = instructionCode & maskDoubleOpcode;
      current_instruction->srcBase = (instructionCode & maskDoubleSource) >> 6;
      current_instruction->addressingModeSrc = (instructionCode & maskDoubleSourceMode) >> 9;
      current_instruction->destBase = instructionCode & maskDoubleDest;
      current_instruction->addressingModeDest = (instructionCode & maskDoubleDestMode) >> 3;

      err = addressDecode(current_instruction->addressingModeSrc, current_instruction->srcBase, current_instruction->src);

      err = addressDecode(current_instruction->addressingModeDest, current_instruction->destBase, current_instruction->dest);
      cout << "DOUBLE" << "\n";
    }
  }

  return err;
}

// TODO how to handle PC and SP modes.
int addressDecode(uint16_t mode, uint16_t baseAddress, uint16_t resultAddress)
{
  // TODO get rid of pointer stuff.
  int err;
  uint16_t X;
  uint16_t workingAddress;
  bool byteMode = current_instruction->byteInstruction;
 
  // TODO byte vs word accesses, flag and some ifs?
  switch (mode)
  {
    // Register
    case 0000000: resultAddress = baseAddress; // Keep baseAddress the same.
                  break;
    // Register deferred
    case 0000001: //resultAddress = *baseAddress;
                  resultAddress = read_byte(baseAddress); // Maybe this? Address of a spot in MEM?
                  break;
    // Autoincrement
    case 0000002: //workingAddress = baseAddress;
                  //resultAddress = *baseAddress;
                  resultAddress = read_byte(baseAddress);
                  if (byteMode == true)
                    {
                      baseAddress++;
                    }
                  else
                    {
                      baseAddress += 2;
                    }
                  break;
    // Autoincrement deferred
    case 0000003: workingAddress = read_byte(baseAddress);
                  //workingAddress = *baseAddress;
                  //resultAddress = *workingAddress;
                  resultAddress = read_byte(workingAddress);
                  baseAddress += 2;
                  break;
    // Autodecrement
    case 0000004: if (byteMode == true)
                  {
                    baseAddress--;
                  }
                  else
                  {
                    baseAddress -= 2;
                  }
                  //workingAddress = baseAddress;
                  //resultAddress = *baseAddress;
                  resultAddress = read_byte(baseAddress);
                  break;
    // Autodecrement deferred
    case 0000005: baseAddress -= 2;
                  workingAddress = read_byte(baseAddress);
                  //workingAddress = *baseAddress;
                  resultAddress = read_byte(workingAddress);
                  //resultAddress = *workingAddress;
                  break;
    // Index
    // TODO WTF how does the X translate in the instruction code?
    case 0000006: //workingAddress = baseAddress;
                  //TODO dereference PC first?
                  X = PC + 2;
                  //resultAddress = *baseAddress + X;
                  resultAddress = read_byte(baseAddress + X);
                  break;
    // Index deferred
    case 0000007: workingAddress = read_byte(baseAddress);
                  //TODO dereference PC first?
                  X = PC + 2;
                  resultAddress = read_byte(workingAddress + X);
                  //resultAddress = *workingAddress + X;
                  break;
  }
  
  return err;
}

// instruction assignType(uint16_t opCode, instruction parsedInstruction)
