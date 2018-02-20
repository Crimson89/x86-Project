// Functions and defines go here
// Function prototypes with a (B) have Byte operations as well

#include <stdio.h>

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
int CCC(); // ???
int SEC(); // Set carry
int SEV(); // Set overflow
int SEZ(); // Set zero
int SEN(); // Set negative
int SCC(); // ???
