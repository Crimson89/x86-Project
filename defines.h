//Debug message verbosity levels
#define LOW_VERBOSITY   1
#define HIGH_VERBOSITY  2
#define DEBUG_VERBOSITY 3


//Handle bad instructions
#define BAD_INST_ABORT 1 // Set this to 0 to continue execution with bad/unimplemeted instructions, just print a message. Setting to 1 causes program execution to halt

//Size of memory space
#define MEMORY_SPACE (1<<16)


// Register indices
#define R0_REG_INDEX 0
#define R1_REG_INDEX 1
#define R2_REG_INDEX 2
#define R3_REG_INDEX 3
#define R4_REG_INDEX 4
#define R5_REG_INDEX 5
#define SP_REG_INDEX 6
#define PC_REG_INDEX 7
//program_execution_control values
#define PRINT_MENU  0
#define RUN_PROGRAM 1
#define LOAD_DATA   2

//Addressing Modes
// Register modes
#define REGISTER_MODE         0
#define REGISTER_DEFR_MODE    1
#define AUTO_INC_MODE         2
#define AUTO_INC_DEFR_MODE    3
#define AUTO_DEC_MODE         4
#define AUTO_DEC_DEFR_MODE    5
#define INDEX_MODE            6
#define INDEX_DEFR_MODE       7


// For use in reading from file
#define FILE_READ             8
// Read function parameters
#define MEMORY_READ           1
#define READ_TRACE            1
#define READ_NO_TRACE         0
#define READ_INSTR_FETCH      1
#define READ_NOT_INSTR_FETCH  0

// PC Only Modes
#define PC_IMMEDIATE_MODE     2
#define PC_ABSOLUTE_MODE      3
#define PC_RELATIVE_MODE      6
#define PC_REL_DEFR_MODE      7

// SP Only Modes
#define SP_DEFR_MODE           1
#define SP_AUTO_INC_MODE       2
#define SP_AUTO_INC_DEFR_MODE  3
#define SP_AUTO_DEC_MODE       4
#define SP_INDEX_MODE          6
#define SP_INDEX_DEFR_MODE     7
// Opcode values

// SINGLE OPERAND

// General
#define m_CLR    0005000 // Clear
#define m_CLRB   0105000 // Clear (B)
#define m_COM    0005100 // 1's Compliment
#define m_COMB   0105100 // 1's Compliment (B)
#define m_INC    0005200 // Increment
#define m_INCB   0105200 // Increment (B)
#define m_DEC    0005300 // Decrement
#define m_DECB   0105300 // Decrement (B)
#define m_NEG    0005400 // 2's Compliment negate
#define m_NEGB   0105400 // 2's Compliment negate (B)
#define m_NOP    0000240 // No operation
#define m_TST    0005700 // Test
#define m_TSTB   0105700 // Test (B)
#define m_TSTSET 0007200 // Test, set low bit
#define m_WRTLCK 0007300 // Read/Lock destination, write/unlock R0 #defineo destination

// Shift & Rotate
#define m_ASR    0006200 // Arithmetic shift right
#define m_ASRB   0106200 // Arithmetic shift right (B)
#define m_ASL    0006300 // Arithmetic shift left
#define m_ASLB   0106300 // Arithmetic shift left (B)
#define m_ROR    0006000 // Rotate right
#define m_RORB   0106000 // Rotate right (B)
#define m_ROL    0006100 // Rotate left
#define m_ROLB   0106100 // Rotate left (B)
#define m_SWAB   0000300 // Swap bytes

// Multiple Precision
#define m_ADC    0005500 // Add carry
#define m_ADCB   0105500 // Add carry (B)
#define m_SBC    0005600 // Subtract carry
#define m_SBCB   0105600 // Subtract carry (B)
#define m_SXT    0006700 // Sign extend

// DOUBLE OPERAND

// Logical
#define m_BIT    0030000 // Bit test
#define m_BITB   0130000 // Bit test (B)
#define m_BIC    0040000 // Bit clear
#define m_BICB   0140000 // Bit clear (B)
#define m_BIS    0050000 // Bit set
#define m_BISB   0150000 // Bit set (B)
#define m_XOR    0074000 // Exclusive OR
#define m_MOV    0010000 // Move source to destination
#define m_MOVB   0110000 // Move source to destination (B)
#define m_ADD    0060000 // Add source to destination
#define m_SUB    0160000 // Subtract source from destination
#define m_CMP    0020000 // Compare source to destination
#define m_CMPB   0120000 // Compare source to destination (B)
#define m_ASH    0072000 // Shift arithmetically
#define m_ASHC   0073000 // Arithmetic shift combined
#define m_MUL    0070000 // Multiply
#define m_DIV    0071000 // Divide

// BRANCH

// Branch
#define m_BR     0000400 // Branch unconditional
#define m_BNE    0001000 // Branch if not equal (to zero)
#define m_BEQ    0001400 // Branch if equal (to zero)
#define m_BPL    0100000 // Branch if plus
#define m_BMI    0100400 // Branch if minus
#define m_BVC    0102000 // Branch if overflow is clear
#define m_BVS    0102400 // Branch if overflow is set
#define m_BCC    0103000 // Branch if carry is clear
#define m_BCS    0103400 // Branch if carry is set

// Signed Conditional Branch
#define m_BGE    0002000 // Branch if greater than or equal (to zero)
#define m_BLT    0002400 // Branch if less than (zero)
#define m_BGT    0003000 // Branch if greater than (zero)
#define m_BLE    0003400 // Branch if less than or equal (to zero)
#define m_SOB    0077000 // Subtract one and branch (if not = 0)

// Unsigned Conditional Branch
#define m_BHI    0101000 // Branch if higher
#define m_BLOS   0101400 // Branch if lower or same
#define m_BHIS   0103000 // Branch if higher or same (same as BCC)
#define m_BLO    0103400 // Branch if lower (same as BCS)

// JUMP & SUBROUTINE

#define m_JMP    0000100 // Jump
#define m_JSR    0004000 // Jump to subroutine
#define m_RTS    0000200 // Return from subroutine
#define m_MARK   0006400 // Facilitates stack clean-up procedures

// TRAPS & INTERRUPTS

#define m_EMT    0104000 // Emulator trap
#define m_TRAP   0104400 // Trap
#define m_BPT    0000003 // Breakpoint trap
#define m_IOT    0000004 // Input/Output trap
#define m_CSM    0007000 // Call to supervisor mode
#define m_RTI    0000002 // Return from interrupt
#define m_RTT    0000006 // Return from trace trap (Same as RTI but supresses the immidialty-following trace trap)

// MISC

#define m_HALT   0000000 // Halt
#define m_WAIT   0000001 // Wait for interrupt
#define m_RESET  0000005 // Reset UNIBUS
#define m_MTPD   0106600 // Move to previous data space
#define m_MTPI   0006600 // Move to previous instruction space
#define m_MFPD   0106500 // Move from previous data space
#define m_MFPI   0006500 // Move from previous instruction space
#define m_MTPS   0106400 // Move byte to processor status word
#define m_MFPS   0106700 // Move byte from processor status word
#define m_MFPT   0000007 // Move from processor type
#define m_SPL    0000230 //SPL is not implemented in PDP-11

// CONDITION CODES
#define m_CCCNOP  0000240 //CCC/NOP
#define m_CLC     0000241 //CLC
#define m_CLV     0000242 //CLV
#define m_CLVnC   0000243 //CLV|CLC
#define m_CLZ     0000244 //CLZ
#define m_CLZnC   0000245 //CLZ|CLC
#define m_CLZnV   0000246 //CLZ|CLV
#define m_CLZnVnC 0000247 //CLZ|CLV|CLC
#define m_CLN     0000250 //CLN
#define m_CLNnC   0000251 //CLN|CLC
#define m_CLNnV   0000252 //CLN|CLV
#define m_CLNnVnC 0000253 //CLN|CLV|CLC
#define m_CLNnZ   0000254 //CLN|CLZ
#define m_CLNnZnC 0000255 //CLN|CLZ|CLC
#define m_CLNnZnV 0000256 //CLN|CLZ|CLV
#define m_CCC     0000257 //CCC
#define m_SCCNOP  0000260 //SCC/NOP
#define m_SEC     0000261 //SEC
#define m_SEV     0000262 //SEV
#define m_SEVnC   0000263 //SEV|SEC
#define m_SEZ     0000264 //SEZ
#define m_SEZnC   0000265 //SEZ|SEC
#define m_SEZnV   0000266 //SEZ|SEV
#define m_SEZnVnC 0000267 //SEZ|SEV|SEC
#define m_SEN     0000270 //SEN
#define m_SENnC   0000271 //SEN|SEC
#define m_SENnV   0000272 //SEN|SEV
#define m_SENnVnC 0000273 //SEN|SEV|SEC
#define m_SENnZ   0000274 //SEN|SEZ
#define m_SENnZnC 0000275 //SEN|SEZ|SEC
#define m_SENnZnV 0000276 //SEN|SEZ|SEV
#define m_SCC     0000277 //SCC


#define SHIFT_WORD_MSB_TO_LSB  15
#define SHIFT_BYTE_MSB_TO_LSB  7
#define WORD_MSB_MASK          0x8000
#define BYTE_MSB_MASK          0x80
#define WORD_MSB_INDEX         15
#define BYTE_MSB_INDEX         7
#define WORD_MAX               0xFFFF
#define BYTE_MAX               0xFF
#define IS_NEGATIVE(value, mask, shift_count) ((value & mask) >> shift_count)
#define IS_NEGATIVE_WORD(value) IS_NEGATIVE(value, WORD_MSB_MASK, SHIFT_WORD_MSB_TO_LSB)
#define IS_NEGATIVE_BYTE(value) IS_NEGATIVE(value, BYTE_MSB_MASK, SHIFT_BYTE_MSB_TO_LSB)
#define IS_ZERO(value) (value==0)
#define IS_NOT_ZERO(value) (value!=0)
#define CARRY_MSB_WORD(value) (value>WORD_MAX)
#define CARRY_MSB_BYTE(value) (value>BYTE_MAX)
#define EXTRACT_BIT(value, shift_count) (value >> shift_count)
#define IS_OVERFLOW_SUB(msb_dest, msb_src, msb_result)( ( (IS_ZERO(msb_dest) && IS_NOT_ZERO(msb_src) ) && IS_NOT_ZERO(msb_result) ) ||  ( (IS_NOT_ZERO(msb_dest) && IS_ZERO(msb_src) ) && IS_ZERO(msb_result) ) )
#define IS_OVERFLOW(msb_dest, msb_src, msb_result)( ( (IS_ZERO(msb_dest) && IS_ZERO(msb_src) ) && IS_NOT_ZERO(msb_result) ) ||  ( (IS_NOT_ZERO(msb_dest) && IS_NOT_ZERO(msb_src) ) && IS_ZERO(msb_result) ) )
