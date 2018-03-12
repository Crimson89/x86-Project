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
// Only here to get rid of a compilation error.
}

static void printMemReg()
{
  cout << "\n";
  for (int i = 0; i < 20; i+=2)
  {
    printf("address: %u,%u   val: %04x\n", i, i+1, (MEM[i]) | (MEM[i+1] << 8) );
  }

  for (int i = 0; i <= 7; i++)
  {
    printf("Reg: %u   val: %u\n", i, REGS[i]);
  }
  cout << "\n";
}

static void fillMem(int val)
{ 
  for (int i = 0; i < MEMORY_SPACE; i++)
  {
    MEM[i] = val;
  }
}

static void clearReg()
{
  for (int i = 0; i <= 7; i++)
    REGS[i] = 0;
}

static int decodeTest()
{
  current_instruction = new instruction;
  //int size = 20;
  //uint8_t TESTMEM[size];
  uint16_t addInstruction = 00061314; // ADD (R4),(R3)  DOUBLE
  uint16_t movInstruction = 00010102; // MOV R2,R1      DOUBLE
  uint16_t clrInstruction = 00005025; // CLR (R5)+      SINGLE
  uint16_t addbInstruction = 0;
  uint16_t movbInstruction = 0;
  uint16_t clrbInstruction = 0;
  
  // DOUBLE          ADD  R1,R2     (R1),(R2) (R1)+,(R2)+  @(R1)+,@(R2)+  -(R1),-(R2)  @-(R1),@-(R2)  X(R1),R2       @X(R1),R2   R1,X(R2)       R1,@X(R2)    X(R1),X(R2)    @X(R1),@X(R2)
  uint16_t addArray[12] = {00060102, 00061112, 00062122,    00063132,      00064142,    00065152,      00066102,      00067102,   00060162,      00060172,   00066162,      00067172};
  // SINGLE          CLR  R5        (R5)      (R5)+     @(R5)+    -(R5)     @-(R5)    X(R5)     @X(R5)
  uint16_t clrArray[8] = {00005005, 00005015, 00005025, 00005035, 00005045, 00005055, 00005065, 00005075};
  // BRANCH
  uint16_t brArray[8] = {};
  uint16_t branch = 00000477;


  cout << "_________________\n";
  cout << "ADD R1,R2\n"; 
  int res;
  int val;
  int test;

  fillMem(0);

  val = 0;
  res = clearInstruction(current_instruction);
  res = parseInstruction(addArray[0], current_instruction);
  res = printInstruction(current_instruction); 
    
  R2 = 10;
  R1 = 14;
  
  //MEM[10] = 4;
  //MEM[14] = 3;

  printMemReg(); 

  test = dispatch(current_instruction);
  
  printMemReg();
  clearReg();
    

  cout << "_________________\n";
  cout << "ADD (R1),(R2)\n"; 
  val = 0;
  fillMem(0);
  res = clearInstruction(current_instruction);
  res = parseInstruction(addArray[1], current_instruction);
  res = printInstruction(current_instruction); 
    
  R2 = 10;
  R1 = 14;
  
  MEM[10] = 4;
  MEM[14] = 3;
  //MEM[4] = ;
  //MEM[3] = ;

  printMemReg(); 

  test = dispatch(current_instruction);
  
  printMemReg();
  clearReg(); 
 
  cout << "_________________\n"; 
  cout << "ADD (R1)+,(R2)+\n"; 
  val = 0;
  
  fillMem(0);

  res = clearInstruction(current_instruction);
  res = parseInstruction(addArray[2], current_instruction);
  res = printInstruction(current_instruction); 
    
  R2 = 10;
  R1 = 14;
  
  MEM[10] = 4;
  MEM[14] = 3;
  //MEM[4] = ;
  //MEM[3] = ;

  printMemReg(); 

  test = dispatch(current_instruction);
  
  printMemReg();
  clearReg(); 
  

  cout << "_________________\n"; 
  cout << "ADD @(R1)+,@(R2)+\n"; 
  val = 0;
  fillMem(0); 
  res = clearInstruction(current_instruction);
  res = parseInstruction(addArray[3], current_instruction);
  res = printInstruction(current_instruction); 
    
  R2 = 10;
  R1 = 14;
  
  MEM[10] = 2;
  MEM[11] = 0;
  MEM[14] = 4;
  MEM[15] = 0;
  MEM[4] = 12;
  MEM[2] = 12;

  printMemReg(); 

  test = dispatch(current_instruction);
  
  printMemReg();
  clearReg(); 
  
      cout << "_________________\n"; 
  cout << "ADD -(R1),-(R2)\n"; 
  val = 0;

  fillMem(0);
 
  
  res = clearInstruction(current_instruction);
  res = parseInstruction(addArray[4], current_instruction);
  res = printInstruction(current_instruction); 
    
  R2 = 10;
  R1 = 14;
  
  MEM[8] = 4;
  MEM[12] = 3;
  //MEM[4] = 12;
  //MEM[3] = 12;

  printMemReg(); 

  test = dispatch(current_instruction);
  
  printMemReg();
  clearReg();

  cout << "_________________\n"; 
  cout << "ADD @-(R1),@-(R2)\n"; 
  val = 0;
  fillMem(0); 
  res = clearInstruction(current_instruction);
  res = parseInstruction(addArray[5], current_instruction);
  res = printInstruction(current_instruction); 
    
  R2 = 10;
  R1 = 14;
  
  MEM[8] = 4;
  MEM[9] = 0;
  MEM[12] = 6;
  MEM[13] = 0;
  MEM[4] = 12;
  MEM[6] = 12;

  printMemReg(); 

  test = dispatch(current_instruction);
  
  printMemReg();
  clearReg();

  cout << "_________________\n"; 
  cout << "ADD 4(R1),R2\n"; 
  val = 0;
  fillMem(0);
  res = clearInstruction(current_instruction);
  res = parseInstruction(addArray[6], current_instruction);
  res = printInstruction(current_instruction); 
  cout << current_instruction->addressingModeSrc << "\n";
  cout << current_instruction->addressingModeDest << "\n";   
  PC = 18;
  R2 = 10;
  R1 = 8;
  
  MEM[14] = 2;
  MEM[8] = 8; 
  MEM[12] = 6;
  MEM[16] = 5;
  MEM[18] = 4;

  printMemReg(); 

  test = dispatch(current_instruction);
  
  printMemReg();
  clearReg();
  

  cout << "_________________\n"; 
  cout << "ADD @4(R1),R2\n"; 
  val = 0;
  fillMem(0);
  res = clearInstruction(current_instruction);
  res = parseInstruction(addArray[7], current_instruction);
  res = printInstruction(current_instruction); 
    
  PC = 16;
  R2 = 10;
  R1 = 14;
  
  MEM[18] = 2;
  MEM[16] = 4;
  MEM[14] = 2;
  MEM[2] = 4;

  printMemReg(); 

  test = dispatch(current_instruction);
  
  printMemReg();
  clearReg();

  cout << "_________________\n"; 
  cout << "ADD R1,4(R2)\n"; 
  val = 0;
  fillMem(0);
  res = clearInstruction(current_instruction);
  res = parseInstruction(addArray[8], current_instruction);
  res = printInstruction(current_instruction); 
  cout << current_instruction->addressingModeSrc << "\n";
  cout << current_instruction->addressingModeDest << "\n";   
  PC = 18;
  R2 = 10;
  R1 = 8;
  
  MEM[14] = 2;
  MEM[8] = 8; 
  MEM[12] = 6;
  MEM[16] = 5;
  MEM[18] = 4;

  printMemReg(); 

  test = dispatch(current_instruction);
  
  printMemReg();
  clearReg();
  

  cout << "_________________\n"; 
  cout << "ADD R1,@4(R2)\n"; 
  val = 0;
  fillMem(0);
  res = clearInstruction(current_instruction);
  res = parseInstruction(addArray[9], current_instruction);
  res = printInstruction(current_instruction); 
    
  PC = 16;
  R2 = 10;
  R1 = 14;
  
  MEM[18] = 2;
  MEM[16] = 4;
  MEM[14] = 2;
  MEM[2] = 4;

  printMemReg(); 

  test = dispatch(current_instruction);
  
  printMemReg();
  clearReg();

    cout << "_________________\n"; 
  cout << "ADD 2(R1),4(R2)\n"; 
  val = 0;
  fillMem(0);
  res = clearInstruction(current_instruction);
  res = parseInstruction(addArray[10], current_instruction);
  res = printInstruction(current_instruction); 
  cout << current_instruction->addressingModeSrc << "\n";
  cout << current_instruction->addressingModeDest << "\n";   
  PC = 16;
  R2 = 10;
  R1 = 8;
  
  MEM[14] = 8;
  MEM[8] = 8; 
  MEM[10] = 2;
  MEM[12] = 6;
  MEM[16] = 2;
  MEM[18] = 4;

  printMemReg(); 

  test = dispatch(current_instruction);
  
  printMemReg();
  clearReg();
  

  cout << "_________________\n"; 
  cout << "ADD @2(R1),@4(R2)\n"; 
  val = 0;
  fillMem(0);
  res = clearInstruction(current_instruction);
  res = parseInstruction(addArray[11], current_instruction);
  res = printInstruction(current_instruction); 
    
  PC = 16;
  R2 = 10;
  R1 = 6;
  
  MEM[8] = 4;
  MEM[6] = 4;
  MEM[18] = 4;
  MEM[16] = 2;
  MEM[14] = 2;
  MEM[2] = 4;
  MEM[4] = 10;

  printMemReg(); 

  test = dispatch(current_instruction);
  
  printMemReg();
  clearReg();
  /*
  cout << "_________________\n";
  cout << "MOV R1,R2\n";
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

  printMemReg();
  res = clearInstruction(current_instruction);
  res = parseInstruction(movInstruction, current_instruction);
  res = printInstruction(current_instruction);
  
  test = MOVTEST(current_instruction);
  printMemReg();
  clearReg(); 
*/

  
  cout << "_________________\n"; 
  cout << "CLR R5\n";

  val = 0;
  
  fillMem(0);
  
  R5 = 14;

  MEM[14] = 10;
  MEM[10] = 55;
  
  printMemReg();

  res = clearInstruction(current_instruction);
  res = parseInstruction(clrArray[0], current_instruction);
  res = printInstruction(current_instruction); 

  test = dispatch(current_instruction);
  printMemReg();
  clearReg();

    cout << "_________________\n"; 
  cout << "CLR (R5)\n";

  val = 0;
  fillMem(0);
  
  R5 = 14;

  MEM[14] = 10;
  MEM[10] = 55;
  
  printMemReg();

  res = clearInstruction(current_instruction);
  res = parseInstruction(clrArray[1], current_instruction);
  res = printInstruction(current_instruction); 

  test = dispatch(current_instruction);
  printMemReg();
  clearReg();

  cout << "_________________\n"; 
  cout << "CLR (R5)+\n";

  val = 0;
  
  fillMem(0);
  
  R5 = 14;

  MEM[14] = 10;
  MEM[10] = 55;
  
  printMemReg();

  res = clearInstruction(current_instruction);
  res = parseInstruction(clrArray[2], current_instruction);
  res = printInstruction(current_instruction); 

  test = dispatch(current_instruction);
  printMemReg();
  clearReg();

  cout << "_________________\n"; 
  cout << "CLR @(R5)+\n";

  val = 0;
  
  fillMem(0);
  
  R5 = 14;

  MEM[14] = 10;
  MEM[15] = 0;
  MEM[10] = 55;
  
  printMemReg();

  res = clearInstruction(current_instruction);
  res = parseInstruction(clrArray[3], current_instruction);
  res = printInstruction(current_instruction); 

  test = dispatch(current_instruction);
  printMemReg();
  clearReg();

  cout << "_________________\n"; 
  cout << "CLR -(R5)\n";

  val = 0;
  
  fillMem(0);
  
  R5 = 14;

  MEM[14] = 10;
  MEM[10] = 55;
  
  printMemReg();

  res = clearInstruction(current_instruction);
  res = parseInstruction(clrArray[4], current_instruction);
  res = printInstruction(current_instruction); 

  test = dispatch(current_instruction);
  printMemReg();
  clearReg();

    cout << "_________________\n"; 
  cout << "CLR @-(R5)\n";

  val = 0;
  
  fillMem(0);
  
  R5 = 14;

  MEM[12] = 16;
  MEM[13] = 0;
  MEM[14] = 10;
  MEM[10] = 55;
  
  printMemReg();

  res = clearInstruction(current_instruction);
  res = parseInstruction(clrArray[5], current_instruction);
  res = printInstruction(current_instruction); 

  test = dispatch(current_instruction);
  printMemReg();
  clearReg();

  PC = 0;

  res = clearInstruction(current_instruction);
  res = parseInstruction(branch, current_instruction);
  res = printInstruction(current_instruction);

  test = dispatch(current_instruction);
  printMemReg();
  clearReg();
  
  cout << "_________________\n"; 
  cout << "CLR 6(R5)\n";

  val = 0;
  
  fillMem(0);
  
  R5 = 10;

  MEM[16] = 9;
  MEM[14] = 10;
  MEM[10] = 55;
  
  printMemReg();

  res = clearInstruction(current_instruction);
  res = parseInstruction(clrArray[6], current_instruction);
  res = printInstruction(current_instruction); 

  test = dispatch(current_instruction);
  printMemReg();
  clearReg();

    cout << "_________________\n"; 
  cout << "CLR @6(R5)\n";

  val = 0;
  
  fillMem(0);
  
  R5 = 10;

  MEM[12] = 16;
  MEM[13] = 0;
  MEM[14] = 10;
  MEM[10] = 55;
  MEM[16] = 10;
  
  printMemReg();

  res = clearInstruction(current_instruction);
  res = parseInstruction(clrArray[7], current_instruction);
  res = printInstruction(current_instruction); 

  test = dispatch(current_instruction);
  printMemReg();
  clearReg();

  PC = 0;

  res = clearInstruction(current_instruction);
  res = parseInstruction(branch, current_instruction);
  res = printInstruction(current_instruction);

  test = dispatch(current_instruction);
  printMemReg();
  clearReg();
}

static int octalTest()
{

  uint16_t testInstruction = 00113435;

  uint16_t testByte = (testInstruction &       00100000) >> 15;
  uint16_t testOp = testInstruction &         00070000;
  uint16_t testSourceMode = testInstruction & 00007000;
  uint16_t testSource = testInstruction &     00000700;
  uint16_t testDestMode = testInstruction &   00000070;
  uint16_t testDest = testInstruction &       00000007;

  cout << "testInstruction" << oct << testInstruction << "\n";
  cout << "testByte" << oct << testByte << "\n";
  cout << "testOp" << oct << testOp << "\n";
  cout << "testSourceMode" << oct << testSourceMode << "\n";
  cout << "testSource" << oct << testSource << "\n";
  cout << "testDestMode" << oct << testDestMode << "\n";
  cout << "testDest" << oct << testDest << "\n";
  return 1;
}


int main()
{
  int result = 0;

  //result = parseTest();
  //result = octalTest();
  result = decodeTest();
  return 1;
}

/*int parseTest()
{

  // Create array of an instruction of every unique type,
  // then iterate through each of them. (94 instructions?)
  int numInstructions = 5;
  int error = 0;
  //uint16_t instructionArray[numInstructions] = {000573, 010573, 0005173, 0105173, 0005273}; // Oh boy, this is gonna be fun. Stopped at INCB

  //                                           MOV       DIV      CLR      BEQ      NOP
  uint16_t instructionArray[numInstructions] = {0014152, 0071341, 0005043, 0001466, 0000240};

  instruction newInstruction;

  //TODO maybe split these up by operand type?
  // also finish the fucker
  // TODO figure out why the fuck this doesn't work. Time for GDB.

  for (int i = 0; i < numInstructions; i++)
  {
    error = parseInstruction(instructionArray[i], &newInstruction);
    
    // Print each field out.
    // Source will always be (01), destination will always be (02), register will always be (03),
    // modes will always be (src:04 , dest:05), offset will always be (0066), individual mode is (07).
    // NZVC b10101

    cout << "opcode: " << std::oct << newInstruction.opcode << "\n";
    cout << "src: " << std::oct << newInstruction.src << "\n";
    cout << "modeSrc: " << std::oct << newInstruction.addressingModeSrc << "\n";
    cout << "dest: " << std::oct << newInstruction.dest << "\n";
    cout << "modeDest: " << std::oct << newInstruction.addressingModeDest << "\n";
    cout << "reg: " << std::oct << newInstruction.reg << "\n";
    cout << "modeReg: " << std::oct << newInstruction.registerMode << "\n";
    cout << "offset: " << std::oct << newInstruction.offset << "\n";
    cout << "\n";
  }

}*/
