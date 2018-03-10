#include "header.h"
// dispatch.cpp


// Function to call operation functions
// Takes instruction* (current instruction) & uint16_t (opcode)
// Passes back return value from the function it called

int dispatch(instruction * inst)
{
  switch (inst->opcode) {

    // SINGLE OPERAND

    // General
    case 0005000: // CLR
    case 0105000: // CLRB
      return CLR(inst);
      break;
    case 0005100: // COM
    case 0105100: // COMB
      return COM(inst);
      break;
    case 0005200: // INC
    case 0105200: // INCB
      return INC(inst);
      break;
    case 0005300: // DEC
    case 0105300: // DECB
      return DEC(inst);
      break;
    case 0005400: // NEG
    case 0105400: // NEGB
      return NEG(inst);
      break;
    case 0000240: // NOP
      return NOP(inst);
      break;
    case 0005700: // TST
    case 0105700: // TSTB
      return TST(inst);
      break;
    
    // Shift & Rotate
    case 0006200: // ASR
    case 0106200: // ASRB
      return ASR(inst);
      break;
    case 0006300: // ASL
    case 0106300: // ASLB
      return ASL(inst);
      break;
    case 0006000: // ROR
    case 0106000: // RORB
      return ROR(inst);
      break;
    case 0006100: // ROL
    case 0106100: // ROLB
      return ROL(inst);
      break;
    case 0000300: // SWAB
      return SWAB(inst);
      break;

    // Multiple precision
    case 0005500: // ADC
    case 0105500: // ADCB
      return ADC(inst);
      break;
    case 0005600: // SBC
    case 0105600: // SBCB
      return SBC(inst);
      break;
    case 0006700: // SXT
      return SXT(inst);
      break;

    // DOUBLE OPERAND

    // Logical
    case 0030000: // BIT
    case 0130000: // BITB
      return BIT(inst);
      break;
    case 0040000: // BIC
    case 0140000: // BICB
      return BIC(inst);
      break;
    case 0050000: // BIS
    case 0150000: // BISB
      return BIS(inst);
      break;
    case 0074000: // XOR
      return XOR(inst);
      break;
    case 0010000: // MOV
    case 0110000: // MOVB
      return MOV(inst);
      break;
    case 0060000: // ADD
      return ADD(inst);
      break;
    case 0160000: // SUB
      return SUB(inst);
      break;
    case 0020000: // CMP
    case 0120000: // CMPB
      return CMP(inst);
      break;
    case 0072000: // ASH
      return ASH(inst);
      break;
    case 0073000: // ASHC
      return ASHC(inst);  
      break;
    case 0070000: // MUL
      return MUL(inst);
      break;
    case 0071000: // DIV
      return DIV(inst);
      break;

    // BRANCH

    // Branch
    case 0000400: // BR
      return BR(inst);
      break;
    case 0001000: // BNE
      return BNE(inst);
      break;
    case 0001400: // BEQ
      return NEQ(inst);
      break;
    case 0100000: // BPL
      return BPL(inst);
      break;
    case 0100400: // BMI
      return BMI(inst);
      break;
    case 0102000: // BVC
      return BVC(inst);
      break;
    case 0102400: // BVS
      return BVS(inst);
      break;

    // Signed Conditional Branch
    case 0002000: // BGE
      return BGE(inst);
      break;
    case 0002400: // BLT
      return BLT(inst);
      break;
    case 0003000: // BGT
      return BGT(inst);
      break;
    case 0003400: // BLE
      return BLE(inst);
      break;
    case 0077000: // SOB
      return SOB(inst);
      break;

    // Unsigned Conditional Branch
    case 0101000: // BHI
      return BHI(inst);
      break;
    case 0101400: // BLOS
      return BLOS(inst);
      break;
    case 0103000: // BHIS and BCC
      return BHIS(inst);
      break;
    case 0103400: // BCS and  BLO
      return BCS(inst);
      break;

    // JUMP & SUBROUTINE

    case 0000100: // JMP
      return JMP(inst);
      break;
    case 0004000: // JSR
      return JSR(inst);
      break;
    case 0000200: // RTS
      return RTS(inst);
      break;
    case 0006400: // MARK
      return MARK(inst);
      break;

    // TRAPS & INTERRUPTS
    /*
    case 0104000: // EMT
      return EMT(inst);
      break;
    case 0104400: // TRAP
      return TRAP(inst);
      break;
    case 0000003: // BPT
      return BPT(inst);
      break;
    case 0000004: // IOT
      return IOT(inst);
      break;
    case 0007000: // CSM
      return CSM(inst);
      break;
    case 0000002: // RTI
      return RTI(inst);
      break;
    case 0000006: // RTT
      return RTT(inst);
      break;
    */

    // MISC

    case 0000000: // HALT
      return HALT(inst);
      break;
    case 0000001: // WAIT
      return WAIT(inst);
      break;
    /*
    case 0000005: // RESET
      return RESET(inst);
      break;
    case 0106600: // MTPD
      return MTPD(inst);
      break;
    case 0006600: // MTPI
      return MTPI(inst);
      break;
    case 0106500: // MFPD
      return MFPD(inst);
      break;
    case 0006500: // MFPI
      return MFPI(inst);
      break;
    case 0106400: // MTPS
      return MTPS(inst);
      break;
    case 0106700: // MFPS
      return MFPS(inst);
      break;
    case 0000007: // MFPT
      return MFPT(inst);
      break;
    case 0000230: // SPL
      return SPL(inst);
      break;
    */

    // CONDITION CODES

    case 0000241: // CLC
      return CLC(inst);
      break;
    case 0000242: // CLV
      return CLV(inst);
      break;
    case 0000244: // CLZ
      return CLZ(inst);
      break;
    case 0000250: // CLN
      return CLN(inst);
      break;
    case 0000257: // CCC
      return CCC(inst);
      break;
    case 0000261: // SEC
      return SEC(inst);
      break;
    case 0000262: // SEV
      return SEV(inst);
      break;
    case 0000264: // SEZ
      return SEZ(inst);
      break;
    case 0000270: // SEN
      return SEN(inst);
      break;
    case 0000277: // SCC
      return SCC(inst);
      break;
  }

}
