#define SILENT_MODE 1
#define VERBOSE_MODE 2
#define MEMORY_SPACE ( (1<<15) - (1<<12) )
#define HALT 0x000000
#define R0_REG_INDEX 0
#define R1_REG_INDEX 1
#define R2_REG_INDEX 2
#define R3_REG_INDEX 3
#define R4_REG_INDEX 4
#define R5_REG_INDEX 5
#define SP_REG_INDEX 6
#define PC_REG_INDEX 7
#define PRINT_MENU  0
#define RUN_PROGRAM 1
#define LOAD_DATA   2

// Opcode values

// SINGLE OPERAND

// General
#define CLR    "005000" // Clear
#define CLRB   "105000" // Clear (B)
#define COM    "005100" // 1's Compliment
#define COMB   "105100" // 1's Compliment (B)
#define INC    "005200" // Increment
#define INCB   "105200" // Increment (B)
#define DEC    "005300" // Decrement
#define DECB   "105300" // Decrement (B)
#define NEG    "005400" // 2's Compliment negate
#define NEGB   "105400" // 2's Compliment negate (B)
#define NOP    "000240" // No operation
#define TST    "005700" // Test
#define TSTB   "105700" // Test (B)
#define TSTSET // Test, set low bit
#define WRTLCK // Read/Lock destination, write/unlock R0 #defineo destination

// Shift & Rotate
#define ASR    "006200" // Arithmetic shift right
#define ASRB   "106200" // Arithmetic shift right (B)
#define ASL    "006300" // Arithmetic shift left
#define ASLB   "106300" // Arithmetic shift left (B)
#define ROR    "006000" // Rotate right
#define RORB   "106000" // Rotate right (B)
#define ROL    "006100" // Rotate left
#define ROLB   "106100" // Rotate left (B)
#define SWAB   "000300" // Swap bytes

// Multiple Precision
#define ADC    "005500" // Add carry
#define ADCB   "105500" // Add carry (B)
#define SBC    "005600" // Subtract carry
#define SBCB   "105600" // Subtract carry (B)
#define SXT    "006700" // Sign extend

// DOUBLE OPERAND

// General
#define MOV    "010000" // Move source to destination
#define MOVB   "110000" // Move source to destination (B)
#define ADD    "060000" // Add source to destination
#define SUB    "160000" // Subtract source from destination
#define CMP    "020000" // Compare source to destination
#define CMPB   "120000" // Compare source to destination (B)
#define ASH    "072000" // Shift arithmetically
#define ASHC   "073000"// Arithmetic shift combined
#define MUL    "070000" // Multiply
#define DIV    "071000" // Divide

// Logical
#define BIT    "030000" // Bit test
#define BITB   "130000" // Bit test (B)
#define BIC    "040000" // Bit clear
#define BICB   "140000" // Bit clear (B)
#define BIS    "050000" // Bit set
#define BISB   "150000" // Bit set (B)
#define XOR    "074000" // Exclusive OR

// BRANCH

// Branch
#define BR     "000400" // Branch unconditional
#define BNE    "001000" // Branch if not equal (to zero)
#define BEQ    "001400" // Branch if equal (to zero)
#define BPL    "100000" // Branch if plus
#define BMI    "100400" // Branch if minus
#define BVC    "102000" // Branch if overflow is clear
#define BVS    "102400" // Branch if overflow is set
#define BCC    "103000" // Branch if carry is clear
#define BCS    "103400" // Branch if carry is set

// Signed Conditional Branch
#define BGE    "002000" // Branch if greater than or equal (to zero)
#define BLT    "002400" // Branch if less than (zero)
#define BGT    "003000" // Branch if greater than (zero)
#define BLE    "003400" // Branch if less than or equal (to zero)
#define SOB    "077000" // Subtract one and branch (if not = 0)

// Unsigned Conditional Branch
#define BHI    "101000" // Branch if higher
#define BLOS   "101400" // Branch if lower or same
#define BHIS   "103000" // Branch if higher or same (same as BCC)
#define BLO    "103400" // Branch if lower (same as BCS)

// JUMP & SUBROUTINE

#define JMP    "000100" // Jump
#define JSR    "004000" // Jump to subroutine
#define RTS    "000200" // Return from subroutine
#define MARK   "006400" // Facilitates stack clean-up procedures

// TRAPS & INTERRUPTS

#define EMT    "104000" // Emulator trap
#define TRAP   "104400" // Trap
#define BPT    "000003" // Breakpoint trap
#define IOT    "000004" // Input/Output trap
#define CSM    // Call to supervisor mode
#define RTI    "000002" // Return from interrupt
#define RTT    "000002" // Return from trace trap (Same as RTI but supresses the immidialty-following trace trap)

// MISC

#define HALT   "000000" // Halt
#define WAIT   "000001" // Wait for interrupt
#define RESET  "000005" // Reset UNIBUS
#define MTPD   "106600" // Move to previous data space
#define MTPI   "006600" // Move to previous instruction space
#define MFPD   "106500" // Move from previous data space
#define MFPI   "006500" // Move from previous instruction space
#define MTPS   "106400" // Move byte to processor status word
#define MFPS   "106700" // Move byte from processor status word
#define MFPT   // Move from processor type

// CONDITION CODES

#define CLC   "000241" // Clear carry
#define CLV   "000242" // Clear overflow
#define CLZ   "000244" // Clear zero
#define CLN   "000250" // Clear negative
#define CCC   "000257" // Clear all flags
#define SEC   "000261" // Set carry
#define SEV   "000262" // Set overflow
#define SEZ   "000264" // Set zero
#define SEN   "000270" // Set negative
#define SCC   "000277" // Set all flags
