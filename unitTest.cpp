#include "header.h"

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

}

uint8_t TESTMEM[20];
static int ADDTEST(instruction * instr)
{
  uint16_t src;
  uint16_t dest;

  src = get_value(instr->addressingModeSrc, instr->srcBase);
  dest = get_value(instr->addressingModeDest, instr->destBase);

  cout << src << "\n";
  cout << dest << "\n";
  uint16_t destAddress;

  destAddress = get_address(instr->addressingModeDest, instr->destBase);

  cout << destAddress << "\n";

  dest = dest + src;

  cout << dest << "\n";
  
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
  uint16_t reg;
  //uint16_t dest;

  reg = get_value(instr->addressingModeReg, instr->regBase);
  //dest = get_value(instr->addressingModeDest, instr->destBase);

  uint16_t regAddress;

  regAddress = get_address(instr->addressingModeReg, instr->regBase);

  write_word(instr->addressingModeReg, regAddress, 0, false);
}

static int decodeTest()
{
  current_instruction = new instruction;
  int size = 20;
  //uint8_t TESTMEM[size];
  uint16_t addInstruction = 00061314; // ADD (R4),(R3)
  uint16_t movInstruction = 00010102; // MOV R2,R1
  uint16_t clrInstruction = 00005025; // CLR (R5)+
  uint16_t addbInstruction = 0;
  uint16_t movbInstruction = 0;
  uint16_t clrbInstruction = 0;

  cout << 1;

  current_instruction->srcBase = (addInstruction & maskDoubleSource) >> 6;
  printf("\nsrcBase: %o, addInstruction: %o, maskDoubleSource: %o\n", current_instruction->srcBase, addInstruction, maskDoubleSource);
  
  current_instruction->destBase = addInstruction & maskDoubleDest;
  printf("\ndestBase: %o, addInstruction: %o, maskDoubleDest: %o\n", current_instruction->destBase, addInstruction, maskDoubleDest);
  
  current_instruction->addressingModeSrc = (addInstruction & maskDoubleSourceMode) >> 9;
  printf("\nsrcMode: %o, addInstruction: %o, maskDoubleSrcMode: %o\n", current_instruction->addressingModeSrc, addInstruction, maskDoubleSourceMode);
  
  current_instruction->addressingModeDest = (addInstruction & maskDoubleDestMode) >> 3;
  printf("\ndestMode: %o, addInstruction: %o, maskDoubleDestMode: %o\n", current_instruction->addressingModeDest, addInstruction, maskDoubleDestMode);
  
  current_instruction->byteMode = (addInstruction & maskByteMode) >> 15;
  printf("\nByte: %o, addInstruction: %o, maskDoubleByte: %o\n", current_instruction->byteMode, addInstruction, maskByteMode);


  int val = 0;
  for (int i = 0; i < size; i++)
  {
    MEM[i] = val;
    val++;
  }

  cout << 2;
  R4 = 10;
  R3 = 14;
  
  MEM[10] = 4;
  MEM[14] = 3;
  
  cout << 3;
  for (int i = 0; i < 20; i++)
  {
    printf("\naddress: %d   val: %u\n", i, MEM[i]);
  }

  for (int i = 0; i <= 7; i++)
  {
    printf("\nReg: %d   val: %u\n", i, REGS[i]);
  }

  int test = ADDTEST(current_instruction);
  
  // Result: R4 = 10, R3 = 14, TESTMEM[10] = 5, TESTMEM[14] = 3
  
  for (int i = 0; i < size; i++)
  {
    printf("\naddress: %d   val: %u\n", i, MEM[i]);
  }

  for (int i = 0; i <= 7; i++)
  {
    printf("\nReg: %d   val: %u\n", i, REGS[i]);
  }
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
