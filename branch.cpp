#include "header.h"

// Branch
int BR(instruction *inst) // Branch unconditional
{
  // PC = PC + (2 x offset)

  int16_t signed_offset = (int16_t)inst->offset;
  PC += (signed_offset << 1); 

  return 0;
}

int BNE(instruction *inst) // Branch if not equal (to zero)
{
  if(PSW.Z) {
    int16_t signed_offset = (int16_t)inst->offset;
    PC += (signed_offset << 1); 
  }

  return 0;
}

int BEQ(instruction *inst) // Branch if equal (to zero)
{
  if(!PSW.Z) {
    int16_t signed_offset = (int16_t)inst->offset;
    PC += (signed_offset << 1); 
  }

  return 0;
}

int BPL(instruction *inst) // Branch if plus
{
  if(!PSW.N) {
    int16_t signed_offset = (int16_t)inst->offset;
    PC += (signed_offset << 1); 
  }

  return 0;
}

int BMI(instruction *inst) // Branch if minus
{
  if(PSW.N) {
    int16_t signed_offset = (int16_t)inst->offset;
    PC += (signed_offset << 1); 
  }

  return 0;
}

int BVC(instruction *inst) // Branch if overflow is clear
{
  if(!PSW.V) {
    int16_t signed_offset = (int16_t)inst->offset;
    PC += (signed_offset << 1); 
  }

  return 0;
}

int BVS(instruction *inst) // Branch if overflow is set
{
  if(PSW.V) {
    int16_t signed_offset = (int16_t)inst->offset;
    PC += (signed_offset << 1); 
  }

  return 0;
}

int BCC(instruction *inst) // Branch if carry is clear
{
  if(!PSW.C) {
    int16_t signed_offset = (int16_t)inst->offset;
    PC += (signed_offset << 1); 
  }

  return 0;
}

int BCS(instruction *inst) // Branch if carry is set
{
  if(PSW.C) {
    int16_t signed_offset = (int16_t)inst->offset;
    PC += (signed_offset << 1); 
  }

  return 0;
}

// Signed Conditional Branch
int BGE(instruction *inst) // Branch if greater than or equal (to zero)
{
  // Branch if N XNOR V 
  if(PSW.N == PSW.V) {
    int16_t signed_offset = (int16_t)inst->offset;
    PC += (signed_offset << 1); 
  }

  return 0;
}

int BLT(instruction *inst) // Branch if less than (zero)
{
  // Branch if N XOR V
  if(PSW.N != PSW.V) {
    int16_t signed_offset = (int16_t)inst->offset;
    PC += (signed_offset << 1); 
  }

  return 0;
}

int BGT(instruction *inst) // Branch if greater than (zero)
{
  // Branch if (N XNOR V) && !Z
  if((PSW.N == PSW.V) && !PSW.Z) {
    int16_t signed_offset = (int16_t)inst->offset;
    PC += (signed_offset << 1); 
  }

  return 0;
}

int BLE(instruction *inst) // Branch if less than or equal (to zero)
{
  // Branch if (N XOR V) || Z
  if(PSW.Z || (PSW.N != PSW.V)) {
    int16_t signed_offset = (int16_t)inst->offset;
    PC += (signed_offset << 1); 
  }

  return 0;
}

// Unsigned Conditional Branch
int BHI(instruction *inst) // Branch if higher
{
  if(!PSW.C && !PSW.Z) {
    int16_t signed_offset = (int16_t)inst->offset;
    PC += (signed_offset << 1); 

  return 0;
}

int BLOS(instruction *inst) // Branch if lower or same
{
  if(PSW.C || PSW.Z) {
    int16_t signed_offset = (int16_t)inst->offset;
    PC += (signed_offset << 1); 
  }

  return 0;
}

// Duplicate of BCC
int BHIS(instruction *inst) // Branch if higher or same
{
  if(!PSW.C) {
    int16_t signed_offset = (int16_t)inst->offset;
    PC += (signed_offset << 1); 
  }

  return 0;
}

// Duplicate of BCS
int BLO(instruction *inst) // Branch if lower
{
  if(PSW.C) {
    int16_t signed_offset = (int16_t)inst->offset;
    PC += (signed_offset << 1); 
  }

  return 0;
}


