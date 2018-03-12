#include "header.h"

int HALT(instruction *inst) //HALT
{
  inst->op_text = "HALT";
  return 0;
}


int NOP(instruction *inst){ // No Operation
  inst->op_text = "NOP";
  return 0;
}

int SPL(instruction *inst) // Set priority level
{
  inst->op_text = "SPL";
  cerr << inst->op_text << " not implemented in PDP-11/60" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int TSTSET(instruction *inst) // TSTSET
{
  inst->op_text = "TSTSET";
  cerr << inst->op_text << " not implemented in PDP-11/60" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int WRTLCK(instruction *inst) // WRTLCK
{
  inst->op_text = "WRTLCK";
  cerr << inst->op_text << " not implemented in PDP-11/60" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int EMT(instruction *inst) // Emulator trap
{
  inst->op_text = "EMT";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int XOR(instruction *inst) //XOR
{
  inst->op_text = "XOR";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int ASH(instruction *inst) //ASH
{
  inst->op_text = "ASH";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}
int ASHC(instruction *inst) //ASHC
{
  inst->op_text = "ASHC";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int MUL(instruction *inst) //MUL
{
  inst->op_text = "MUL";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}
int DIV(instruction *inst) //DIV
{
  inst->op_text = "DIV";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int SOB(instruction *inst) //SOB
{
  inst->op_text = "SOB";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int MARK(instruction *inst) //MARK
{
  inst->op_text = "MARK";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}
int TRAP(instruction *inst) //TRAP
{
  inst->op_text = "TRAP";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int BPT(instruction *inst) //BPT
{
  inst->op_text = "BPT";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int IOT(instruction *inst) //IOT
{
  inst->op_text = "IOT";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int CSM(instruction *inst) //CSM
{
  inst->op_text = "CSM";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int RTI(instruction *inst) //RTI
{
  inst->op_text = "RTI";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int RTT(instruction *inst) //RTT
{
  inst->op_text = "RTT";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int WAIT(instruction *inst) //WAIT
{
  inst->op_text = "WAIT";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int RESET(instruction *inst) //RESET
{
  inst->op_text = "RESET";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int MTPD(instruction *inst) //MTPD
{
  inst->op_text = "MTPD";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int MTPI(instruction *inst) //MTPI
{
  inst->op_text = "MTPI";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int MFPD(instruction *inst) //MFPD
{
  inst->op_text = "MFPD";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int MFPI(instruction *inst) //MFPI
{
  inst->op_text = "MFPI";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int MTPS(instruction *inst) //MTPS
{
  inst->op_text = "MTPS";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int MFPS(instruction *inst) //MFPS
{
  inst->op_text = "MFPS";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}

int MFPT(instruction *inst) //MFPT
{
  inst->op_text = "MFPT";
  cerr << "Instruction \'" << inst->op_text << "\' not implemented" << endl;
  if(BAD_INST_ABORT) {
	  return 1;
  }
  return 0;
}