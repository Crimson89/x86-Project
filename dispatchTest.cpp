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
instruction * current_instruction;	//decoded instruction information
int verbosity_level = 0;            //Level of verbosity in print statements
string trace_file = "trace.txt";
string data_file = "addc.ascii";
string branch_trace_file;
PSW_t PSW;

void get_user_octal(string prompt, string error_text, uint16_t &word)
{
//Only here to get rid of a compilation error.
}

void clear_psw(PSW_t & PSW) {
	PSW.PSW_BYTE = 0;
}

void print_psw(PSW_t & PSW) {
	cout << "Global PSW: " << setfill('0') << setw(3) << oct << uint16_t(PSW.PSW_BYTE) << endl;
	cout << "Global PSW.SPL: " << setfill('0') << setw(1) << uint16_t(PSW.SPL) << endl;
	cout << "Global PSW.T: " << setfill('0') << setw(1) << uint16_t(PSW.T) << endl;
	cout << "Global PSW.N: " << setfill('0') << setw(1) << uint16_t(PSW.N) << endl;
	cout << "Global PSW.Z: " << setfill('0') << setw(1) << uint16_t(PSW.Z) << endl;
	cout << "Global PSW.V: " << setfill('0') << setw(1) << uint16_t(PSW.V) << endl;
	cout << "Global PSW.C: " << setfill('0') << setw(1) << uint16_t(PSW.C) << endl;
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

static int operationTest()
{
	current_instruction = new instruction;

	// Variables for changing Byte/Word instruction,
	// Addressing mode, and register
	// Single is for single ops, double is for double ops
	// Change the spots with "x" to desired value

	// Byte mode                 x00000
	uint16_t byte_mode   =      0000000;

	// Addr mode                 0000x0
	uint16_t single_mode =      0000000;

	// Register                  00000x
	uint16_t single_reg  =      0000001;

	// Addr mode 1               00x000
	uint16_t double_mode_src =  0000000;

	// Register 1                000x00
	uint16_t double_src =       0000100;

	// Addr mode 2               0000x0
	uint16_t double_mode_dest = 0000000;

	// Register 2                00000x
	uint16_t double_dest =      0000002;

	//SINGLE OPERAND

	//General
	uint16_t CLR  =  0005000 + byte_mode + single_mode + single_reg; // Clear
	uint16_t COM  =  0005100 + byte_mode + single_mode + single_reg; // 1's Compliment
	uint16_t INC  =  0005200 + byte_mode + single_mode + single_reg; // Increment
	uint16_t DEC  =  0005300 + byte_mode + single_mode + single_reg; // Decrement
	uint16_t NEG  =  0005400 + byte_mode + single_mode + single_reg; // 2's Compliment negate
	uint16_t TST  =  0005700 + byte_mode + single_mode + single_reg; // Test

	//Shift & Rotate
	uint16_t ASR  =  0006200 + byte_mode + single_mode + single_reg; // Arithmetic shift right
	uint16_t ASL  =  0006300 + byte_mode + single_mode + single_reg; // Arithmetic shift left
	uint16_t ROR  =  0006000 + byte_mode + single_mode + single_reg; // Rotate right
	uint16_t ROL  =  0006100 + byte_mode + single_mode + single_reg; // Rotate left
	uint16_t SWAB =  0000300 + single_mode + single_reg; // Swap bytes

	//Multiple Precision
	uint16_t ADC  =  0005500 + byte_mode + single_mode + single_reg; // Add carry
	uint16_t SBC  =  0005600 + byte_mode + single_mode + single_reg; // Subtract carry
	uint16_t SXT  =  0006700 + single_mode + single_reg; // Sign extend

	//DOUBLE OPERAND

	//Logical
	uint16_t BIT  =  0030000 + byte_mode + double_mode_src + double_src + double_mode_dest + double_dest; // Bit test
	uint16_t BIC  =  0040000 + byte_mode + double_mode_src + double_src + double_mode_dest + double_dest; // Bit clear
	uint16_t BIS  =  0050000 + byte_mode + double_mode_src + double_src + double_mode_dest + double_dest; // Bit set
	uint16_t MOV  =  0010000 + byte_mode + double_mode_src + double_src + double_mode_dest + double_dest; // Move source to destination
	uint16_t CMP  =  0020000 + byte_mode + double_mode_src + double_src + double_mode_dest + double_dest; // Compare source to destination
	uint16_t ADD  =  0060000 + double_mode_src + double_src + double_mode_dest + double_dest; // Add source to destination
	uint16_t SUB  =  0160000 + double_mode_src + double_src + double_mode_dest + double_dest; // Subtract source from destination
/*
	uint16_t CLRB  =  0105000
	uint16_t COMB  =  0105100
	uint16_t INCB  =  0105200
	uint16_t DECB  =  0105300
	uint16_t NEGB  =  0105400
	uint16_t TSTB  =  0105700
	uint16_t ASRB  =  0106200
	uint16_t ASLB  =  0106300
	uint16_t RORB  =  0106000
	uint16_t ROLB  =  0106100
	uint16_t ADCB  =  0105500
	uint16_t SBCB  =  0105600
	uint16_t BITB  =  0130000
	uint16_t BICB  =  0140000
	uint16_t BISB  =  0150000
	uint16_t MOVB  =  0110000
	uint16_t CMPB  =  0120000
*/	
	uint16_t WRTLCK = m_WRTLCK;
	uint16_t TSTSET = m_TSTSET;
	uint16_t XOR = m_XOR;
	uint16_t ASH = m_ASH;
	uint16_t ASHC = m_ASHC;
	uint16_t MUL = m_MUL;
	uint16_t DIV = m_DIV;
	uint16_t BR = m_BR;
	uint16_t BNE = m_BNE;
	uint16_t BEQ = m_BEQ;
	uint16_t BPL = m_BPL;
	uint16_t BMI = m_BMI;
	uint16_t BVC = m_BVC;
	uint16_t BVS = m_BVS;
	uint16_t BCC = m_BCC;
	uint16_t BCS = m_BCS;
	uint16_t BGE = m_BGE;
	uint16_t BLT = m_BLT;
	uint16_t BGT = m_BGT;
	uint16_t BLE = m_BLE;
	uint16_t SOB = m_SOB;
	uint16_t BHI = m_BHI;
	uint16_t BLOS = m_BLOS;
	uint16_t JMP = m_JMP;
	uint16_t JSR = m_JSR;
	uint16_t RTS = m_RTS;
	uint16_t MARK = m_MARK;
	uint16_t EMT = m_EMT;
	uint16_t TRAP = m_TRAP;
	uint16_t BPT = m_BPT;
	uint16_t IOT = m_IOT;
	uint16_t CSM = m_CSM;
	uint16_t RTI = m_RTI;
	uint16_t RTT = m_RTT;
	uint16_t HALT = m_HALT;
	uint16_t WAIT = m_WAIT;
	uint16_t RESET = m_RESET;
	uint16_t MTPD = m_MTPD;
	uint16_t MTPI = m_MTPI;
	uint16_t MFPD = m_MFPD;
	uint16_t MFPI = m_MFPI;
	uint16_t MTPS = m_MTPS;
	uint16_t MFPS = m_MFPS;
	uint16_t MFPT = m_MFPT;
	uint16_t SPL = m_SPL;	
	int res, test;
	
	
/*	
	int entries = 34;
	int end_byte_opable= 0;

	CLC = 0000241;
	CLV = 0000242;
	CLZ = 0000244;
	CLN = 0000250;
	CCC = 0000257;
	SEC = 0000261;
	SEV = 0000262;
	SEZ = 0000264;
	SEN = 0000270;
	SCC = 0000277;
	NOP = 0000240;
///*	
	uint16_t ops[entries] = {
							NOP,
							CLC, 
							CLV,
							0000243,
							CLZ,
							0000245,
							0000246,
							0000247,
							CLN,
							0000251,
							0000252,
							0000253,
							0000254,
							0000255,
							0000256,
							0000257,
							CCC,
							0000260,
							SEC, 
							SEV,
							0000263,
							SEZ,
							0000265,
							0000266,
							0000267,
							SEN,
							0000271,
							0000272,
							0000273,
							0000274,
							0000275,
							0000276,
							0000277,
							SCC
							};
							
	string ops_string[entries] = {
							"NOP",
							"CCC",
							"CCC",
							"CCC",
							"CCC",
							"CCC",
							"CCC",
							"CCC",
							"CCC",
							"CCC",
							"CCC",
							"CCC",
							"CCC",
							"CCC",
							"CCC",
							"CCC",
							"CCC",
							"NOP",
							"SCC",
							"SCC",
							"SCC",
							"SCC",
							"SCC",
							"SCC",
							"SCC",
							"SCC",
							"SCC",
							"SCC",
							"SCC",
							"SCC",
							"SCC",
							"SCC",
							"SCC",
							"SCC"
							};
*/
///*	
	int entries = 115;
	int end_byte_opable= 34;
	uint16_t ops[entries] = {
							CLR, //Start of byte-able operations
							CLR, 
							COM, 
							COM, 
							INC, 
							INC, 
							DEC, 
							DEC, 
							NEG, 
							NEG, 
							TST, 
							TST, 
							ASR, 
							ASR, 
							ASL, 
							ASL, 
							ROR, 
							ROR, 
							ROL, 
							ROL, 
							ADC, 
							ADC, 
							SBC, 
							SBC, 
							BIT, 
							BIT, 
							BIC, 
							BIC, 
							BIS, 
							BIS, 
							MOV, 
							MOV, 
							CMP, 
							CMP, //End of byte-able operations 
							SXT, 
							XOR, 
							ADD, 
							SUB, 
							SWAB, 
							TSTSET,
							WRTLCK,
							ASH, 
							ASHC, 
							MUL, 
							DIV, 
							BR, 
							BNE, 
							BEQ, 
							BPL, 
							BMI, 
							BVC, 
							BVS, 
							BCC, 
							BCS, 
							BGE, 
							BLT, 
							BGT, 
							BLE, 
							SOB, 
							BHI, 
							BLOS, 
							JMP, 
							JSR, 
							RTS, 
							MARK, 
							EMT, 
							TRAP, 
							BPT, 
							IOT, 
							CSM, 
							RTI, 
							RTT, 
							HALT, 
							WAIT, 
							RESET, 
							MTPD, 
							MTPI, 
							MFPD, 
							MFPI, 
							MTPS, 
							MFPS, 
							MFPT, 
							SPL, 
							m_CCCNOP ,// Condition codes
							m_CLC    ,
							m_CLV    ,
							m_CLVnC  ,
							m_CLZ    ,
							m_CLZnC  ,
							m_CLZnV  ,
							m_CLZnVnC,
							m_CLN    ,
							m_CLNnC  ,
							m_CLNnV  ,
							m_CLNnVnC,
							m_CLNnZ  ,
							m_CLNnZnC,
							m_CLNnZnV,
							m_CCC    ,
							m_SCCNOP ,
							m_SEC    ,
							m_SEV    ,
							m_SEVnC  ,
							m_SEZ    ,
							m_SEZnC  ,
							m_SEZnV  ,
							m_SEZnVnC,
							m_SEN    ,
							m_SENnC  ,
							m_SENnV  ,
							m_SENnVnC,
							m_SENnZ  ,
							m_SENnZnC,
							m_SENnZnV,
							m_SCC
							};
	
	string ops_string[entries] = {
									"CLR", 
									"CLRB", 
									"COM", 
									"COMB", 
									"INC", 
									"INCB", 
									"DEC", 
									"DECB", 
									"NEG", 
									"NEGB", 
									"TST", 
									"TSTB", 
									"ASR", 
									"ASRB", 
									"ASL", 
									"ASLB", 
									"ROR", 
									"RORB", 
									"ROL", 
									"ROLB", 
									"ADC", 
									"ADCB", 
									"SBC", 
									"SBCB", 
									"BIT", 
									"BITB", 
									"BIC", 
									"BICB", 
									"BIS", 
									"BISB", 
									"MOV", 
									"MOVB", 
									"CMP", 
									"CMPB", 
									"SXT", 
									"XOR", 
									"ADD", 
									"SUB", 
									"SWAB", 
									"TSTSET",
									"WRTLCK",
									"ASH", 
									"ASHC", 
									"MUL", 
									"DIV", 
									"BR", 
									"BNE", 
									"BEQ", 
									"BPL", 
									"BMI", 
									"BVC", 
									"BVS", 
									"BCC", 
									"BCS", 
									"BGE", 
									"BLT", 
									"BGT", 
									"BLE", 
									"SOB", 
									"BHI", 
									"BLOS", 
									"JMP", 
									"JSR", 
									"RTS", 
									"MARK", 
									"EMT", 
									"TRAP", 
									"BPT", 
									"IOT", 
									"CSM", 
									"RTI", 
									"RTT", 
									"HALT", 
									"WAIT", 
									"RESET", 
									"MTPD", 
									"MTPI", 
									"MFPD", 
									"MFPI", 
									"MTPS", 
									"MFPS", 
									"MFPT", 
									"SPL", 
									"NOP"        ,
									"CLC"        ,
									"CLV"        ,
									"CLV|CLC"    ,
									"CLZ"        ,
									"CLZ|CLC"    ,
									"CLZ|CLV"    ,
									"CLZ|CLV|CLC",
									"CLN"        ,
									"CLN|CLC"    ,
									"CLN|CLV"    ,
									"CLN|CLV|CLC",
									"CLN|CLZ"    ,
									"CLN|CLZ|CLC",
									"CLN|CLZ|CLV",
									"CCC"        ,
									"NOP"        ,
									"SEC"        ,
									"SEV"        ,
									"SEV|SEC"    ,
									"SEZ"        ,
									"SEZ|SEC"    ,
									"SEZ|SEV"    ,
									"SEZ|SEV|SEC",
									"SEN"        ,
									"SEN|SEC"    ,
									"SEN|SEV"    ,
									"SEN|SEV|SEC",
									"SEN|SEZ"    ,
									"SEN|SEZ|SEC",
									"SEN|SEZ|SEV",
									"SCC"
										};
//*/
/*
	//Just testing the problem children
	int entries = 49;
	int end_byte_opable= 5;
	uint16_t ops[entries] = {	BIT, 
								BIC, 
								BIS, 
								MOV, 
								CMP, 
								TSTSET,
								WRTLCK,
								SXT, 
								XOR, 
								SUB, 
								SWAB, 
								ASH, 
								ASHC, 
								MUL, 
								DIV, 
								BPL, 
								BMI, 
								BVC, 
								BVS, 
								BCC, 
								BCS, 
								SOB, 
								BHI, 
								BLOS, 
								EMT, 
								TRAP, 
								BPT, 
								IOT, 
								RTI, 
								RTT, 
								HALT, 
								WAIT, 
								RESET, 
								MTPD, 
								MFPD, 
								MTPS, 
								MFPS, 
								MFPT, 
								SPL, 
								CLC, 
								CLV, 
								CLZ, 
								CLN, 
								CCC, 
								SEC, 
								SEV, 
								SEZ, 
								SEN, 
								SCC}; 
	string ops_string[entries] = {	"BITB", 
									"BICB", 
									"BISB", 
									"MOVB", 
									"CMPB", 
									"TSTSET",
									"WRTLCK",
									"SXT", 
									"XOR", 
									"SUB", 
									"SWAB", 
									"ASH", 
									"ASHC", 
									"MUL", 
									"DIV", 
									"BPL", 
									"BMI", 
									"BVC", 
									"BVS", 
									"BCC", 
									"BCS", 
									"SOB", 
									"BHI", 
									"BLOS", 
									"EMT", 
									"TRAP", 
									"BPT", 
									"IOT", 
									"RTI", 
									"RTT", 
									"HALT", 
									"WAIT", 
									"RESET", 
									"MTPD", 
									"MFPD", 
									"MTPS", 
									"MFPS", 
									"MFPT", 
									"SPL", 
									"CLC", 
									"CLV", 
									"CLZ", 
									"CLN", 
									"CCC", 
									"SEC", 
									"SEV", 
									"SEZ", 
									"SEN", 
									"SCC"}; */
	int runs = entries;
	uint16_t temp_op = 0;
	for (int i = 0; i < runs; i++)
	{
		cout << "_________________\n" <<endl;
		cout << "Testing " <<ops_string[i] << endl;
		temp_op = ops[i];
		if((i <= end_byte_opable) && ((i%2) == 1)) { // If this is a byte instruction
			cout << "Byte Instruction" << endl;
			temp_op|=0x8000;
		}
		cout << "Raw OpCode: "<< setfill('0') << setw(7) << oct << temp_op << endl;
		res = parseInstruction(temp_op, current_instruction);
		cout << "Pretty-Print function generated: "<< op_formatted(current_instruction) << endl;
		test = dispatch(current_instruction);
		cout << "Actually Ran : "<< current_instruction->op_text << endl;
		clearReg();
		printInstruction(current_instruction);
		clearInstruction(current_instruction);
		temp_op = 0;
		cout << "\n_________________" << endl;
	}
}

int main()
{
	int result = 0;

	result = operationTest();
	return 1;
}
