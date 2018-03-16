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
	//Math
    case m_XOR:
      return XOR(inst);
      break;
    case m_MOV:
    case m_MOVB:
      return MOV(inst);
      break;
    case m_ADD:
    case m_SUB:
      if(inst->byteMode) return SUB(inst);
	  else return ADD(inst);
      break;
    case m_CMP:
    case m_CMPB:
      return CMP(inst);
      break;
    case m_ASH:
      return ASH(inst);
      break;
    case m_ASHC:
      return ASHC(inst);  
      break;
    case m_MUL:
      return MUL(inst);
      break;
    case m_DIV:
      return DIV(inst);
      break;

    // BRANCH
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
	case m_BCC:
      return BCC(inst);
      break;
	case m_BCS:
      return BCS(inst);
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
    case m_SOB:
      return SOB(inst);
      break;

    // Unsigned Conditional Branch
    case m_BHI:
      return BHI(inst);
      break;
    case m_BLOS:
      return BLOS(inst);
      break;

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
    case m_MARK:
      return MARK(inst);
      break;

    // TRAPS & INTERRUPTS
    case m_EMT:
      return EMT(inst);
      break;
    case m_TRAP:
      return TRAP(inst);
      break;
    case m_BPT: 
      return BPT(inst);
      break;
    case m_IOT:
      return IOT(inst);
      break;
    case m_CSM: 
      return CSM(inst);
      break;
    case m_RTI: 
      return RTI(inst);
      break;
    case m_RTT:
      return RTT(inst);
      break;

	  
    // MISC
    case m_HALT: 
      return HALT(inst);
      break;
    case m_WAIT:
      return WAIT(inst);
      break;
    case m_RESET: 
      return RESET(inst);
      break;
    case m_MTPD:
      return MTPD(inst);
      break;
    case m_MTPI:
      return MTPI(inst);
      break;
    case m_MFPD: 
      return MFPD(inst);
      break;
    case m_MFPI:
      return MFPI(inst);
      break;
    case m_MTPS:
      return MTPS(inst);
      break;
    case m_MFPS: 
      return MFPS(inst);
      break;
    case m_MFPT: 
      return MFPT(inst);
      break;
    case m_SPL: 
      return SPL(inst);
      break;
	 case m_TSTSET:
      return TSTSET(inst);
      break;
	 case m_WRTLCK:
      return WRTLCK(inst);
      break;
	  
   // CONDITION CODES
	case m_CCCNOP :
	case m_CLC    :
	case m_CLV    :
	case m_CLVnC  :
	case m_CLZ    :
	case m_CLZnC  :
	case m_CLZnV  :
	case m_CLZnVnC:
	case m_CLN    :
	case m_CLNnC  :
	case m_CLNnV  :
	case m_CLNnVnC:
	case m_CLNnZ  :
	case m_CLNnZnC:
	case m_CLNnZnV:
	case m_CCC    :
      return CCC(inst);
      break;
	case m_SCCNOP :
	case m_SEC    :
	case m_SEV    :
	case m_SEVnC  :
	case m_SEZ    :
	case m_SEZnC  :
	case m_SEZnV  :
	case m_SEZnVnC:
	case m_SEN    :
	case m_SENnC  :
	case m_SENnV  :
	case m_SENnVnC:
	case m_SENnZ  :
	case m_SENnZnC:
	case m_SENnZnV:
	case m_SCC    :
      return SCC(inst);
      break;
	default: if(verbosity_level > HIGH_VERBOSITY) cerr << "Dispatch operation failed, unknown masked opCode "<< inst->opcode << endl;break;

  }

}
