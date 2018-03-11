#include "header.h"

bool MEM_USED_FLAGS[MEMORY_SPACE]; //dummy define for compilation error
bool BREAK_POINT[MEMORY_SPACE/2];  //dummy define for compilation error

uint16_t REGS[7];
uint8_t MEM[MEMORY_SPACE];
uint16_t& R0 = REGS[0];
uint16_t& R1 = REGS[1];
uint16_t& R2 = REGS[2];
uint16_t& R3 = REGS[3];
uint16_t& R4 = REGS[4];
uint16_t& R5 = REGS[5];
uint16_t& R6 = REGS[6];
uint16_t& R7 = REGS[7];
uint16_t& SP = REGS[6];
uint16_t& PC = REGS[7];
uint16_t starting_pc;
instruction * current_instruction;	// decoded instruction information
int verbosity_level;            // Level of verbosity in print statements
string trace_file;
string data_file;

void get_user_octal(string prompt, string error_text, uint16_t &word)
{
// Only hear to get rid of a compilation error.
}

static int ADDTEST(instruction * instr)
{
  uint16_t src;
  uint16_t dest;

  uint16_t destAddress = get_address(instr->addressingModeDest, instr->destBase);
  src = get_value(instr->addressingModeSrc, instr->srcBase);
  dest = get_value(instr->addressingModeDest, instr->destBase);

  //cout << "\nsourceVal: \n" << hex << src << "\n";
  //cout << "\ndestVal: \n" << hex << dest << "\n";
  //uint16_t destAddress;


  //cout << "\ndestAddress: \n" << destAddress << "\n";

  dest = dest + src;

  //cout << "\ndestVal: \n" << hex << dest << "\n";
  
  write_word(instr->addressingModeDest, destAddress, dest, false);
  
  return 1;
}

static int MOVTEST(instruction * instr)
{ 
  uint16_t src;
  //uint16_t dest;

  src = get_value(instr->addressingModeSrc, instr->srcBase);
  //dest = get_value(instr->addressingModeDest, instr->destBase);

  uint16_t destAddress;

  destAddress = get_address(instr->addressingModeDest, instr->destBase);

  write_word(instr->addressingModeDest, destAddress, src, false);

  return 1;
}

static int CLRTEST(instruction * instr)
{
  uint16_t regDummyVal;
  uint16_t regAddress;
  regAddress = get_address(instr->addressingModeReg, instr->regBase);
  regDummyVal = get_value(instr->addressingModeReg, instr->regBase);

  //cout << "\nregAddress: \n" << regAddress;
  //cout << "\nregAddressMode: \n" << instr->addressingModeReg;
  //cout << "\nregBase: \n" << instr->regBase << "\n";

  write_word(instr->addressingModeReg, regAddress, 0, false);
}

static int BRTEST(instruction * instr)
{
  //cout << "\nPC: \n" << PC;
  uint16_t offset;
  offset = instr->offset;

  //cout << "\noffset: \n" << offset;

  PC = PC + offset;
 
  //cout << "\nPC: \n" << PC;
}

static void printMemReg()
{
  //cout << "\n";
  for (int i = 0; i < 20; i+=2)
  {
    printf("address: %u,%u   val: %04x\n", i, i+1, (MEM[i]) | (MEM[i+1] << 8) );
  }

  for (int i = 0; i <= 7; i++)
  {
    printf("Reg: %u   val: %u\n", i, REGS[i]);
  }
  //cout << "\n";
}

static void clearReg()
{
  for (int i = 0; i <= 7; i++)
    REGS[i] = 0;
}

static int decodeTest()
{
  current_instruction = new instruction;
  int size = 20;
  //uint8_t TESTMEM[size];
  uint16_t addInstruction = 00061314; // ADD (R4),(R3)  DOUBLE
  uint16_t movInstruction = 00010102; // MOV R2,R1      DOUBLE
  uint16_t clrInstruction = 00005025; // CLR (R5)+      SINGLE
  uint16_t addbInstruction = 0;
  uint16_t movbInstruction = 0;
  uint16_t clrbInstruction = 0;
  
  // DOUBLE          ADD  R2,R1     (R2),(R1) (R2)+,(R1)+  @(R2)+,@(R1)+  -(R2),-(R1)  @-(R2),@-(R1)  X(R2),X(R1)    @X(R2),@X(R1)
  uint16_t addArray[8] = {00060102, 00061112, 00062122,    00063132,      00064142,    00065152};//,      00066162,      00067172};
  // SINGLE          CLR  R5        (R5)      (R5)+     @(R5)+    -(R5)     @-(R5)    X(R5)     @X(R5)
  uint16_t clrArray[8] = {00005005, 00005015, 00005025, 00005035, 00005045, 00005055};//, 00005061, 00005071};
  // BRANCH
  uint16_t brArray[8] = {};
  uint16_t branch = 00000477;


  cout << "_________________\n";
  cout << "Testing: ADD R2,R1\n"; 
  int res;
  int val;
  int test;

  val = 0;
  for (int i = 0; i < size; i++)
  {
    MEM[i] = val;
    val++;
  }
  res = clearInstruction(current_instruction);
  res = parseInstruction(addArray[0], current_instruction);
  //res = printInstruction(current_instruction); 
    
  R2 = 10;
  R1 = 14;
  
  //MEM[10] = 4;
  //MEM[14] = 3;

  //printMemReg(); 

  test = ADDTEST(current_instruction);
  cout << "Instruction: " << op_formatted(current_instruction) << endl;
  
  //printMemReg();
  clearReg();
    

  cout << "_________________\n";
  cout << "Testing: ADD (R2),(R1)\n"; 
  val = 0;
  for (int i = 0; i < size; i++)
  {
    MEM[i] = val;
    val++;
  }
  res = clearInstruction(current_instruction);
  res = parseInstruction(addArray[1], current_instruction);
  //res = printInstruction(current_instruction); 
    
  R2 = 10;
  R1 = 14;
  
  MEM[10] = 4;
  MEM[14] = 3;
  //MEM[4] = ;
  //MEM[3] = ;

  //printMemReg(); 

  test = ADDTEST(current_instruction);
  cout << "Instruction: " << op_formatted(current_instruction) << endl;
  
  //printMemReg();
  clearReg(); 
 
  cout << "_________________\n"; 
  cout << "Testing: ADD (R2)+,(R1)+\n"; 
  val = 0;
  for (int i = 0; i < size; i++)
  {
    MEM[i] = val;
    val++;
  }
  res = clearInstruction(current_instruction);
  res = parseInstruction(addArray[2], current_instruction);
  //res = printInstruction(current_instruction); 
    
  R2 = 10;
  R1 = 14;
  
  MEM[10] = 4;
  MEM[14] = 3;
  //MEM[4] = ;
  //MEM[3] = ;

  //printMemReg(); 

  test = ADDTEST(current_instruction);
  cout << "Instruction: " << op_formatted(current_instruction) << endl;
  
  //printMemReg();
  clearReg(); 
  

    cout << "_________________\n"; 
  cout << "Testing: ADD @(R2)+,@(R1)+\n"; 
  val = 0;
  for (int i = 0; i < size; i++)
  {
    MEM[i] = val;
    val++;
  }
  res = clearInstruction(current_instruction);
  res = parseInstruction(addArray[3], current_instruction);
  //res = printInstruction(current_instruction); 
    
  R2 = 10;
  R1 = 14;
  
  MEM[10] = 2;
  MEM[11] = 0;
  MEM[14] = 4;
  MEM[15] = 0;
  MEM[4] = 12;
  MEM[2] = 12;

  //printMemReg(); 

  test = ADDTEST(current_instruction);
  cout << "Instruction: " << op_formatted(current_instruction) << endl;
  
  //printMemReg();
  clearReg(); 
  
      cout << "_________________\n"; 
  cout << "Testing: ADD -(R2),-(R1)\n"; 
  val = 0;
  for (int i = 0; i < size; i++)
  {
    MEM[i] = val;
    val++;
  }
  res = clearInstruction(current_instruction);
  res = parseInstruction(addArray[4], current_instruction);
  //res = printInstruction(current_instruction); 
    
  R2 = 10;
  R1 = 14;
  
  MEM[10] = 4;
  MEM[14] = 3;
  //MEM[4] = 12;
  //MEM[3] = 12;

  //printMemReg();

  test = ADDTEST(current_instruction);
  cout << "Instruction: " << op_formatted(current_instruction) << endl;
  
  //printMemReg();
  clearReg();

      cout << "_________________\n"; 
  cout << "Testing: ADD @-(R2),@-(R1)\n"; 
  val = 0;
  for (int i = 0; i < size; i++)
  {
    MEM[i] = val;
    val++;
  }
  res = clearInstruction(current_instruction);
  res = parseInstruction(addArray[5], current_instruction);
  //res = printInstruction(current_instruction); 
    
  R2 = 10;
  R1 = 14;
  
  MEM[8] = 4;
  MEM[9] = 0;
  MEM[12] = 6;
  MEM[13] = 0;
  MEM[4] = 12;
  MEM[6] = 12;

 // printMemReg(); 

  test = ADDTEST(current_instruction);
  cout << "Instruction: " << op_formatted(current_instruction) << endl;
  
  //printMemReg();
  clearReg();




 
  cout << "_________________\n";
  cout << "Testing: MOV R1,R2\n";
  val = 0;
  for (int i = 0; i < size; i++)
  {
    MEM[i] = val;
    val++;
  }

  R2 = 10;
  R1 = 14;
  
  MEM[10] = 4;
  MEM[14] = 3;

  //printMemReg();
  res = clearInstruction(current_instruction);
  res = parseInstruction(movInstruction, current_instruction);
  //res = printInstruction(current_instruction);
  
  test = MOVTEST(current_instruction);
  //printMemReg();
  clearReg(); 

  cout << "_________________\n"; 
  cout << "Testing: CLR R5\n";

  val = 0;
  for (int i = 0; i < size; i++)
  {
    MEM[i] = val;
    val++;
  }
  
  R5 = 14;

  MEM[14] = 10;
  MEM[10] = 55;
  
  //printMemReg();

  res = clearInstruction(current_instruction);
  res = parseInstruction(clrArray[0], current_instruction);
  //res = printInstruction(current_instruction); 

  test = CLRTEST(current_instruction);
  cout << "Instruction: " << op_formatted(current_instruction) << endl;
  //printMemReg();
  clearReg();

    cout << "_________________\n"; 
  cout << "Testing: CLR (R5)\n";

  val = 0;
  for (int i = 0; i < size; i++)
  {
    MEM[i] = val;
    val++;
  }
  
  R5 = 14;

  MEM[14] = 10;
  MEM[10] = 55;
  
  //printMemReg();

  res = clearInstruction(current_instruction);
  res = parseInstruction(clrArray[1], current_instruction);
  //res = printInstruction(current_instruction); 

  test = CLRTEST(current_instruction);
  cout << "Instruction: " << op_formatted(current_instruction) << endl;
  //printMemReg();
  clearReg();

  cout << "_________________\n"; 
  cout << "Testing: CLR (R5)+\n";

  val = 0;
  for (int i = 0; i < size; i++)
  {
    MEM[i] = val;
    val++;
  }
  
  R5 = 14;

  MEM[14] = 10;
  MEM[10] = 55;
  
  //printMemReg();

  res = clearInstruction(current_instruction);
  res = parseInstruction(clrArray[2], current_instruction);
  //res = printInstruction(current_instruction); 

  test = CLRTEST(current_instruction);
  cout << "Instruction: " << op_formatted(current_instruction) << endl;
  //printMemReg();
  clearReg();

  cout << "_________________\n"; 
  cout << "Testing: CLR @(R5)+\n";

  val = 0;
  for (int i = 0; i < size; i++)
  {
    MEM[i] = val;
    val++;
  }
  
  R5 = 14;

  MEM[14] = 10;
  MEM[15] = 0;
  MEM[10] = 55;
  
  //printMemReg();

  res = clearInstruction(current_instruction);
  res = parseInstruction(clrArray[3], current_instruction);
  //res = printInstruction(current_instruction); 

  test = CLRTEST(current_instruction);
  cout << "Instruction: " << op_formatted(current_instruction) << endl;
  //printMemReg();
  clearReg();

  cout << "_________________\n"; 
  cout << "Testing: CLR -(R5)\n";

  val = 0;
  for (int i = 0; i < size; i++)
  {
    MEM[i] = val;
    val++;
  }
  
  R5 = 14;

  MEM[14] = 10;
  MEM[10] = 55;
  
  //printMemReg();

  res = clearInstruction(current_instruction);
  res = parseInstruction(clrArray[4], current_instruction);
  //res = printInstruction(current_instruction); 

  test = CLRTEST(current_instruction);
  cout << "Instruction: " << op_formatted(current_instruction) << endl;
  //printMemReg();
  clearReg();

  cout << "_________________\n"; 
  cout << "Testing: CLR @-(R5)\n";

  val = 0;
  for (int i = 0; i < size; i++)
  {
    MEM[i] = val;
    val++;
  }
  
  R5 = 14;

  MEM[12] = 16;
  MEM[13] = 0;
  MEM[14] = 10;
  MEM[10] = 55;
  
  //printMemReg();

  res = clearInstruction(current_instruction);
  res = parseInstruction(clrArray[5], current_instruction);
  //res = printInstruction(current_instruction); 

  test = CLRTEST(current_instruction);
  cout << "Instruction: " << op_formatted(current_instruction) << endl;
  //printMemReg();
  clearReg();

  PC = 0;

  res = clearInstruction(current_instruction);
  res = parseInstruction(branch, current_instruction);
  //res = printInstruction(current_instruction);

  test = BRTEST(current_instruction);
  //printMemReg();
  clearReg();
}


int main()
{
	trace_file = "test_trace.txt";
	data_file = "FALSE";
	SP = 0xFFFF; // Assign at start to invalid value, detection of unassigned SP
	PC = 0xFFFF; // Assign at start to invalid value, detection of unassigned PC
	starting_pc = PC;
	initializeMemory();
  int result = 0;
  result = decodeTest();
  return 1;
}
