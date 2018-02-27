#define SILENT_MODE 1
#define VERBOSE_MODE 2
#define MEMORY_SPACE ( (1<<15) - (1<<12) )
//#define HALT 0x000000 (why is this here?)
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

//TODO I think we need to add a leading 0 to some of these.
// General
#define opCLR    005000 // Clear
#define opCLRB   0105000 // Clear (B)
#define opCOM    005100 // 1's Compliment
#define opCOMB   0105100 // 1's Compliment (B)
#define opINC    005200 // Increment
#define opINCB   0105200 // Increment (B)
#define opDEC    005300 // Decrement
#define opDECB   0105300 // Decrement (B)
#define opNEG    005400 // 2's Compliment negate
#define opNEGB   0105400 // 2's Compliment negate (B)
#define opNOP    000240 // No operation
#define opTST    005700 // Test
#define opTSTB   0105700 // Test (B)
#define opTSTSET // Test, set low bit
#define opWRTLCK // Read/Lock destination, write/unlock R0 #defineo destination

// Shift & Rotate
#define opASR    006200 // Arithmetic shift right
#define opASRB   0106200 // Arithmetic shift right (B)
#define opASL    006300 // Arithmetic shift left
#define opASLB   0106300 // Arithmetic shift left (B)
#define opROR    006000 // Rotate right
#define opRORB   0106000 // Rotate right (B)
#define opROL    006100 // Rotate left
#define opROLB   0106100 // Rotate left (B)
#define opSWAB   000300 // Swap bytes

// Multiple Precision
#define opADC    005500 // Add carry
#define opADCB   0105500 // Add carry (B)
#define opSBC    005600 // Subtract carry
#define opSBCB   0105600 // Subtract carry (B)
#define opSXT    006700 // Sign extend

// DOUBLE OPERAND

// General
#define opMOV    010000 // Move source to destination
#define opMOVB   0110000 // Move source to destination (B)
#define opADD    060000 // Add source to destination
#define opSUB    0160000 // Subtract source from destination
#define opCMP    020000 // Compare source to destination
#define opCMPB   0120000 // Compare source to destination (B)
#define opASH    072000 // Shift arithmetically
#define opASHC   073000// Arithmetic shift combined
#define opMUL    070000 // Multiply
#define opDIV    071000 // Divide

// Logical
#define opBIT    030000 // Bit test
#define opBITB   0130000 // Bit test (B)
#define opBIC    040000 // Bit clear
#define opBICB   0140000 // Bit clear (B)
#define opBIS    050000 // Bit set
#define opBISB   0150000 // Bit set (B)
#define opXOR    074000 // Exclusive OR

// BRANCH

// Branch
#define opBR     000400 // Branch unconditional
#define opBNE    001000 // Branch if not equal (to zero)
#define opBEQ    001400 // Branch if equal (to zero)
#define opBPL    0100000 // Branch if plus
#define opBMI    0100400 // Branch if minus
#define opBVC    0102000 // Branch if overflow is clear
#define opBVS    0102400 // Branch if overflow is set
#define opBCC    0103000 // Branch if carry is clear
#define opBCS    0103400 // Branch if carry is set

// Signed Conditional Branch
#define opBGE    002000 // Branch if greater than or equal (to zero)
#define opBLT    002400 // Branch if less than (zero)
#define opBGT    003000 // Branch if greater than (zero)
#define opBLE    003400 // Branch if less than or equal (to zero)
#define opSOB    077000 // Subtract one and branch (if not = 0)

// Unsigned Conditional Branch
#define opBHI    0101000 // Branch if higher
#define opBLOS   0101400 // Branch if lower or same
#define opBHIS   0103000 // Branch if higher or same (same as BCC)
#define opBLO    0103400 // Branch if lower (same as BCS)

// JUMP & SUBROUTINE

#define opJMP    000100 // Jump
#define opJSR    004000 // Jump to subroutine
#define opRTS    000200 // Return from subroutine
#define opMARK   006400 // Facilitates stack clean-up procedures

// TRAPS & INTERRUPTS

#define opEMT    0104000 // Emulator trap
#define opTRAP   0104400 // Trap
#define opBPT    000003 // Breakpoint trap
#define opIOT    000004 // Input/Output trap
#define opCSM    // Call to supervisor mode
#define opRTI    000002 // Return from interrupt
#define opRTT    000002 // Return from trace trap (Same as RTI but supresses the immidialty-following trace trap)

// MISC

#define opHALT   000000 // Halt
#define opWAIT   000001 // Wait for interrupt
#define opRESET  000005 // Reset UNIBUS
#define opMTPD   0106600 // Move to previous data space
#define opMTPI   006600 // Move to previous instruction space
#define opMFPD   0106500 // Move from previous data space
#define opMFPI   006500 // Move from previous instruction space
#define opMTPS   0106400 // Move byte to processor status word
#define opMFPS   0106700 // Move byte from processor status word
#define opMFPT   // Move from processor type

// CONDITION CODES

#define opCLC   000241 // Clear carry
#define opCLV   000242 // Clear overflow
#define opCLZ   000244 // Clear zero
#define opCLN   000250 // Clear negative
#define opCCC   000257 // Clear all flags
#define opSEC   000261 // Set carry
#define opSEV   000262 // Set overflow
#define opSEZ   000264 // Set zero
#define opSEN   000270 // Set negative
#define opSCC   000277 // Set all flags
