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


// SINGLE OPERAND

// General
int CLR(); // Clear (B)
int COM(); // 1's Compliment (B)
int INC(); // Increment (B)
int DEC(); // Decrement (B)
int NEG(); // 2's Compliment negate (B)
int NOP(); // No operation
int TST(); // Test (B)
int TSTSET(); // Test, set low bit
int WRTLCK(); // Read/Lock destination, write/unlock R0 into destination

// Shift & Rotate
int ASR(); // Arithmetic shift right (B)
int ASL(); // Arithmetic shift left (B)
int ROR(); // Rotate right (B)
int ROL(); // Rotate left (B)
int SWAB(); // Swap bytes

// Multiple Precision
int ADC(); // Add carry (B)
int SBC(); // Subtract carry (B)
int SXT(); // Sign extend

// DOUBLE OPERAND

// General
int MOV(); // Move source to destination (B)
int ADD(); // Add source to destination
int SUB(); // Subtract source from destination
int CMP(); // Compare source to destination (B)
int ASH(); // Shift arithmetically
int ASHC(); // Arithmetic shift combined
int MUL(); // Multiply
int DIV(); // Divide

// Logical
int BIT(); // Bit test (B)
int BIC(); // Bit clear (B)
int BIS(); // Bit set (B)
int XOR(); // Exclusive OR

// BRANCH

// Branch
int BR(); // Branch unconditional
int BNE(); // Branch if not equal (to zero)
int BEQ(); // Branch if equal (to zero)
int BPL(); // Branch if plus
int BMI(); // Branch if minus
int BVC(); // Branch if overflow is clear
int BVS(); // Branch if overflow is set
int BCC(); // Branch if carry is clear
int BCS(); // Branch if carry is set

// Signed Conditional Branch
int BGE(); // Branch if greater than or equal (to zero)
int BLT(); // Branch if less than (zero)
int BGT(); // Branch if greater than (zero)
int BLE(); // Branch if less than or equal (to zero)
int SOB(); // Subtract one and branch (if not = 0)

// Unsigned Conditional Branch
int BHI(); // Branch if higher
int BLOS(); // Branch if lower or same
int BHIS(); // Branch if higher or same
int BLO(); // Branch if lower

// JUMP & SUBROUTINE

int JMP(); // Jump
int JSR(); // Jump to subroutine
int RTS(); // Return from subroutine
int MARK(); // Facilitates stack clean-up procedures

// TRAPS & INTERRUPTS

int EMT(); // Emulator trap
int TRAP(); // Trap
int BPT(); // Breakpoint trap
int IOT(); // Input/Output trap
int CSM(); // Call to supervisor mode
int RTI(); // Return from interrupt
int RTT(); // Return from trap???

// MISC

//int HALT(); // Halt -- Nothing to do, handled in program loop - Exists loop
int WAIT(); // Wait for interrupt
int RESET(); // Reset UNIBUS
int MTPD(); // Move to previous data space
int MTPI(); // Move to previous instruction space
int MFPD(); // Move from previous data space
int MFPI(); // Move from previous instruction space
int MTPS(); // Move byte to processor status word
int MFPS(); // Move byte from processor status word
int MFPT(); // Move from processor type

// CONDITION CODES

int CLC(); // Clear carry
int CLV(); // Clear overflow
int CLZ(); // Clear zero
int CLN(); // Clear negative
int CCC(); // Clear Condition Codes
int SEC(); // Set carry
int SEV(); // Set overflow
int SEZ(); // Set zero
int SEN(); // Set negative
int SCC(); // Set Condition Codes


// Parsing functions

typedef struct {
  u16_t opCode;
  enum byteMode;
  enum instructionType; //Double operand, single operand, conditional jump, shit like that.
  enum instructionFamily; // J, I, R?
  enum addressingModeSrc;
  enum addressingModeDest;
  int reg;
  int src;
  int dest;
  int offset;
  enum set;
  int N;
  int Z;
  int V;
  int C;
}instruction;

// BETTER NAMES MAX
const unsigned char maskRelevantBits = 0x78A0;
const unsigned char maskSingleCondBranchCondCheck = 0x7000;
const unsigned char maskSingle = 0x0800;
const unsigned char maskCondCheck = 0x00E0; // Else conditional branch
const unsigned char maskRegSource = 0x7000; // Else double operand

// Assignment masks
// Double-operand
const unsigned char maskDoubleOpcode = 0x7000;
const unsigned char maskDoubleSourceMode = 0x0E00;
const unsigned char maskDoubleSource = 0x01C0;
const unsigned char maskDoubleDestMode = 0x0038;
const unsigned char maskDoubleDest = 0x0007;

// Double-operand register
const unsigned char maskDoubleRegisterOpcode = 0x0700;
const unsigned char maskDoubleRegister = 0x00C0;
const unsigned char maskDoubleRegisterSourceDestMode = 0x0038;
const unsigned char maskDoubleRegisterSourceDest = 0x0007;

// Single-operand
const unsigned char maskSingleOpcode = 0x0730;
const unsigned char maskSingleMode = 0x0038;
const unsigned char maskSingleRegister = 0x0007;

// Conditional branch
const unsigned char maskCondBranchOpcode = 0x0700;
const unsigned char maskCondBranchOffset = 0x00FF;

// Conditional check
const unsigned char maskCondCodeOpcode = 0xFFE0;
const unsigned char maskCondsc = 0x0010;
const unsigned char maskCondN = 0x0008;
const unsigned char maskCondZ = 0x0004;
const unsigned char maskCondV = 0x0002;
const unsigned char maskCondC = 0x0001;

// Byte mask
const unsigned char maskByteInstruction = 0x8000;


instruction parseType(u16_t opCode);