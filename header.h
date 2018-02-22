// Functions and defines go here
// Function prototypes with a (B) have Byte operations as well

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>

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

int HALT(); // Halt
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
int CCC(); // Clear all
int SEC(); // Set carry
int SEV(); // Set overflow
int SEZ(); // Set zero
int SEN(); // Set negative
int SCC(); // Set all


// Parsing functions
/*
struct instruction {
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
};

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
*/
// Opcode values

// SINGLE OPERAND

// General
#define CLR "005000" // Clear
#define CLRB "105000" // Clear (B)
#define COM "005100" // 1's Compliment
#define COMB "105100" // 1's Compliment (B)
#define INC "005200" // Increment
#define INCB "105200" // Increment (B)
#define DEC "005300" // Decrement
#define DECB "105300" // Decrement (B)
#define NEG "005400" // 2's Compliment negate
#define NEGB "105400" // 2's Compliment negate (B)
#define NOP // No operation
#define TST "005700" // Test
#define TSTB "105700" // Test (B)
#define TSTSET // Test, set low bit
#define WRTLCK // Read/Lock destination, write/unlock R0 #defineo destination

// Shift & Rotate
#define ASR "006200" // Arithmetic shift right
#define ASRB "106200" // Arithmetic shift right (B)
#define ASL "006300" // Arithmetic shift left
#define ASLB "106300" // Arithmetic shift left (B)
#define ROR "006000" // Rotate right
#define RORB "106000" // Rotate right (B)
#define ROL "006100" // Rotate left
#define ROLB "106100" // Rotate left (B)
#define SWAB "000300" // Swap bytes

// Multiple Precision
#define ADC "005500" // Add carry
#define ADCB "105500" // Add carry (B)
#define SBC "005600" // Subtract carry
#define SBCB "105600" // Subtract carry (B)
#define SXT "006700" // Sign extend

// DOUBLE OPERAND

// General
#define MOV "010000" // Move source to destination
#define MOVB "110000" // Move source to destination (B)
#define ADD "060000" // Add source to destination
#define SUB "160000" // Subtract source from destination
#define CMP "020000" // Compare source to destination
#define CMPB "120000" // Compare source to destination (B)
#define ASH "072000" // Shift arithmetically
#define ASHC "073000"// Arithmetic shift combined
#define MUL "070000" // Multiply
#define DIV "071000" // Divide

// Logical
#define BIT "030000" // Bit test
#define BITB "130000" // Bit test (B)
#define BIC "040000" // Bit clear
#define BICB "140000" // Bit clear (B)
#define BIS "050000" // Bit set
#define BISB "150000" // Bit set (B)
#define XOR "074000" // Exclusive OR

// BRANCH

// Branch
#define BR "000400" // Branch unconditional
#define BNE "001000" // Branch if not equal (to zero)
#define BEQ "001400" // Branch if equal (to zero)
#define BPL "100000" // Branch if plus
#define BMI "100400" // Branch if minus
#define BVC "102000" // Branch if overflow is clear
#define BVS "102400" // Branch if overflow is set
#define BCC "103000" // Branch if carry is clear
#define BCS "103400" // Branch if carry is set

// Signed Conditional Branch
#define BGE "002000" // Branch if greater than or equal (to zero)
#define BLT "002400" // Branch if less than (zero)
#define BGT "003000" // Branch if greater than (zero)
#define BLE "003400" // Branch if less than or equal (to zero)
#define SOB "077000" // Subtract one and branch (if not = 0)

// Unsigned Conditional Branch
#define BHI "101000" // Branch if higher
#define BLOS "101400" // Branch if lower or same
#define BHIS "103000" // Branch if higher or same (same as BCC)
#define BLO "103400" // Branch if lower (same as BCS)

// JUMP & SUBROUTINE

#define JMP "000100" // Jump
#define JSR "004000" // Jump to subroutine
#define RTS "000200" // Return from subroutine
#define MARK "006400" // Facilitates stack clean-up procedures

// TRAPS & INTERRUPTS

#define EMT "104000" // Emulator trap
#define TRAP "104400" // Trap
#define BPT // Breakpoint trap
#define IOT "000004" // Input/Output trap
#define CSM // Call to supervisor mode
#define RTI "000002" // Return from interrupt
#define RTT // Return from trap???

// MISC

#define HALT "000000" // Halt
#define WAIT "000001" // Wait for interrupt
#define RESET "000005" // Reset UNIBUS
#define MTPD "106600" // Move to previous data space
#define MTPI "006600" // Move to previous instruction space
#define MFPD "106500" // Move from previous data space
#define MFPI "006500" // Move from previous instruction space
#define MTPS "106400" // Move byte to processor status word
#define MFPS "106700" // Move byte from processor status word
#define MFPT // Move from processor type

// CONDITION CODES

#define CLC "000241" // Clear carry
#define CLV "000242" // Clear overflow
#define CLZ "000244" // Clear zero
#define CLN "000250" // Clear negative
#define CCC "000257" // Clear all
#define SEC "000261" // Set carry
#define SEV "000262" // Set overflow
#define SEZ "000264" // Set zero
#define SEN "000270" // Set negative
#define SCC "000277" // Set all
