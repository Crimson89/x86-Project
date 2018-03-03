// Functions and defines go here
// Function prototypes with a (B) have Byte operations as well

#include <iostream> //io
#include <fstream>  //file reading
#include <string>   //Strings
#include <math.h>   //log2
#include <unistd.h> //Getopt
#include <vector>   //vectors
#include <stdint.h> //64 bit ints
#include <iomanip>  // Octal formatting
#include <limits>   // Numerical limits
#include "defines.h"

using namespace std;

//instructionType instructionFamily {singleOperand, doubleOperand}; // (? Probably not necessary) 
 
typedef struct {  
  uint16_t opcode;  
  uint16_t byteMode;  
  //instructionFamily instructionType ; //Double operand, single operand, conditional jump, shit like that.  
  //enum instructionFamily; // J, I, R? Not in PDP right?  
	uint8_t addressingModeSrc;  
	uint8_t addressingModeDest;  
	uint8_t srcReg;	// src pre-dereferencing
	uint8_t destReg;// dest pre-dereferencing
	uint16_t *reg; 
	uint16_t *src; // src post-dereferencing
	uint16_t *dest;// dest post-dereferencing
	uint16_t offset;
	uint16_t immediate; 
	uint16_t registerMode;  
	union {
		struct {
			int SC: 3;
			int N: 1;
			int Z: 1;
			int V: 1;
			int C: 1;
		};
		uint8_t PSW; // Processor Status Word
	};
  uint16_t rtsR; // placeholder till I figure this out.
}instruction;  

// MAIN functions
int loadData(int argc, char ** argv);
int readData(string fileName);
int menu_function(void);
uint16_t string_to_operation(string input_string);
uint16_t string_to_address(string input_string);
uint16_t address_to_index(uint16_t this_address);
uint16_t index_to_address(uint16_t this_index);
void print_operation(uint16_t operation);
void print_address(uint16_t address);
void print_address_at_index(int index);
void print_all_memory(void);
void print_all_registers(void);
void initializeMemory(void);

// Utility functions
int loadOperands();
int updateTracefile(bool write, uint16_t address);


// SINGLE OPERAND

// General
int CLR(instruction *inst); // Clear (B)
int COM(instruction *inst); // 1's Compliment (B)
int INC(instruction *inst); // Increment (B)
int DEC(instruction *inst); // Decrement (B)
int NEG(instruction *inst); // 2's Compliment negate (B)
int NOP(instruction *inst); // No operation
int TST(instruction *inst); // Test (B)
int TSTSET(instruction *inst); // Test, set low bit
int WRTLCK(instruction *inst); // Read/Lock destination, write/unlock R0 into destination

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
int ASH(instruction *inst); // Shift arithmetically
int ASHC(instruction *inst); // Arithmetic shift combined
int MUL(instruction *inst); // Multiply
int DIV(instruction *inst); // Divide

// Logical
int BIT(instruction *inst); // Bit test (B)
int BIC(instruction *inst); // Bit clear (B)
int BIS(instruction *inst); // Bit set (B)
int XOR(instruction *inst); // Exclusive OR

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
int SOB(instruction *inst); // Subtract one and branch (if not = 0)

// Unsigned Conditional Branch
int BHI(instruction *inst); // Branch if higher
int BLOS(instruction *inst); // Branch if lower or same
int BHIS(instruction *inst); // Branch if higher or same
int BLO(instruction *inst); // Branch if lower

// JUMP & SUBROUTINE

int JMP(instruction *inst); // Jump
int JSR(instruction *inst); // Jump to subroutine
int RTS(instruction *inst); // Return from subroutine
int MARK(instruction *inst); // Facilitates stack clean-up procedures

// TRAPS & INTERRUPTS

int EMT(instruction *inst); // Emulator trap
int TRAP(instruction *inst); // Trap
int BPT(instruction *inst); // Breakpoint trap
int IOT(instruction *inst); // Input/Output trap
int CSM(instruction *inst); // Call to supervisor mode
int RTI(instruction *inst); // Return from interrupt
int RTT(instruction *inst); // Return from trap???

// MISC

//int HALT(instruction *inst); // Halt -- Nothing to do, handled in program loop - Exists loop
int WAIT(instruction *inst); // Wait for interrupt
int RESET(instruction *inst); // Reset UNIBUS
int MTPD(instruction *inst); // Move to previous data space
int MTPI(instruction *inst); // Move to previous instruction space
int MFPD(instruction *inst); // Move from previous data space
int MFPI(instruction *inst); // Move from previous instruction space
int MTPS(instruction *inst); // Move byte to processor status word
int MFPS(instruction *inst); // Move byte from processor status word
int MFPT(instruction *inst); // Move from processor type

// CONDITION CODES

int CLC(instruction *inst); // Clear carry
int CLV(instruction *inst); // Clear overflow
int CLZ(instruction *inst); // Clear zero
int CLN(instruction *inst); // Clear negative
int CCC(instruction *inst); // Clear Condition Codes
int SEC(instruction *inst); // Set carry
int SEV(instruction *inst); // Set overflow
int SEZ(instruction *inst); // Set zero
int SEN(instruction *inst); // Set negative
int SCC(instruction *inst); // Set Condition Codes


// Parsing functions

const uint16_t maskByteMode = 0100000;//0x8000; 
 
// BETTER NAMES MAX TODO double check these masks to see if they're doing what I want them to do.  
const uint16_t maskRelevantBits = 0177777;//0x78A0;  
//const uint16_t maskRelevantBits2 = 0070000;
const uint16_t maskSingleCondBranchCondCheck = 0070000;//0x7000; // (Maybe 0x7800)  
const uint16_t maskSingle = 0074000;//0x0800; // (Maybe 0x7800) 
const uint16_t maskCondCheck = 0077760;//0x00E0; // (Maybe 0xFF80) Else conditional branch  
const uint16_t maskRegSource = 0070000;// 0x7000; // (Maybe 0x8000) Else double operand  

// Assignment masks
// Double-operand
const uint16_t maskDoubleOpcode = 0070000;//0x7000;
const uint16_t maskDoubleSourceMode = 0007000;//0x0E00;
const uint16_t maskDoubleSource = 0000700;//0x01C0;
const uint16_t maskDoubleDestMode = 0000070;//0x0038;
const uint16_t maskDoubleDest = 0000007;//0x0007;

// Double-operand register
const uint16_t maskDoubleRegisterOpcode = 0003400;// 0x0700;
const uint16_t maskDoubleRegisterReg = 0000300;//0x00C0;
const uint16_t maskDoubleRegisterSourceDestMode = 0000070;//0x0038;
const uint16_t maskDoubleRegisterSourceDest = 0000007;//0x0007;

// Single-operand
const uint16_t maskSingleOpcode = 0003700;//0x0730;
const uint16_t maskSingleMode = 0000070;//0x0038;
const uint16_t maskSingleRegister = 0000007;//0x0007;

// Conditional branch
const uint16_t maskCondBranchOpcode = 0003400;//0x0700;
const uint16_t maskCondBranchOffset = 0000377;//0x00FF;

// Conditional check
const uint16_t maskCondCodeOpcode = 0177740;//0xFFE0;
const uint16_t maskCondSC = 0000020;//0x0010;
const uint16_t maskCondN = 0000010;//0x0008;
const uint16_t maskCondZ = 0000004;//0x0004;
const uint16_t maskCondV = 0000002;//0x0002;
const uint16_t maskCondC = 0000001;//0x0001;

int parseInstruction(uint16_t instructionCode, instruction* newInstruction);

int parseTest();
