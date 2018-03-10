#include "header.h"

// Branch
int BR(instruction *inst) // Branch unconditional
{
  // PC = PC + (2 x offset)

  int16_t signed_offset;
  memcpy(&signed_offset, &(inst->offset), 2);
  PC += (signed_offset << 1); 

  return 0;
}

int BNE(instruction *inst) // Branch if not equal (to zero)
{
  if(inst->Z) {
    int16_t signed_offset;
    memcpy(&signed_offset, &(inst->offset), 2);
    PC += (signed_offset << 1); 
  }

  return 0;
}

int BEQ(instruction *inst) // Branch if equal (to zero)
{
  if(!(inst->Z)) {
    int16_t signed_offset;
    memcpy(&signed_offset, &(inst->offset), 2);
    PC += (signed_offset << 1); 
  }

  return 0;
}

int BPL(instruction *inst) // Branch if plus
{
  if(!(inst->N)) {
    int16_t signed_offset;
    memcpy(&signed_offset, &(inst->offset), 2);
    PC += (signed_offset << 1); 
  }

  return 0;
}

int BMI(instruction *inst) // Branch if minus
{
  if(inst->N) {
    int16_t signed_offset;
    memcpy(&signed_offset, &(inst->offset), 2);
    PC += (signed_offset << 1); 
  }

  return 0;
}

int BVC(instruction *inst) // Branch if overflow is clear
{
  if(!(inst->V)) {
    int16_t signed_offset;
    memcpy(&signed_offset, &(inst->offset), 2);
    PC += (signed_offset << 1); 
  }

  return 0;
}

int BVS(instruction *inst) // Branch if overflow is set
{
  if(inst->V) {
    int16_t signed_offset;
		memcpy(&signed_offset, &(inst->offset), 2);
    PC += (signed_offset << 1); 
  }

  return 0;
}

int BCC(instruction *inst) // Branch if carry is clear
{
  if(!inst->C) {
    int16_t signed_offset;
		memcpy(&signed_offset, &(inst->offset), 2);
    PC += (signed_offset << 1); 
  }

  return 0;
}

int BCS(instruction *inst) // Branch if carry is set
{
  if(inst->C) {
    int16_t signed_offset;
		memcpy(&signed_offset, &(inst->offset), 2);
    PC += (signed_offset << 1); 
  }

  return 0;
}

// Signed Conditional Branch
int BGE(instruction *inst) // Branch if greater than or equal (to zero)
{
  // Branch if N XNOR V 
  if(inst->N == inst->V) {
    int16_t signed_offset;
		memcpy(&signed_offset, &(inst->offset), 2);
    PC += (signed_offset << 1); 
  }

  return 0;
}

int BLT(instruction *inst) // Branch if less than (zero)
{
  // Branch if N XOR V
  if(inst->N != inst->V) {
    int16_t signed_offset;
		memcpy(&signed_offset, &(inst->offset), 2);
    PC += (signed_offset << 1); 
  }

  return 0;
}

int BGT(instruction *inst) // Branch if greater than (zero)
{
  // Branch if (N XNOR V) && !Z
  if((inst->N == inst->V) && !(inst->Z)) {
    int16_t signed_offset;
		memcpy(&signed_offset, &(inst->offset), 2);
    PC += (signed_offset << 1); 
  }

  return 0;
}

int BLE(instruction *inst) // Branch if less than or equal (to zero)
{
  // Branch if (N XOR V) || Z
  if(inst->Z || (inst->N != inst->V)) {
    int16_t signed_offset;
		memcpy(&signed_offset, &(inst->offset), 2);
    PC += (signed_offset << 1); 
  }

  return 0;
}

// Unsigned Conditional Branch
int BHI(instruction *inst) // Branch if higher
{
  if(!(inst->C) && !(inst->Z)) {
    int16_t signed_offset;
		memcpy(&signed_offset, &(inst->offset), 2);
    PC += (signed_offset << 1); 
  }

  return 0;
}


int BLOS(instruction *inst) // Branch if lower or same
{
  if(inst->C || inst->Z) {
    int16_t signed_offset;
		memcpy(&signed_offset, &(inst->offset), 2);
    PC += (signed_offset << 1); 
  }

  return 0;
}

// Duplicate of BCC
int BHIS(instruction *inst) // Branch if higher or same
{
  if(!(inst->C)) {
    int16_t signed_offset;
		memcpy(&signed_offset, &(inst->offset), 2);
    PC += (signed_offset << 1); 
  }

  return 0;
}

// Duplicate of BCS
int BLO(instruction *inst) // Branch if lower
{
  if(inst->C) {
    int16_t signed_offset;
		memcpy(&signed_offset, &(inst->offset), 2);
    PC += (signed_offset << 1); 
  }

  return 0;
}


