// dispatch.cpp

#include "header.h"

// Function to call operation functions
// Takes instruction* (current instruction) & uint16_t (opcode)
// Passes back return value from the function it called

int dispatch(instruction * inst)
{
  switch (inst->opcode) {

    // SINGLE OPERAND

    // General
    case m_CLR:
    case m_CLRB:
      return CLR(inst);
      break;
    case m_COM:
    case m_COMB:
      return COM(inst);
      break;
    case m_INC:
    case m_INCB:
      return INC(inst);
      break;
    case m_DEC:
    case m_DECB:
      return DEC(inst);
      break;
    case m_NEG:
    case m_NEGB:
      return NEG(inst);
      break;
    //case 0000240: // NOP
    //  return NOP(inst);
    //  break;
    case m_TST:
    case m_TSTB:
      return TST(inst);
      break;
    
    // Shift & Rotate
    case m_ASR:
    case m_ASRB:
      return ASR(inst);
      break;
    case m_ASL:
    case m_ASLB:
      return ASL(inst);
      break;
    case m_ROR:
    case m_RORB:
      return ROR(inst);
      break;
    case m_ROL:
    case m_ROLB:
      return ROL(inst);
      break;
    case m_SWAB:
      return SWAB(inst);
      break;

    // Multiple precision
    case m_ADC:
    case m_ADCB:
      return ADC(inst);
      break;
    case m_SBC:
    case m_SBCB:
      return SBC(inst);
      break;
    case m_SXT:
      return SXT(inst);
      break;

    // DOUBLE OPERAND

    // Logical
    case m_BIT:
    case m_BITB:
      return BIT(inst);
      break;
    case m_BIC:
    case m_BICB:
      return BIC(inst);
      break;
    case m_BIS:
    case m_BISB:
      return BIS(inst);
      break;
    //case m_XOR:
    //  return XOR(inst);
    //  break;
    case m_MOV:
    case m_MOVB:
      return MOV(inst);
      break;
    case m_ADD:
    case m_SUB:
      if(inst->byteMode) return SUB(inst);
	  return ADD(inst);
      break;
    case m_CMP:
    case m_CMPB:
      return CMP(inst);
      break;
    //case m_ASH:
    //  return ASH(inst);
    //  break;
    //case m_ASHC:
    //  return ASHC(inst);  
    //  break;
    //case m_MUL:
    //  return MUL(inst);
    //  break;
    //case m_DIV:
    //  return DIV(inst);
    //  break;

    // BRANCH

    // Branch
    case m_BR:
      return BR(inst);
      break;
    case m_BNE:
      return BNE(inst);
      break;
    case m_BEQ:
      return BEQ(inst);
      break;
    case m_BPL:
      return BPL(inst);
      break;
    case m_BMI:
      return BMI(inst);
      break;
    case m_BVC:
      return BVC(inst);
      break;
    case m_BVS:
      return BVS(inst);
      break;

    // Signed Conditional Branch
    case m_BGE:
      return BGE(inst);
      break;
    case m_BLT:
      return BLT(inst);
      break;
    case m_BGT:
      return BGT(inst);
      break;
    case m_BLE:
      return BLE(inst);
      break;
    //case m_SOB:
    //  return SOB(inst);
    //  break;

    // Unsigned Conditional Branch
    case m_BHI:
      return BHI(inst);
      break;
    case m_BLOS:
      return BLOS(inst);
      break;
    //case m_BHIS:
    //  return BHIS(inst);
    //  break;
    //case m_BLO:
    //  return BLO(inst);
    //  break;

    // JUMP & SUBROUTINE

    case m_JMP:
      return JMP(inst);
      break;
    case m_JSR:
      return JSR(inst);
      break;
    case m_RTS:
      return RTS(inst);
      break;
    //case m_MARK
    //  return MARK(inst);
    //  break;

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

    //case 0000000: // HALT
    //  return HALT(inst);
    //  break;
    //case 0000001: // WAIT
    //  return WAIT(inst);
    //  break;
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
/*
    case m_CLC:
      return CLC(inst);
      break;
    case m_CLV:
      return CLV(inst);
      break;
    case m_CLZ:
      return CLZ(inst);
      break;
    case m_CLN:
      return CLN(inst);
      break;
    case m_CCC:
      return CCC(inst);
      break;
    case m_SEC:
      return SEC(inst);
      break;
    case m_SEV:
      return SEV(inst);
      break;
    case m_SEZ:
      return SEZ(inst);
      break;
    case m_SEN:
      return SEN(inst);
      break;
    case m_SCC:
      return SCC(inst);
      break;
*/
  }

}
