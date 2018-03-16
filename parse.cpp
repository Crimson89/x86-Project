#include "header.h"

int printInstruction(instruction* newInstruction)
{
  cout << "Operation Name:" << newInstruction->op_text << endl;
  cout << "0pcode :" << setfill('0') << setw(7) << oct << newInstruction->opcode << endl;
  //printf("\nopcode %u\n", newInstruction->opcode);
  printf("byteMode %u\n", newInstruction->byteMode);
  printf("addressingModeSrc %u\n", newInstruction->addressingModeSrc);
  printf("addressingModeDest %u\n", newInstruction->addressingModeDest);
  printf("addressingModeReg %u\n", newInstruction->addressingModeReg);
  printf("srcBase %u\n", newInstruction->srcBase);
  printf("destBase %u\n", newInstruction->destBase);
  printf("regBase %u\n", newInstruction->regBase);
  printf("offset %u\n", newInstruction->offset);
  printf("immediate %u\n", newInstruction->immediate);
  printf("rtsReg %u\n\n", newInstruction->rtsReg);
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
  newInstruction->immediate = 0;
  newInstruction->PSW = 0;
  newInstruction->rtsReg = 0;
  newInstruction->op_text = "";

  return 0;
}

int parseInstruction(uint16_t instructionCode, instruction* newInstruction)
{
    //instruction newInstruction;
    int err = 0; // will be error code. 0 = no error, non-zero = error

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
  
  bool special = false;
  if(verbosity_level > HIGH_VERBOSITY) cout << "In parser, found ";
  //TODO add masks to some of these (like EMT and TRAP) maybe pull out if needed
  //                                    IOT      BPT     HALT     WAIT      RTI    RESET      RTT
  uint16_t uniqueInstructions[7] = {0000004, 0000003, 0000000, 0000001, 0000002, 0000005, 0000006};

  for (int i = 0; i < 10; i++)
  {
    if (instructionCode == uniqueInstructions[i])
    {
      // unique, full instruction is a unique opcode. Because why not I guess.
      current_instruction->opcode = instructionCode;
      if(verbosity_level > HIGH_VERBOSITY) cout << "Unique instruction list" << "\n";
	  special = true;
      break;
    }
      
  }

  // TODO fill these out
  // EMT
  if (!special& ( (instructionCode & 0177400) == 0104000))
  {
    current_instruction->opcode = (instructionCode & 0177400);
	current_instruction->offset = (instructionCode & 0000077);
    if(verbosity_level > HIGH_VERBOSITY) cout << "EMT" << "\n";
    special = true;
  } 

  // TRAP
  if (!special& ( (instructionCode & 104400) == 0104440))
  {
	  current_instruction->opcode = (instructionCode & 0104400);
	  current_instruction->offset = (instructionCode & 0000077);
    if(verbosity_level > HIGH_VERBOSITY) cout << "TRAP" << "\n";
      special = true;
  }


  // Check MARK
  if (!special& ((instructionCode & 0176400) == 0006400))
  {
	current_instruction->opcode = (instructionCode & 0006400);
	current_instruction->offset = (instructionCode & 0000077);
    if(verbosity_level > HIGH_VERBOSITY) cout << "MARK" << "\n";
    special = true;
  }
  
  // Check RTS (0 0 0 2 0)
  if (!special& ( (instructionCode & 0177770) == 0000200))
  {
    current_instruction->opcode = (instructionCode & 0177770);
    current_instruction->rtsReg = (instructionCode & 0000007);
    if(verbosity_level > HIGH_VERBOSITY) cout << "RTS" << "\n";
    special = true;
  }// Check JSR (0 0 4)
  if (!special& ( (instructionCode & 0177000) == 0004000))
  {
    current_instruction->opcode = (instructionCode & 0177000);
    current_instruction->regBase = (instructionCode & 0000700) >> 6;
    current_instruction->addressingModeReg = (instructionCode & 0007000) >> 9;
    current_instruction->addressingModeDest = (instructionCode & 000070) >> 3;
    current_instruction->destBase = (instructionCode & 0000007);
    if(verbosity_level > HIGH_VERBOSITY) cout << "JSR" << "\n";
    special = true;
  }// Check JMP ()
  if (!special& ( (instructionCode & 0177700) == 0000100))
  {
    current_instruction->opcode = instructionCode & 0177700;
    current_instruction->addressingModeReg = (instructionCode & 000070) >> 3;
    current_instruction->regBase = (instructionCode & 0000007);
    if(verbosity_level > HIGH_VERBOSITY) cout << "JMP" << "\n";
    special = true;
  }
  // Check SWAB
  if (!special& ( (instructionCode & 0177700) == 0000300))
  {
    current_instruction->opcode = instructionCode & maskSingleOpcode;
    current_instruction->addressingModeReg = (instructionCode & maskSingleMode) >> 3;
    current_instruction->regBase = instructionCode & maskSingleRegister;
    current_instruction->byteMode = (instructionCode & maskByteMode) >> 15;
    if(verbosity_level > HIGH_VERBOSITY) cout << "SWAB " << "\n";
    special = true;
  }
  if (!special) 
  {
    uint16_t relevantBits = instructionCode & maskRelevantBits;
    uint16_t bitPattern = relevantBits & maskSingleCondBranchCondCheck;
    uint16_t tempLocation;

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
        current_instruction->byteMode = (instructionCode & maskByteMode) >> 15;
        if(verbosity_level > HIGH_VERBOSITY) cout << "SINGLE " << "\n";
      }
      else
      {
        bitPattern = relevantBits & maskCondCheck;
        if ((bitPattern == 0000240) || (bitPattern == 0000260))
        {
          // cond check
        
          current_instruction->opcode = instructionCode & maskCondCodeOpcode;
          current_instruction->offset = instructionCode & 0x000F;
          if(verbosity_level > HIGH_VERBOSITY) cout << "COND CHECK" << "\n";
        }
        else
        {
          //cond branch
          current_instruction->opcode = instructionCode & maskCondBranchOpcode;
          current_instruction->offset = instructionCode & maskCondBranchOffset;
          if(verbosity_level > HIGH_VERBOSITY) cout << "COND BRANCH " << "\n";
        }
      } 
    }
    else
    {
      // TODO don't actually need this, I don't think we handle any "double reg" instructions.
      bitPattern = relevantBits & maskRegSource;
      if (bitPattern == 0070000)
      {
        // register source double operand
        current_instruction->opcode = instructionCode & maskDoubleRegisterOpcode;
        current_instruction->regBase = (instructionCode & maskDoubleRegisterReg) >> 6;
        current_instruction->srcBase = instructionCode & maskDoubleRegisterSourceDest;
        current_instruction->destBase = instructionCode & maskDoubleRegisterSourceDest;
        current_instruction->addressingModeSrc = (instructionCode & maskDoubleRegisterSourceDestMode) >> 3;
        current_instruction->addressingModeDest = (instructionCode & maskDoubleRegisterSourceDestMode) >> 3;
        if(verbosity_level > HIGH_VERBOSITY) cout << "DOUBLE REG " << "\n";
      }
      else
      {
        // double operand
        current_instruction->opcode = instructionCode & maskDoubleOpcode;
        current_instruction->srcBase = (instructionCode & maskDoubleSource) >> 6;
        current_instruction->addressingModeSrc = (instructionCode & maskDoubleSourceMode) >> 9;
        current_instruction->destBase = instructionCode & maskDoubleDest;
        current_instruction->addressingModeDest = (instructionCode & maskDoubleDestMode) >> 3;
        current_instruction->byteMode = (instructionCode & maskByteMode) >> 15;
        if(verbosity_level > HIGH_VERBOSITY) cout << "DOUBLE" << "\n";
        if(verbosity_level > HIGH_VERBOSITY) cout << current_instruction->addressingModeSrc << "\n";
        if(verbosity_level > HIGH_VERBOSITY) cout << current_instruction->addressingModeDest << "\n";
      }
    }
  }
 
  //TODO right place to put this?
  current_instruction->PSW = PSW.PSW_BYTE;
  
  uint16_t bad_mode = 0; 
  if ((current_instruction->regBase == 7) || (current_instruction->regBase == 6)) {
	if(current_instruction->regBase == 7) { // PC Register Modes
		if (current_instruction->addressingModeReg == 0){
			bad_mode = current_instruction->addressingModeReg;
			goto pc_error;
		}
		if (current_instruction->addressingModeReg == 1){
			bad_mode = current_instruction->addressingModeReg;
			goto pc_error;
		}
		if (current_instruction->addressingModeReg == 4){
			bad_mode = current_instruction->addressingModeReg;
			goto pc_error;
		}
		if (current_instruction->addressingModeReg == 5){
			bad_mode = current_instruction->addressingModeReg;
			goto pc_error;
		}		
	}
	else {
		if (current_instruction->addressingModeReg == 0){
			bad_mode = current_instruction->addressingModeReg;
			goto sp_error;
		}
	}
  }
  
  if ((current_instruction->srcBase == 7) || (current_instruction->srcBase == 6)) {
	if(current_instruction->srcBase == 7) { // PC Register Modes
		if (current_instruction->addressingModeSrc == 0){
			bad_mode = current_instruction->addressingModeSrc;
			goto pc_error;
		}
		if (current_instruction->addressingModeSrc == 1){
			bad_mode = current_instruction->addressingModeSrc;
			goto pc_error;
		}
		if (current_instruction->addressingModeSrc == 4){
			bad_mode = current_instruction->addressingModeSrc;
			goto pc_error;
		}
		if (current_instruction->addressingModeSrc == 5){
			bad_mode = current_instruction->addressingModeSrc;
			goto pc_error;
		}		
	}
	else {
		if (current_instruction->addressingModeSrc == 0){
			bad_mode = current_instruction->addressingModeSrc;
			goto sp_error;
		}
	}
  }
    
  if ((current_instruction->destBase == 7) || (current_instruction->destBase == 6)) {
	if(current_instruction->destBase == 7) { // PC Register Modes
		if (current_instruction->addressingModeDest == 0){
			bad_mode = current_instruction->addressingModeDest;
			goto pc_error;
		}
		if (current_instruction->addressingModeDest == 1){
			bad_mode = current_instruction->addressingModeDest;
			goto pc_error;
		}
		if (current_instruction->addressingModeDest == 4){
			bad_mode = current_instruction->addressingModeDest;
			goto pc_error;
		}
		if (current_instruction->addressingModeDest == 5){
			bad_mode = current_instruction->addressingModeDest;
			goto pc_error;
		}		
	}
	else {
		if (current_instruction->addressingModeDest == 0){
			bad_mode = current_instruction->addressingModeDest;
			goto sp_error;
		}
	}
  }
  
  if(verbosity_level > HIGH_VERBOSITY) cout << "In parse, opCode=" << oct << current_instruction->opcode << endl;
  if((current_instruction->opcode == m_JSR) && (current_instruction->addressingModeDest)) {
	  cerr << "TRAP!, to Vector Address 4, Invalid JSR addressing mode: " << current_instruction->addressingModeDest << ", not currently implemented" <<endl;
	  return 2;
  }


  return err;
sp_error:
	cerr << "Invalid SP addressing mode: " << bad_mode <<endl;
	return 1;
pc_error:
	cerr << "Invalid PC addressing mode: " << bad_mode <<endl;
	return 2;
}


// TODO, test PC and SP?
// This returns the address, doing N-1 trace statements. We could probably have the trace statements in
// the read_byte and read_word functions honestly.
uint16_t get_address(uint16_t mode, uint16_t baseAddress, bool trace)
{
  uint16_t X = 0;
  uint16_t workingAddress = 0;
  bool byte = current_instruction->byteMode;
  uint16_t resultAddress = 0;
  uint16_t offset = 0;
  
  // TODO define constants for all the values used for logic in here.
  if (baseAddress == 7)
  {
    switch (mode)
    {
    // Register
    // TODO What to do here?
    case 0000000: //INVALID
                  break;
    // Register deferred
    case 0000001: //INVALID
                  break;
    // Immediate
    case 0000002: resultAddress = PC;
                  //PC += 2; TODO i think this is unneeded.
                  break;
    // Immediate  deferred
    case 0000003: workingAddress = PC; 
                  // READ TRACE
                  if (byte)
                  {
                    resultAddress = read_byte(REGISTER_MODE, workingAddress, trace);
                  }
                  else
                  {
                    resultAddress = read_word(REGISTER_MODE, workingAddress, trace);
                  }
                  //PC += 2; TODO i think this is unneeded.
                  break;
    // Autodecrement
    case 0000004: // INVALID
                  break;
    // Autodecrement deferred
    case 0000005: // INVALID
                  break;
    // Relative
    case 0000006: 
                  offset = 2;
                  if(verbosity_level > HIGH_VERBOSITY) cout << "g_a offset: " << offset << "\n";
                  X = read_word(mode, PC, trace);
				          current_instruction->immediate = X;
                  //PC += 2;
                  resultAddress = PC + offset + X;
                  if(verbosity_level > HIGH_VERBOSITY) cout << "get_a PC: " << oct << PC << "X: " << oct << X << "\n";
                  break;
    // Relative deferred
    case 0000007: /*if ((modeTest == 6) || (modeTest == 7))
                    offset = 2;
                  else
                    offset = 0;*/
                  offset = 2;
                  X = read_word(mode, PC, trace);
				          current_instruction->immediate = X;
                  //PC += 2;
                  workingAddress = PC + offset + X;
                  // READ TRACE
                  if (byte)
                  {
                    resultAddress = read_byte(mode, workingAddress, trace);
                  }
                  else
                  {
                    resultAddress = read_word(mode, workingAddress, trace);
                  }
                  break;
    }
  }
  else if (baseAddress == 6)
  {

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
                  //SP += 2;
                  break;
    // Autoincrement deferred
    case 0000003: workingAddress = SP; 
                  // READ TRACE
                  if (byte)
                  {
                    resultAddress = read_byte(mode, workingAddress, trace);
                  }
                  else
                  {
                    resultAddress = read_word(mode, workingAddress, trace);
                  }
                  //SP += 2;
                  break;
    // Autodecrement
    case 0000004: //SP -= 2;
                  resultAddress = SP - 2;
                  break;
    // Autodecrement deferred
    case 0000005: cout << "INVALID MODE autodecrement deferred for SP" << "\n";
                  /*REGS[baseAddress] -= 2;
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
                  /*if ((modeTest == 6) || (modeTest == 7))
                    offset = 2;
                  else
                    offset = 0;*/
                  X = read_word(mode, PC, trace);
				          current_instruction->immediate = X;
                  //PC += 2;
                  resultAddress = SP + X;
                  break;
    // Index deferred
    case 0000007: workingAddress = SP; 
                  /*if ((modeTest == 6) || (modeTest == 7))
                    offset = 2;
                  else
                    offset = 0;*/
                  //TODO only read word?
                  // READ TRACE
                  X = read_word(mode, PC, trace);
				          current_instruction->immediate = X;
                  //PC += 2;
                  // READ TRACE
                  if (byte)
                  {
                    resultAddress = read_byte(mode, workingAddress + X, trace);
                  }
                  else
                  {
                    resultAddress = read_word(mode, workingAddress + X, trace);
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
                    resultAddress = read_byte(mode, workingAddress, trace);
                  }
                  else
                  {
                    resultAddress = read_word(mode, workingAddress, trace);
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
                    resultAddress = read_byte(mode, workingAddress, trace);
                  }
                  else
                  {
                    resultAddress = read_word(mode, workingAddress, trace);
                  }
                  break;
    // Index
    case 0000006: // READ TRACE
                  /*cout << "modeTest \n" << modeTest << "\n";
                  if ((modeTest == 6) || (modeTest == 7))
                    offset = 2;
                  else
                    offset = 0;*/
                  X = read_word(mode, PC, trace);
                  if(verbosity_level > HIGH_VERBOSITY) cout << "offset \n" << X << "\n";
				  current_instruction->immediate = X;
                  //PC += 2;
                  resultAddress = REGS[baseAddress] + X;
                  break;
    // Index deferred
    case 0000007: workingAddress = REGS[baseAddress];
                  //TODO only read word?
                  // READ TRACE
                  /*if ((modeTest == 6) | (modeTest == 7))
                    offset = 2;
                  else
                    offset = 0;*/
                  X = read_word(mode, PC, trace);
				  current_instruction->immediate = X;
                  //PC += 2;
                  // READ TRACE
                  if (byte)
                  {
                    resultAddress = read_byte(mode, workingAddress + X, trace);
                  }
                  else
                  {
                    resultAddress = read_word(mode, workingAddress + X, trace);
                  }
                  break;
    }
  }

  return resultAddress;
}

uint16_t get_value(uint16_t mode, uint16_t baseAddress, bool trace)
{
  uint16_t X = 0;
  uint16_t workingAddress = 0;
  bool byte = current_instruction->byteMode;
  uint16_t resultValue = 0;
  
  if (baseAddress == 7)
  {
    // PC ADDRESSING
    switch (mode)
    {
      // Register
      case 0000000: 
                    cerr << "Invalid PC addressing mode: 0\n";
                    break;
      // Register deferred
      case 0000001: 
                    cerr << "Invalid PC addressing mode: 1\n";
                    break;
      // Autoincrement
      case 0000002: workingAddress = PC;
                    if (byte)
                    {
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress, trace);
                    }
                    else
                    {
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, trace);
                    }
                    PC += 2;
                    break;
      // Autoincrement deferred
      case 0000003: // READ TRACE
                    workingAddress = PC;
                    if (byte)
                    {
                      workingAddress = read_byte(mode, workingAddress, trace);
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress, trace);
                    }
                    else
                    { 
                      workingAddress = read_word(mode, workingAddress, trace);
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, trace);
                    }
                    PC += 2;
                    break;
      // Autodecrement
      case 0000004: 
                    cerr << "Invalid PC addressing mode: 4\n";
                    break;
      // Autodecrement deferred
      case 0000005: 
                    cerr << "Invalid PC addressing mode: 5\n";
                    break;
      // Index
      case 0000006: // READ TRACE
                    X = read_word(mode, PC, trace);
					current_instruction->immediate = X;
                    PC += 2;
                    if (byte)
                    {
                      // READ TRACE
                      resultValue = read_byte(mode, PC + X, trace);
                    }
                    else
                    { 
                      // READ TRACE
                      resultValue = read_word(mode, PC + X, trace);
                    }
                    break;
      // Index deferred
      case 0000007: // READ TRACE
                    X = read_word(mode, PC, true);
					current_instruction->immediate = X;
                    PC += 2;
                    if (byte)
                    {
                      // READ TRACE
                      workingAddress = read_byte(mode, PC + X, trace);
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress, trace);
                    }
                    else
                    {
                      // READ TRACE
                      workingAddress = read_word(mode, PC + X, trace);
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, trace);
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
                      resultValue = read_byte(mode, workingAddress, trace);
                    }
                    else
                    {
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, trace);
                    }
                    break;
      // Autoincrement
      case 0000002: workingAddress = SP;
                    if (byte)
                    {
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress, trace);
                    }
                    else
                    {
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, trace);
                    }
                    SP += 2;
                    break;
      // Autoincrement deferred
      case 0000003: // READ TRACE
                    workingAddress = SP;
                    if (byte)
                    {
                      workingAddress = read_byte(mode, workingAddress, trace);
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress, trace);
                    }
                    else
                    { 
                      workingAddress = read_word(mode, workingAddress, trace);
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, trace);
                    }
                    SP += 2;
                    break;
      // Autodecrement
      case 0000004: SP -= 2;
                    workingAddress = SP;
                    if (byte)
                    {
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress, trace);
                    }
                    else
                    {
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, trace);
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
                    X = read_word(mode, PC, trace);
					current_instruction->immediate = X;
                    PC += 2;
                    if (byte)
                    {
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress + X, trace);
                    }
                    else
                    { 
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress + X, trace);
                    }
                    break;
      // Index deferred
      case 0000007: // READ TRACE
                    workingAddress = SP;
                    X = read_word(mode, PC, trace);
					current_instruction->immediate = X;
                    PC += 2;
                    if (byte)
                    {
                      // READ TRACE
                      workingAddress = read_byte(mode, workingAddress + X, trace);
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress, trace);
                    }
                    else
                    {
                      // READ TRACE
                      workingAddress = read_word(mode, workingAddress + X, trace);
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, trace);
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
                      resultValue = read_byte(mode, workingAddress, trace);
                    }
                    else
                    {
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, trace);
                    }
                    break;
      // Autoincrement
      case 0000002: workingAddress = REGS[baseAddress];
                    if (byte)
                    {
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress, trace);
                    }
                    else
                    {
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, trace);
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
                      workingAddress = read_byte(mode, workingAddress, trace);
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress, trace);
                    }
                    else
                    { 
                      workingAddress = read_word(mode, workingAddress, trace);
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, trace);
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
                      resultValue = read_byte(mode, workingAddress, trace);
                    }
                    else
                    {
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, trace);
                    }
                    break;
      // Autodecrement deferred
      case 0000005: REGS[baseAddress] -= 2;
                    workingAddress = REGS[baseAddress];
                    if (byte)
                    {
                      // READ TRACE
                      workingAddress = read_byte(mode, workingAddress, trace);
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress, trace);
                    }
                    else
                    { 
                      // READ TRACE
                      workingAddress = read_word(mode, workingAddress, trace);
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, trace);
                    }
                    break;
      // Index
      case 0000006: // READ TRACE
                    workingAddress = REGS[baseAddress];
                    X = read_word(mode, PC, trace);
					current_instruction->immediate = X;
                    PC += 2;
                    if (byte)
                    {
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress + X, trace);
                    }
                    else
                    { 
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress + X, trace);
                    }
                    break;
      // Index deferred
      case 0000007: // READ TRACE
                    workingAddress = REGS[baseAddress];
                    X = read_word(mode, PC, trace);
					current_instruction->immediate = X;
                    PC += 2;
                    if (byte)
                    {
                      // READ TRACE
                      workingAddress = read_byte(mode, workingAddress + X, trace);
                      // READ TRACE
                      resultValue = read_byte(mode, workingAddress, trace);
                    }
                    else
                    {
                      // READ TRACE
                      workingAddress = read_word(mode, workingAddress + X, trace);
                      // READ TRACE
                      resultValue = read_word(mode, workingAddress, trace);
                    }
                    break;
    }
  }
  return resultValue;
}

