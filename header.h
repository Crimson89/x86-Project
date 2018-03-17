#ifndef HEADER_H
#define HEADER_H
// Functions and defines go here
// Function prototypes with a (B) have Byte operations as well

#include <iostream> //io
#include <fstream>  //file reading
#include <iomanip>  // Octal formatting
#include <string>   //Strings
#include <string.h> // memcpy
#include <sstream>  //String stream buffer
#include <math.h>   //log2
#include <unistd.h> //Getopt
#include <vector>   //vectors
#include <stdint.h> //64 bit ints
#include <limits>   // Numerical limits
#include "defines.h"

using namespace std;

#pragma pack(push, 1)
typedef union {
	struct {
		// In reverse order so that the bits fall in the correct order for printing the word
		uint8_t C:1;
		uint8_t V:1;
		uint8_t Z:1;
		uint8_t N:1;
		uint8_t T:1;
		uint8_t SPL:3;
	};
	uint8_t PSW_BYTE;
}PSW_t;

typedef struct {
	uint16_t opcode; // instruction opcode 
	uint16_t byteMode;  // If it's a byte instruction
	uint16_t addressingModeSrc; // Source addressing mode, for double ops.
	uint16_t addressingModeDest; // Destination addressing mode, for double ops.
	uint16_t addressingModeReg;  // Register addressing mode for single ops.
	uint16_t srcBase;	// src register
	uint16_t destBase;	// dest register
	uint16_t regBase;	// reg register (lol)

	int16_t offset;   // Signed offset
	int16_t immediate; // Signed immediate value, used for debug pretty-print
	uint16_t rtsReg; // Specific register for RTS instruction
	uint8_t padding; // Make this struct an even multiple of 64 bits up to the end of this struct
	union {
		struct {
			// In reverse order so that the bits fall in the correct order for printing the word
			uint8_t C:1;
			uint8_t V:1;
			uint8_t Z:1;
			uint8_t N:1;
			uint8_t T:1;
			uint8_t SPL:3;
		};
		uint8_t PSW;            // Processor Status Word for this operation, written to global PSW at writeback
	};

	bool is_branch;         // This instruction is a branch
	bool branch_taken;      // Was this branch taken or not
	uint16_t branch_target; // Destination of branch
	string op_text;         // instruction name used for pretty print
}instruction;
#pragma pack(pop)

//Define REGS, MEM, and  globals
extern uint16_t REGS[7];
extern uint8_t MEM[MEMORY_SPACE];
extern bool MEM_USED_FLAGS[MEMORY_SPACE];
extern bool BREAK_POINT[MEMORY_SPACE/2];
extern uint16_t& R0;
extern uint16_t& R1;
extern uint16_t& R2;
extern uint16_t& R3;
extern uint16_t& R4;
extern uint16_t& R5;
extern uint16_t& R6;
extern uint16_t& R7;
extern uint16_t& SP;
extern uint16_t& PC;
extern uint16_t starting_pc;
extern instruction * current_instruction;	 // decoded instruction information
extern int verbosity_level;                  // Level of verbosity in print statements, 1 prints little, 2 prints some debugs, 3 prints all debugs
extern string trace_file;
extern string data_file;
extern string branch_trace_file;
extern PSW_t PSW;


// MAIN functions - main.cpp
int menu_function(bool & bp_print_mem, bool & bp_print_regs, string & trace_file, bool branch_trace_en);
void get_user_octal(string prompt, string error_text, uint16_t &word);
int instruction_fetch(bool & at_breakpoint, uint16_t & instruction_code,  uint16_t & breakpoint_pc, uint16_t & PC, uint16_t & if_pc_falue);
int write_back(PSW_t & PSW, instruction * inst);
void clear_psw(PSW_t & PSW);
void print_psw(PSW_t & PSW);

// Memory functions - memory.cpp
int get_cmd_options(int argc, char ** argv, string & branch_trace_file, string & data_file, string & trace_file);
int readData(string & data_file);
uint16_t string_to_octal(string input_string);
string octal_to_string(uint16_t value);
uint16_t read_byte(uint16_t addressMode, uint16_t address, bool trace = true);                              //Read a byte and return it in the low 8 bits
uint16_t read_word(uint16_t addressMode, uint16_t address, bool trace = true, bool is_instruction = false); //Read two bytes in memory and return as little endian word
void write_byte(uint16_t addressMode, uint16_t address, uint16_t byte, bool trace = true);                  //Take byte in the low 8 bits and write to memory
void write_word(uint16_t addressMode, uint16_t address, uint16_t word, bool trace = true);                  //Write little endian word to two bytes in memory
void print_octal(uint16_t value);
void print_all_memory(void);
void print_all_registers(void);
void initializeMemory(void);
int set_breakpoint(uint16_t address);
int clear_all_breakpoints(void);
int clear_breakpoint(uint16_t address);
bool check_breakpoint(uint16_t address);
bool handle_breakpoint(uint16_t address, uint16_t instruction_code, bool old_program_step_mode, bool & bp_print_mem, bool & bp_print_regs);
void print_bp_config(bool & bp_print_mem, bool & bp_print_regs);
void print_all_breakpoints(bool & bp_print_mem, bool & bp_print_regs);
string get_op_name(void);
string format_arg(uint8_t reg, uint8_t mode, uint16_t immediate);
string op_formatted(instruction * op);

//Function dispatcher - dispatch.cpp
int dispatch(instruction *inst);


// Trace Functions - trace.cpp
int clear_trace(void);
int print_trace(void);
int read_trace(uint16_t address, uint16_t value, bool is_instruction = false);
int write_trace(uint16_t address, uint16_t value);
int write_line(string text, string filename);
int clear_branch_trace();
int print_branch_trace();
int branch_trace(uint16_t PC, uint16_t target, string name, bool taken);

// Parse functions - parse.cpp
int parseInstruction(uint16_t instructionCode, instruction* newInstruction);
uint16_t get_address(uint16_t mode, uint16_t baseAddress, bool trace = true);
uint16_t get_value(uint16_t mode, uint16_t baseAddress, bool trace = true);

int printInstruction(instruction* newInstruction);
int clearInstruction(instruction* newInstruction);


// SINGLE OPERAND

// General
int CLR(instruction *inst); // Clear (B)
int COM(instruction *inst); // 1's Compliment (B)
int INC(instruction *inst); // Increment (B)
int DEC(instruction *inst); // Decrement (B)
int NEG(instruction *inst); // 2's Compliment negate (B)
int TST(instruction *inst); // Test (B)

// Shift & Rotate
int ASR(instruction *inst); // Arithmetic shift right (B)
int ASL(instruction *inst); // Arithmetic shift left (B)
int ROR(instruction *inst); // Rotate right (B)
int ROL(instruction *inst); // Rotate left (B)
int SWAB(instruction *inst); // Swap bytes

// Multiple Precision
int ADC(instruction *inst); // Add carry (B)
int SBC(instruction *inst); // Subtract carry (B)
int SXT(instruction *inst); // Sign extend

// DOUBLE OPERAND

// General
int MOV(instruction *inst); // Move source to destination (B)
int ADD(instruction *inst); // Add source to destination
int SUB(instruction *inst); // Subtract source from destination
int CMP(instruction *inst); // Compare source to destination (B)

// Logical
int BIT(instruction *inst); // Bit test (B)
int BIC(instruction *inst); // Bit clear (B)
int BIS(instruction *inst); // Bit set (B)

// BRANCH

// Branch
int BR(instruction *inst); // Branch unconditional
int BNE(instruction *inst); // Branch if not equal (to zero)
int BEQ(instruction *inst); // Branch if equal (to zero)
int BPL(instruction *inst); // Branch if plus
int BMI(instruction *inst); // Branch if minus
int BVC(instruction *inst); // Branch if overflow is clear
int BVS(instruction *inst); // Branch if overflow is set
int BCC(instruction *inst); // Branch if carry is clear
int BCS(instruction *inst); // Branch if carry is set

// Signed Conditional Branch
int BGE(instruction *inst); // Branch if greater than or equal (to zero)
int BLT(instruction *inst); // Branch if less than (zero)
int BGT(instruction *inst); // Branch if greater than (zero)
int BLE(instruction *inst); // Branch if less than or equal (to zero)

// Unsigned Conditional Branch
int BHI(instruction *inst); // Branch if higher
int BLOS(instruction *inst); // Branch if lower or same
int BHIS(instruction *inst); // Branch if higher or same
int BLO(instruction *inst); // Branch if lower

// JUMP & SUBROUTINE

int JMP(instruction *inst); // Jump
int JSR(instruction *inst); // Jump to subroutine
int RST(instruction *inst); // Return from subroutine

// TRAPS & INTERRUPTS

int EMT(instruction *inst); // Emulator trap

// CONDITION CODES

int SPL(instruction *inst); // Set priority level
int CCC(instruction *inst); // Clear Condition Codes
int SCC(instruction *inst); // Set Condition Codes


//Temporary dummy functions
int XOR(instruction *inst);
int ASH(instruction *inst);
int ASHC(instruction *inst);
int MUL(instruction *inst);
int DIV(instruction *inst);
int NEQ(instruction *inst);
int SOB(instruction *inst);
int RTS(instruction *inst);
int MARK(instruction *inst);
int HALT(instruction *inst);
int WAIT(instruction *inst);
int TRAP(instruction *inst);
int BPT(instruction *inst);
int IOT(instruction *inst);
int CSM(instruction *inst);
int RTI(instruction *inst);
int RTT(instruction *inst);
int RESET(instruction *inst);
int MTPD(instruction *inst);
int MTPI(instruction *inst);
int MFPD(instruction *inst);
int MFPI(instruction *inst);
int MTPS(instruction *inst);
int MFPS(instruction *inst);
int MFPT(instruction *inst);
int TSTSET(instruction *inst);
int WRTLCK(instruction *inst);


// --PARSE--
// Masks used for parsing instructions and pulling out relevant data from them
const uint16_t maskByteMode = 0100000; 
 
const uint16_t maskRelevantBits = 0177777;  
const uint16_t maskSingleCondBranchCondCheck = 0070000; // Else double or double reg
const uint16_t maskSingle = 0074000; // Else conditional
const uint16_t maskCondCheck = 0077760; // Else conditional branch  
const uint16_t maskRegSource = 0070000; // Else double operand  

// Assignment masks, these facilitate the extraction of relevant data for instruction families.
// Double-operand
const uint16_t maskDoubleOpcode = 0070000;
const uint16_t maskDoubleSourceMode = 0007000;
const uint16_t maskDoubleSource = 0000700;
const uint16_t maskDoubleDestMode = 0000070;
const uint16_t maskDoubleDest = 0000007;

// Double-operand register
const uint16_t maskDoubleRegisterOpcode = 0177000;
const uint16_t maskDoubleRegisterReg = 0000300;
const uint16_t maskDoubleRegisterSourceDestMode = 0000070;
const uint16_t maskDoubleRegisterSourceDest = 0000007;

// Single-operand
const uint16_t maskSingleOpcode = 0077700;
const uint16_t maskSingleMode = 0000070;
const uint16_t maskSingleRegister = 0000007;

// Conditional branch
const uint16_t maskCondBranchOpcode = 0177400;
const uint16_t maskCondBranchOffset = 0000377;

// Conditional check
const uint16_t maskCondCodeOpcode = 0177760;
const uint16_t maskCondSC = 0000020;
const uint16_t maskCondN = 0000010;
const uint16_t maskCondZ = 0000004;
const uint16_t maskCondV = 0000002;
const uint16_t maskCondC = 0000001;

#endif // HEADER_H
