#include "header.h"


 
/*

Convenience function for printing relevant info in an instruction structure.
Arguments:
  newInstruction - A pointer to the instruction structure you want to modify.

*/
int printInstruction(instruction* newInstruction)
{
  cout << "Operation Name:" << newInstruction->op_text << endl;
  cout << "0pcode :" << setfill('0') << setw(7) << oct << newInstruction->opcode << endl;
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

/*

Convenience function for clearing out an instruction structure..
Arguments:
  newInstruction - A pointer to the instruction structure you want to modify.

*/
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
  newInstruction->is_branch = false;    
  newInstruction->branch_taken = false; 
  newInstruction->branch_target = 0;

  return 0;
}

/*

The main work horse of the instruction decode sequence. This function takes in a 16 bit instrucion code
  and determines what "family" it belongs to. Instructions were sorted into families based on common functionality
  and common data locations within the instruction code (like addressing modes and registers).
Arguments:
  newInstruction - A pointer to the instruction structure you want to modify.
  instructionCode - the 16 bit instruction code that was just fetched form memory
Returns
  An error code through an int for any invalid modes.

*/
int parseInstruction(uint16_t instructionCode, instruction* newInstruction)
{
    int err = 0; // will be error code. 0 = no error, non-zero = error

  // First thing is first, check against instructions with entirely unique opcodes.

  bool special = false;
  if(verbosity_level > HIGH_VERBOSITY) cout << "In parser, found ";
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

  // In the list of Ifs below, only JSR, RTS, and JMP are actually implemented.
  // Other instructions are caught for consistencies sake, but this is by no means an exhaustive list.

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
  if (!special& ((instructionCode & 0177700) == 0006400))
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
    current_instruction->addressingModeReg = 2; // Shouldn't matter, so just hard code something non harmful. 
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
    // If a special instruction above hasn't been caught, we have a general case.
    // This chain of conditionals narrows down the "family" of the isntruction, based off of unique fields where they exist
    /* All mask definitions are in header.h, under --PARSE-- header.
      General steps as follows.
      1. Check if the instruction is a single op, conditional branch or conditional check
        1.a If not check if it is a double op, or a double op register source (none of these are actually implemented)
      2. Check if it is a conditional (either branch or condition code)
        2.a If If not, it is a single op
      3. If we reached here, it's either a conditional branch or condition code operation
    */
    uint16_t relevantBits = instructionCode & maskRelevantBits;
    uint16_t bitPattern = relevantBits & maskSingleCondBranchCondCheck;
    uint16_t tempLocation;

    if (bitPattern == 0000000)
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
 
  // Save the current processor status word so that instructions have the ability to grab it and modify it.
  // Will be written back after instruction execution to update.
  current_instruction->PSW = PSW.PSW_BYTE;
  
  // Check for any invalid addressing modes for either SP or PC (some are technically allowed on SP, but should be heavily avoided)
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
  if((current_instruction->opcode == m_JSR) && (!(current_instruction->addressingModeDest))) {
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


/*

This function does the instruction decoding for addressing modes. It takes the addressing mode
  applied to a speicifc register, and returns the address of the value that is at the end of the
  decoding. It is used get an address for an item you wish to modify. This function does NOT
  increment any registers in increment or decrement modes.
  Please see the function comment for get_value for a further explanation of required use.
Arguments:
  mode - The mode applied to the register (baseAddress)
  baseAddress - The register index, representing what register the mode is to be applied to
  trace -  A boolean to turn off and on writing to the trace file for instruction, data, and memory
    reads and writes.

*/
uint16_t get_address(uint16_t mode, uint16_t baseAddress, bool trace)
{
  uint16_t X = 0;
  uint16_t workingAddress = 0;
  bool byte = current_instruction->byteMode;
  uint16_t resultAddress = 0;
  uint16_t offset = 0;
 
  // Decodes the addressing mode, and retrieves the proper address needed.
  // Has different operation for unique PC and SP addressing modes for clarity. 
  if (baseAddress == PC_REG_INDEX)
  {
    // An important note for PC addressing mdoes here, is that PC has already been icnremented form the previous instruction,
    // since the incrementing is done right after we fetch the instruction from memory.
    switch (mode)
    {
    // Register
    case 0000000: //INVALID
                  break;
    // Register deferred
    case 0000001: //INVALID
                  break;
    // Immediate
    case 0000002: resultAddress = PC;
                  break;
    // Immediate  deferred
    case 0000003: workingAddress = PC; 
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
                  resultAddress = PC + offset + X;
                  if(verbosity_level > HIGH_VERBOSITY) cout << "get_a PC: " << oct << PC << "X: " << oct << X << "\n";
                  break;
    // Relative deferred
    case 0000007: offset = 2;
                  X = read_word(mode, PC, trace);
				          current_instruction->immediate = X;
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
  else if (baseAddress == SP_REG_INDEX)
  {

    // Nothing is terribly special about these modes, this is just here for clarity
    // and to facilitate checking of invalid modes not recommended for SP.
    switch (mode)
    {
    // Register
    case 0000000: resultAddress = SP_REG_INDEX;
                  break;
    // Register deferred
    case 0000001: resultAddress = SP;
                  break;
    // Autoincrement
    case 0000002: resultAddress = SP;
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
                  break;
    // Autodecrement
    case 0000004: resultAddress = SP - 2;
                  break;
    // Autodecrement deferred
    case 0000005: workingAddress = SP - 2;
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
                  X = read_word(mode, PC, trace);
				          current_instruction->immediate = X;
                  resultAddress = SP + X;
                  break;
    // Index deferred
    case 0000007: workingAddress = SP; 
                  // READ TRACE
                  X = read_word(mode, PC, trace);
				          current_instruction->immediate = X;
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
                      // We don't want to actually change any values with get_address
                      // since that would mess up calls to get_value within the same instruction
                    }
                  else
                    {
                      // For consistency and readability.
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
                  break;
    // Autodecrement
    case 0000004: if (byte == true)
                  {
                    resultAddress = REGS[baseAddress] - 1;
                  }
                  else
                  {
                    resultAddress = REGS[baseAddress] - 2;
                  }
                  break;
    // Autodecrement deferred
    case 0000005: workingAddress = REGS[baseAddress] - 2;
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
                  X = read_word(mode, PC, trace);
                  if(verbosity_level > HIGH_VERBOSITY) cout << "offset \n" << X << "\n";
				          current_instruction->immediate = X;
                  resultAddress = REGS[baseAddress] + X;
                  break;
    // Index deferred
    case 0000007: workingAddress = REGS[baseAddress];
                  // READ TRACE
                  X = read_word(mode, PC, trace);
				          current_instruction->immediate = X;
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


/*


  This function actually returns the value at the end of the addressing mode decoding.
  To be used when a value from source, reg, or destination in an instruction is needed.
  NOTE: this function will increment values and decrement values in the modes that are relevatn,
  whereas get_address will not by design.
  This requires a certain order of calls to be made in the instructions themselves, as follows:
  1. Call get_value on source (if a value is needed from source, and in a double op)
  2. Call get_address on destination
  3. Call get_value on destination (a dummy call is needed to facilitate incrementing and decrementing
     even if not using the value).Arguments:
  mode - The mode applied to the register (baseAddress)
  baseAddress - The register index, representing what register the mode is to be applied to
  trace -  A boolean to turn off and on writing to the trace file for instruction, data, and memory
    reads and writes.

*/
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
      case 0000000: // INVALID 
                    break;
      // Register deferred
      case 0000001: // INVALID
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
      case 0000000: resultValue = SP;
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
      case 0000005: SP -= 2;
                    workingAddress = SP;
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

