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
  dest = dest + src;
  write_word(instr->addressingModeDest, destAddress, dest, false);
  
  return 1;
}

static int MOVTEST(instruction * instr)
{ 
  uint16_t src;
  src = get_value(instr->addressingModeSrc, instr->srcBase);

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
  write_word(instr->addressingModeReg, regAddress, 0, false);
}

static int BRTEST(instruction * instr)
{
  uint16_t offset;
  offset = instr->offset;
  PC = PC + offset;
}

static void printMemReg()
{

  for (int i = 0; i < 20; i+=2)
  {
    printf("address: %u,%u   val: %04x\n", i, i+1, (MEM[i]) | (MEM[i+1] << 8) );
  }

  for (int i = 0; i <= 7; i++)
  {
    printf("Reg: %u   val: %u\n", i, REGS[i]);
  }
}

static void clearReg()
{
  for (int i = 0; i <= 7; i++)
    REGS[i] = 0;
}

static void br_wrapper(uint16_t inst_in, instruction * current_instruction, string test_string)
{
  cout << "_________________\n";
  cout << test_string << endl; 
  int res;
  int test;
  
  clearReg();
  res = clearInstruction(current_instruction);
  res = parseInstruction(inst_in, current_instruction);
  test = BRTEST(current_instruction);
  cout << "Instruction: " << op_formatted(current_instruction) << endl;
}


static void add_wrapper(uint16_t inst_in, instruction * current_instruction, string test_string)
{
  cout << "_________________\n";
  cout << test_string << endl; 
  int res;
  int test;
  
  clearReg();
  res = clearInstruction(current_instruction);
  res = parseInstruction(inst_in, current_instruction);
  test = ADDTEST(current_instruction);
  cout << "Instruction: " << op_formatted(current_instruction) << endl;
}

static void clr_wrapper(uint16_t inst_in, instruction * current_instruction, string test_string)
{
  cout << "_________________\n";
  cout << test_string << endl; 
  int res;
  int test;
  
  clearReg();
  res = clearInstruction(current_instruction);
  res = parseInstruction(inst_in, current_instruction);
  test = CLRTEST(current_instruction);
  cout << "Instruction: " << op_formatted(current_instruction) << endl;
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
  
  // DOUBLE
  string addStringArray[8] = {"Testing:     ADD R1,R2", 
									"Testing:     ADD (R1),(R2)", 
									"Testing:     ADD (R1)+,(R2)+", 
									"Testing:     ADD @(R1)+,@(R2)+", 
									"Testing:     ADD -(R1),-(R2)", 
									"Testing:     ADD @-(R1),@-(R2)", 
									"Testing:     ADD X(R1),X(R2)", 
									"Testing:     ADD @X(R1),@X(R2)"};
  uint16_t addArray[8] = {00060102, 00061112, 00062122,    00063132,      00064142,    00065152,      00066162,      00067172};
  string addPCStringArray[8] = {"Testing:     ADD PC,R2", 
									"Testing:     ADD (PC),(R2)", 
									"Testing:     ADD (PC)+,(R2)+", 
									"Testing:     ADD @(PC)+,@(R2)+", 
									"Testing:     ADD -(PC),-(R2)", 
									"Testing:     ADD @-(PC),@-(R2)", 
									"Testing:     ADD X(PC),X(R2)", 
									"Testing:     ADD @X(PC),@X(R2)"};
  uint16_t addPCArray[8] = {00060702, 00061712, 00062722,    00063732,      00064742,    00065752,      00066762,      00067772};
  string addSPStringArray[8] = {"Testing:     ADD SP,R2", 
								"Testing:     ADD (SP),(R2)", 
								"Testing:     ADD (SP)+,(R2)+", 
								"Testing:     ADD @(SP)+,@(R2)+", 
								"Testing:     ADD -(SP),-(R2)", 
								"Testing:     ADD @-(SP),@-(R2)", 
								"Testing:     ADD X(SP),X(R2)", 
								"Testing:     ADD @X(SP),@X(R2)"};
  uint16_t addSPArray[8] = {00060602, 00061612, 00062622,    00063632,      00064642,    00065652,      00066662,      00067672};
  
  string addPCStringArray2[8] = {"Testing:     ADD R2,PC", 
									"Testing:     ADD (R2),(PC)", 
									"Testing:     ADD (R2)+,(PC)+", 
									"Testing:     ADD @(R2)+,@(PC)+", 
									"Testing:     ADD -(R2),-(PC)", 
									"Testing:     ADD @-(R2),@-(PC)", 
									"Testing:     ADD X(R2),X(PC)", 
									"Testing:     ADD @X(R2),@X(PC)"};
  uint16_t addPCArray2[8] = {00060207, 00061217, 00062227,    00063237,      00064247,    00065257,      00066267,      00067277};
  string addSPStringArray2[8] = {"Testing:     ADD R2,SP", 
								"Testing:     ADD (R2),(SP)", 
								"Testing:     ADD (R2)+,(SP)+", 
								"Testing:     ADD @(R2)+,@(SP)+", 
								"Testing:     ADD -(R2),-(SP)", 
								"Testing:     ADD @-(R2),@-(SP)", 
								"Testing:     ADD X(R2),X(SP)", 
								"Testing:     ADD @X(R2),@X(SP)"};
  uint16_t addSPArray2[8] = {00060206, 00061216, 00062226,    00063236,      00064246,    00065256,      00066266,      00067276};
  
  // SINGLE
   string clrStringArray[8] = {"Testing:     CLR,R2", 
									"Testing:     CLR (R2)", 
									"Testing:     CLR (R2)+", 
									"Testing:     CLR @(R2)+", 
									"Testing:     CLR -(R2)", 
									"Testing:     CLR @-(R2)", 
									"Testing:     CLR X(R2)", 
									"Testing:     CLR @X(R2)"};
  uint16_t clrArray[8] = {00005002, 00005012, 00005022, 00005032, 00005042, 00005052, 00005062, 00005072};
   string clrSPStringArray[8] = {"Testing:     CLR,SP", 
									"Testing:     CLR (SP)", 
									"Testing:     CLR (SP)+", 
									"Testing:     CLR @(SP)+", 
									"Testing:     CLR -(SP)", 
									"Testing:     CLR @-(SP)", 
									"Testing:     CLR X(SP)", 
									"Testing:     CLR @X(SP)"};
  uint16_t clrSPArray[8] = {00005006, 00005016, 00005026, 00005036, 00005046, 00005056, 00005066, 00005076};
   string clrPCStringArray[8] = {"Testing:     CLR,PC", 
									"Testing:     CLR (PC)", 
									"Testing:     CLR (PC)+", 
									"Testing:     CLR @(PC)+", 
									"Testing:     CLR -(PC)", 
									"Testing:     CLR @-(PC)", 
									"Testing:     CLR X(PC)", 
									"Testing:     CLR @X(PC)"};
  uint16_t clrPCArray[8] = {00005007, 00005017, 00005027, 00005037, 00005047, 00005057, 00005067, 00005077};
  
  // BRANCH
  string brStringArray[1] = {"Testing:     BR R1"};
  uint16_t brArray[1] = {00000401};
  uint16_t branch;
  
  // Jump
  
  
  // RTS
  
  
  // JSR

	//Test double ops
 	for (int i = 0; i < 8; i++)
		add_wrapper(addArray[i], current_instruction, addStringArray[i]);
 	for (int i = 0; i < 8; i++)
		add_wrapper(addSPArray[i], current_instruction, addSPStringArray[i]);
 	for (int i = 0; i < 8; i++)
		add_wrapper(addPCArray[i], current_instruction, addPCStringArray[i]);
 	for (int i = 0; i < 8; i++)
		add_wrapper(addSPArray2[i], current_instruction, addSPStringArray2[i]);
 	for (int i = 0; i < 8; i++)
		add_wrapper(addPCArray2[i], current_instruction, addPCStringArray2[i]);
	
	//Test single ops
	for (int i = 0; i < 8; i++)
		clr_wrapper(clrArray[i], current_instruction, clrStringArray[i]);
	for (int i = 0; i < 8; i++)
		clr_wrapper(clrSPArray[i], current_instruction, clrSPStringArray[i]);
	for (int i = 0; i < 8; i++)
		clr_wrapper(clrPCArray[i], current_instruction, clrPCStringArray[i]);
	
	//Test Branch
	for (int i = 0; i < 1; i++)
		br_wrapper(brArray[i], current_instruction, brStringArray[i]);
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
