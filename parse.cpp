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
//      err = addressDecode(current_instruction->addressingModeReg, current_instruction->regBase, current_instruction->reg);
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

  //    err = addressDecode(current_instruction->addressingModeReg, current_instruction->regBase, current_instruction->reg);

    //  err = addressDecode(current_instruction->addressingModeSrc, current_instruction->srcBase, current_instruction->src);

      //err = addressDecode(current_instruction->addressingModeDest, current_instruction->destBase, current_instruction->dest);
    }
    else
    {
      // double operand
      current_instruction->opcode = instructionCode & maskDoubleOpcode;
      current_instruction->srcBase = (instructionCode & maskDoubleSource) >> 6;
      current_instruction->addressingModeSrc = (instructionCode & maskDoubleSourceMode) >> 9;
      current_instruction->destBase = instructionCode & maskDoubleDest;
      current_instruction->addressingModeDest = (instructionCode & maskDoubleDestMode) >> 3;

    //  err = addressDecode(current_instruction->addressingModeSrc, current_instruction->srcBase, current_instruction->src);

      //err = addressDecode(current_instruction->addressingModeDest, current_instruction->destBase, current_instruction->dest);
      cout << "DOUBLE" << "\n";
    }
  }

  return err;
}

// TODO how to handle PC and SP modes.
// TODO this function will be deprecated most likely.
/*int addressDecode(uint16_t mode, uint16_t baseAddress, uint16_t resultAddress)
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
                  // No traces. Set a flag here.
                  break;
    // Register deferred
    case 0000001: //resultAddress = *baseAddress;
                  if (byteMode)
                  {
                    resultAddress = read_byte(mode, baseAddress); // Maybe this? Address of a spot in MEM?
                  }
                  else
                  {
                    resultAddress = read_word(mode, baseAddress);
                  }
                  break;
    // Autoincrement
    case 0000002: //workingAddress = baseAddress;
                  //resultAddress = *baseAddress;
                  resultAddress = read_byte(mode, baseAddress);
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
}*/

// TODO, gotta add PC and SP (probably just check if R6 or R7, since baseAddress is only specifying 0-7 right)?
// This returns the address, doing N-1 trace statements. We could probably have the trace statements in
// the read_byte and read_word functions honestly.
uint16_t get_address(uint16_t mode, uint16_t baseAddress)
{
  uint16_t X;
  uint16_t workingAddress;
  bool byteMode = current_instruction->byteInstruction;
  uint16_t resultAddress = 0;

  // TODO define constants for all the values used for logic in here.
  if (baseAddress == 7)
  {
    // TODO PC
    /*switch (mode)
    {
    // Register
    // What to do here?
    // Probably have to check in the instruction if it's mode is register any time get address is
    // wanted.
    case 0000000: //INVALID
                  break;
    // Register deferred
    case 0000001: resultAddress = REGS[baseAddress];
                  break;
    // Autoincrement
    // TODO something is wrong here for sure with the increment, this used to be a pointer.
    case 0000002: resultAddress = REGS[baseAddress];
                  if (byteMode == true)
                    {
                      REGS[baseAddress]++;
                    }
                  else
                    {
                      REGS[baseAddress] += 2;
                    }
                  break;
    // Autoincrement deferred
    case 0000003: workingAddress = REGS[baseAddress]; 
                  // READ TRACE
                  if (byteMode)
                  {
                    resultAddress = read_byte(mode, workingAddress, true);
                  }
                  else
                  {
                    resultAddress = read_word(mode, workingAddress, true);
                  }
                  REGS[baseAddress] += 2;
                  break;
    // Autodecrement
    case 0000004: if (byteMode == true)
                  {
                    REGS[baseAddress]--;
                  }
                  else
                  {
                    REGS[baseAddress] -= 2;
                  }
                  resultAddress = REGS[baseAddress];
                  break;
    // Autodecrement deferred
    case 0000005: REGS[baseAddress] -= 2;
                  workingAddress = REGS[baseAddress];
                  // READ TRACE
                  if (byteMode)
                  {
                    resultAddress = read_byte(mode, workingAddress, true);
                  }
                  else
                  {
                    resultAddress = read_word(mode, workingAddress, true);
                  }
                  break;
    // Index
    // TODO WTF how does the X translate in the instruction code?
    case 0000006: //TODO how to interface with PC?
                  X = PC + 2;
                  resultAddress = REGS[baseAddress] + X;
                  break;
    // Index deferred
    case 0000007: workingAddress = REGS[baseAddress];
                  //TODO how to interface with PC?
                  X = PC + 2;
                  // READ TRACE
                  if (byteMode)
                  {
                    resultAddress = read_byte(mode, workingAddress + X, true);
                  }
                  else
                  {
                    resultAddress = read_word(mode, workingAddress + X, true);
                  }
                  break;
    }*/
  }
  else if (baseAddress == 6)
  {
    // TODO SP

    /*switch (mode)
    {
    // Register
    // What to do here?
    // Probably have to check in the instruction if it's mode is register any time get address is
    // wanted.
    case 0000000: //INVALID
                  break;
    // Register deferred
    case 0000001: resultAddress = REGS[baseAddress];
                  break;
    // Autoincrement
    // TODO something is wrong here for sure with the increment, this used to be a pointer.
    case 0000002: resultAddress = REGS[baseAddress];
                  if (byteMode == true)
                    {
                      REGS[baseAddress]++;
                    }
                  else
                    {
                      REGS[baseAddress] += 2;
                    }
                  break;
    // Autoincrement deferred
    case 0000003: workingAddress = REGS[baseAddress]; 
                  // READ TRACE
                  if (byteMode)
                  {
                    resultAddress = read_byte(workingAddress);
                  }
                  else
                  {
                    resultAddress = read_word(workingAddress);
                  }
                  REGS[baseAddress] += 2;
                  break;
    // Autodecrement
    case 0000004: if (byteMode == true)
                  {
                    REGS[baseAddress]--;
                  }
                  else
                  {
                    REGS[baseAddress] -= 2;
                  }
                  resultAddress = REGS[baseAddress];
                  break;
    // Autodecrement deferred
    case 0000005: REGS[baseAddress] -= 2;
                  workingAddress = REGS[baseAddress];
                  // READ TRACE
                  if (byteMode)
                  {
                    resultAddress = read_byte(workingAddress);
                  }
                  else
                  {
                    resultAddress = read_word(workingAddress);
                  }
                  break;
    // Index
    case 0000006: 
                  X = PC + 2;
                  resultAddress = REGS[baseAddress] + X;
                  break;
    // Index deferred
    case 0000007: workingAddress = REGS[baseAddress];
                  X = PC + 2;
                  // READ TRACE
                  if (byteMode)
                  {
                    resultAddress = read_byte(workingAddress + X);
                  }
                  else
                  {
                    resultAddress = read_word(workingAddress + X);
                  }
                  break;
    }*/
  }
  else
  {
    switch (mode)
    {
    // Register
    // TODO What to do here?
    // Probably have to check in the instruction if it's mode is register any time get address is
    // wanted. Maybe not needed with new read/write_byte/word instructions.
    case 0000000: resultAddress = baseAddress;
                  break;
    // Register deferred
    case 0000001: resultAddress = REGS[baseAddress];
                  break;
    // Autoincrement
    case 0000002: resultAddress = REGS[baseAddress];
                  if (byteMode == true)
                    {
                      REGS[baseAddress]++;
                    }
                  else
                    {
                      REGS[baseAddress] += 2;
                    }
                  break;
    // Autoincrement deferred
    case 0000003: workingAddress = REGS[baseAddress]; 
                  // READ TRACE
                  if (byteMode)
                  {
                    resultAddress = read_byte(mode, workingAddress, true);
                  }
                  else
                  {
                    resultAddress = read_word(mode, workingAddress, true);
                  }
                  REGS[baseAddress] += 2;
                  break;
    // Autodecrement
    case 0000004: if (byteMode == true)
                  {
                    REGS[baseAddress]--;
                  }
                  else
                  {
                    REGS[baseAddress] -= 2;
                  }
                  resultAddress = REGS[baseAddress];
                  break;
    // Autodecrement deferred
    case 0000005: REGS[baseAddress] -= 2;
                  workingAddress = REGS[baseAddress];
                  // READ TRACE
                  if (byteMode)
                  {
                    resultAddress = read_byte(mode, workingAddress, true);
                  }
                  else
                  {
                    resultAddress = read_word(mode, workingAddress, true);
                  }
                  break;
    // Index
    case 0000006: // READ TRACE
                  X = read_word(mode, PC + 2, true);
                  PC += 2;
                  resultAddress = REGS[baseAddress] + X;
                  break;
    // Index deferred
    case 0000007: workingAddress = REGS[baseAddress];
                  //TODO only read word?
                  // READ TRACE
                  X = read_word(mode, PC + 2, true);
                  PC += 2;
                  // READ TRACE
                  if (byteMode)
                  {
                    resultAddress = read_byte(mode, workingAddress + X, true);
                  }
                  else
                  {
                    resultAddress = read_word(mode, workingAddress + X, true);
                  }
                  break;
    }
  }

  return resultAddress;
}

uint16_t get_value(uint16_t mode, uint16_t baseAddress)
{
  uint16_t X;
  uint16_t workingAddress;
  bool byteMode = current_instruction->byteInstruction;
  uint16_t resultValue = 0;
  
  // TODO byte vs word accesses, flag and some ifs?
  switch (mode)
  {
    // Register
    case 0000000: resultValue = REGS[baseAddress];
                  break;
    // Register deferred
    case 0000001: workingAddress = REGS[baseAddress];
                  if (byteMode)
                  {
                    // READ TRACE
                    resultValue = read_byte(mode, workingAddress, true);
                  }
                  else
                  {
                    // READ TRACE
                    resultValue = read_word(mode, workingAddress, true);
                  }
                  break;
    // Autoincrement
    case 0000002: workingAddress = REGS[baseAddress];
                  if (byteMode)
                  {
                    // READ TRACE
                    resultValue = read_byte(mode, workingAddress, true);
                  }
                  else
                  {
                    // READ TRACE
                    resultValue = read_word(mode, workingAddress, true);
                  }
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
    case 0000003: // READ TRACE
                  workingAddress = REGS[baseAddress];
                  if (byteMode)
                  {
                    workingAddress = read_byte(mode, workingAddress, true);
                    // READ TRACE
                    resultValue = read_byte(mode, workingAddress, true);
                  }
                  else
                  { 
                    workingAddress = read_word(mode, workingAddress, true);
                    // READ TRACE
                    resultValue = read_word(mode, workingAddress, true);
                  }
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
                  workingAddress = REGS[baseAddress];
                  if (byteMode)
                  {
                    // READ TRACE
                    resultValue = read_byte(mode, baseAddress, true);
                  }
                  else
                  {
                    // READ TRACE
                    resultValue = read_word(mode, baseAddress, true);
                  }
                  break;
    // Autodecrement deferred
    case 0000005: baseAddress -= 2;
                  workingAddress = REGS[baseAddress];
                  if (byteMode)
                  {
                    // READ TRACE
                    workingAddress = read_byte(mode, workingAddress, true);
                    // READ TRACE
                    resultValue = read_byte(mode, workingAddress, true);
                  }
                  else
                  { 
                    // READ TRACE
                    workingAddress = read_word(mode, workingAddress, true);
                    // READ TRACE
                    resultValue = read_word(mode, workingAddress, true);
                  }
                  break;
    // Index
    case 0000006: // READ TRACE
                  X = read_word(mode, PC + 2, true);
                  PC += 2;
                  if (byteMode)
                  {
                    // READ TRACE
                    resultValue = read_byte(mode, baseAddress + X, true);
                  }
                  else
                  { 
                    // READ TRACE
                    resultValue = read_word(mode, baseAddress + X, true);
                  }
                  break;
    // Index deferred
    case 0000007: // READ TRACE
                  X = read_word(mode, PC + 2, true);
                  PC += 2;
                  if (byteMode)
                  {
                    // READ TRACE
                    workingAddress = read_byte(mode, baseAddress, true);
                    // READ TRACE
                    resultValue = read_byte(mode, workingAddress + X, true);
                  }
                  else
                  {
                    // READ TRACE
                    workingAddress = read_word(mode, baseAddress, true);
                    // READ TRACE
                    resultValue = read_word(mode, workingAddress + X, true);
                  }
                  break;
  }

  return resultValue;
}

//uint16_t writeRegister(uint16_t addressMode, uint16_t register)
//{
  //pseudocode
  /*
    Check the addressmode to confirm it is register (although we may want to wrap this up
     in the read memory and write memory functions
  */
//}
