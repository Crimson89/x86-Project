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
  result = octalTest();
  
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
