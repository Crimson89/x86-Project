#include "header.h"

int printInstruction(instruction* newInstruction)
{
  printf("\nopcode %u\n", newInstruction->opcode);
  printf("byteMode %u\n", newInstruction->byteMode);
  printf("addressingModeSrc %u\n", newInstruction->addressingModeSrc);
  printf("addressingModeDest %u\n", newInstruction->addressingModeDest);
  printf("addressingModeReg %u\n", newInstruction->addressingModeReg);
  printf("srcBase %u\n", newInstruction->srcBase);
  printf("destBase %u\n", newInstruction->destBase);
  printf("regBase %u\n", newInstruction->regBase);
  printf("offset %u\n", newInstruction->offset);
  printf("rtsReg %u\n\n", newInstruction->rtsReg);
  //printf("opcode % u", newInstruction->opcode);
  //printf("opcode % u", newInstruction->opcode);
  return 0;
}

int clearInstruction(instruction* newInstruction)
{
  newInstruction->opcode = 0;
  newInstruction->byteMode = 0;
  newInstruction->addressingModeSrc = 0;
  newInstruction->addressingModeDest = 0;
  newInstruction->addressingModeReg = 0;
  newInstruction->srcBase = 0;
  newInstruction->destBase = 0;
  newInstruction->regBase = 0;
  newInstruction->offset = 0;
  newInstruction->PSW = 0;
  newInstruction->rtsReg = 0;

  return 0;
}

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

  // TODO need to account for EMT, JMP, 
  // EMT should be the only one to implement needed with a full opcode.
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
  if ((instructionCode && 0xFFF8) == 0x0080)
  {
    newInstruction->opcode = instructionCode & 0xFF80;
    newInstruction->rtsReg = instructionCode & 0x0007;
  }
  
  // Check JSR (0 0 4) (maybe taken care of by single operand)
  /*if ((instructionCode && 0xFE00) == 0x0800)
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
      current_instruction->regBase = tempLocation;
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


// TODO, gotta add PC and SP (probably just check if R6 or R7, since baseAddress is only specifying 0-7 right)?
// This returns the address, doing N-1 trace statements. We could probably have the trace statements in
// the read_byte and read_word functions honestly.
uint16_t get_address(uint16_t mode, uint16_t baseAddress)
{
  uint16_t X;
  uint16_t workingAddress;
  bool byte = current_instruction->byteMode;
  uint16_t resultAddress = 0;

  // TODO define constants for all the values used for logic in here.
  if (baseAddress == 7)
  {
    // TODO PC take into account differences between address and value
    switch (mode)
    {
    // Register
    // What to do here?
    case 0000000: //INVALID
                  break;
    // Register deferred
    case 0000001: //INVALID
                  break;
    // Autoincrement
    case 0000002: resultAddress = PC;
                  PC += 2;
                  break;
    // Autoincrement deferred
    case 0000003: workingAddress = PC; 
                  // READ TRACE
                  if (byte)
                  {
                    resultAddress = read_byte(REGISTER_MODE, workingAddress, false);
                  }
                  else
                  {
                    resultAddress = read_word(REGISTER_MODE, workingAddress, false);
                  }
                  PC += 2;
                  break;
    // Autodecrement
    case 0000004: 
    // Autodecrement deferred
    case 0000005: 
    // Index
    // TODO WTF how does the X translate in the instruction code?
    case 0000006: //TODO how to interface with PC?
                  X = PC;
                  PC += 2;
                  resultAddress = PC + X;
                  break;
    // Index deferred
    case 0000007: X = PC;
                  PC += 2;
                  workingAddress = PC + X;
                  // READ TRACE
                  if (byte)
                  {
                    resultAddress = read_byte(mode, workingAddress, true);
                  }
                  else
                  {
                    resultAddress = read_word(mode, workingAddress, true);
                  }
                  break;
    }
  }
  else if (baseAddress == 6)
  {
    // TODO SP

    switch (mode)
    {
    // Register
    case 0000000: //resultAddress = baseAddress;
                  break;
    // Register deferred
    case 0000001: resultAddress = SP;
                  break;
    // Autoincrement
    case 0000002: resultAddress = SP;
                  SP += 2;
                  break;
    // Autoincrement deferred
    case 0000003: workingAddress = SP; 
                  // READ TRACE
                  if (byte)
                  {
                    resultAddress = read_byte(mode, workingAddress, true);
                  }
                  else
                  {
                    resultAddress = read_word(mode, workingAddress, true);
                  }
                  SP += 2;
                  break;
    // Autodecrement
    case 0000004: SP -= 2;
                  resultAddress = SP;
                  break;
    // Autodecrement deferred
    case 0000005: /*REGS[baseAddress] -= 2;
                  workingAddress = REGS[baseAddress];
                  // READ TRACE
                  if (byte)
                  {
                    resultAddress = read_byte(mode, workingAddress, true);
                  }
                  else
                  {
                    resultAddress = read_word(mode, workingAddress, true);
                  }*/
                  break;
    // Index
    case 0000006: // READ TRACE
                  X = read_word(mode, PC, true);
                  PC += 2;
                  resultAddress = SP + X;
                  break;
    // Index deferred
    case 0000007: workingAddress = SP;
                  //TODO only read word?
                  // READ TRACE
                  X = read_word(mode, PC, true);
                  PC += 2;
                  // READ TRACE
                  if (byte)
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
  else
  {
    switch (mode)
    {
    // Register
    case 0000000: resultAddress = baseAddress;
                  break;
    // Register deferred
    case 0000001: resultAddress = REGS[baseAddress];
                  break;
    // Autoincrement
    case 0000002: resultAddress = REGS[baseAddress];
                  if (byte == true)
                    {
                      //REGS[baseAddress]++;
                    }
                  else
                    {
                      //REGS[baseAddress] += 2;
                    }
                  break;
    // Autoincrement deferred
    case 0000003: workingAddress = REGS[baseAddress]; 
                  // READ TRACE
                  if (byte)
                  {
                    resultAddress = read_byte(mode, workingAddress, true);
                  }
                  else
                  {
                    resultAddress = read_word(mode, workingAddress, true);
                  }
                  //REGS[baseAddress] += 2;
                  break;
    // Autodecrement
    case 0000004: if (byte == true)
                  {
                    resultAddress = REGS[baseAddress] - 1;
                  }
                  else
                  {
                    resultAddress = REGS[baseAddress] - 2;
                    //REGS[baseAddress] -= 2;
                  }
                  //resultAddress = REGS[baseAddress];
                  break;
    // Autodecrement deferred
    case 0000005: //REGS[baseAddress] -= 2;
                  workingAddress = REGS[baseAddress] - 2;
                  // READ TRACE
                  if (byte)
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
                  X = read_word(mode, PC, true);
                  PC += 2;
                  resultAddress = REGS[baseAddress] + X;
                  break;
    // Index deferred
    case 0000007: workingAddress = REGS[baseAddress];
                  //TODO only read word?
                  // READ TRACE
                  X = read_word(mode, PC, true);
                  PC += 2;
                  // READ TRACE
                  if (byte)
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
  bool byte = current_instruction->byteMode;
  uint16_t resultValue = 0;
  
  if (baseAddress == 7)
  {
    // PC ADDRESSING
    switch (mode)
    {
      // Register
      case 0000000: //resultValue = REGS[baseAddress];
                    break;
      // Register deferred
      case 0000001: /*workingAddress = REGS[baseAddress];
                    if (byte)
                    {
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress, true);
                    }
                    else
                    {
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, true);
                    }*/
                    break;
      // Autoincrement
      case 0000002: workingAddress = PC;
                    if (byte)
                    {
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress, true);
                    }
                    else
                    {
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, true);
                    }
                    PC += 2;
                    break;
      // Autoincrement deferred
      case 0000003: // READ TRACE
                    workingAddress = PC;
                    if (byte)
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
                    PC += 2;
                    break;
      // Autodecrement
      case 0000004: 
                    break;
      // Autodecrement deferred
      case 0000005: 
                    break;
      // Index
      case 0000006: // READ TRACE
                    X = read_word(mode, PC, true);
                    PC += 2;
                    if (byte)
                    {
                      // READ TRACE
                      resultValue = read_byte(mode, PC + X, true);
                    }
                    else
                    { 
                      // READ TRACE
                      resultValue = read_word(mode, PC + X, true);
                    }
                    break;
      // Index deferred
      case 0000007: // READ TRACE
                    X = read_word(mode, PC, true);
                    PC += 2;
                    if (byte)
                    {
                      // READ TRACE
                      workingAddress = read_byte(mode, PC + X, true);
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress, true);
                    }
                    else
                    {
                      // READ TRACE
                      workingAddress = read_word(mode, PC + X, true);
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, true);
                    }
                    break;
    }
  }
  else if (baseAddress == 6)
  {
    switch (mode)
    {
      // SP ADDRESSING
      // Register
      case 0000000: // TODO how to gracefully handle these?
                    //resultValue = REGS[baseAddress];
                    break;
      // Register deferred
      case 0000001: workingAddress = SP;
                    if (byte)
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
      case 0000002: workingAddress = SP;
                    if (byte)
                    {
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress, true);
                    }
                    else
                    {
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, true);
                    }
                    SP += 2;
                    break;
      // Autoincrement deferred
      case 0000003: // READ TRACE
                    workingAddress = SP;
                    if (byte)
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
                    SP += 2;
                    break;
      // Autodecrement
      case 0000004: SP -= 2;
                    workingAddress = SP;
                    if (byte)
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
      // Autodecrement deferred
      case 0000005: /*baseAddress -= 2;
                    workingAddress = REGS[baseAddress];
                    if (byte)
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
                    }*/
                    break;
      // Index
      case 0000006: // READ TRACE
                    workingAddress = SP;
                    X = read_word(mode, PC, true);
                    PC += 2;
                    if (byte)
                    {
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress + X, true);
                    }
                    else
                    { 
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress + X, true);
                    }
                    break;
      // Index deferred
      case 0000007: // READ TRACE
                    workingAddress = SP;
                    X = read_word(mode, PC, true);
                    PC += 2;
                    if (byte)
                    {
                      // READ TRACE
                      workingAddress = read_byte(mode, workingAddress + X, true);
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress, true);
                    }
                    else
                    {
                      // READ TRACE
                      workingAddress = read_word(mode, workingAddress + X, true);
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, true);
                    }
                    break;
    }
  }
  else
  {
    switch (mode)
    {
      // Register
      case 0000000: resultValue = REGS[baseAddress];
                    break;
      // Register deferred
      case 0000001: workingAddress = REGS[baseAddress];
                    if (byte)
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
                    if (byte)
                    {
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress, true);
                    }
                    else
                    {
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, true);
                    }
                    if (byte == true)
                    {
                      REGS[baseAddress]++;
                    }
                    else
                    {
                      REGS[baseAddress] += 2;
                    }
                    break;
      // Autoincrement deferred
      case 0000003: // READ TRACE
                    workingAddress = REGS[baseAddress];
                    if (byte)
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
                    REGS[baseAddress] += 2;
                    break;
      // Autodecrement
      case 0000004: if (byte == true)
                    {
                      REGS[baseAddress]--;
                    }
                    else
                    {
                      REGS[baseAddress] -= 2;
                    }
                    workingAddress = REGS[baseAddress];
                    if (byte)
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
      // Autodecrement deferred
      case 0000005: REGS[baseAddress] -= 2;
                    workingAddress = REGS[baseAddress];
                    if (byte)
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
                    workingAddress = REGS[baseAddress];
                    X = read_word(mode, PC, true);
                    PC += 2;
                    if (byte)
                    {
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress + X, true);
                    }
                    else
                    { 
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress + X, true);
                    }
                    break;
      // Index deferred
      case 0000007: // READ TRACE
                    workingAddress = REGS[baseAddress];
                    X = read_word(mode, PC, true);
                    PC += 2;
                    if (byte)
                    {
                      // READ TRACE
                      workingAddress = read_byte(mode, workingAddress + X, true);
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress, true);
                    }
                    else
                    {
                      // READ TRACE
                      workingAddress = read_word(mode, workingAddress + X, true);
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, true);
                    }
                    break;
    }
  }
  return resultValue;
}

