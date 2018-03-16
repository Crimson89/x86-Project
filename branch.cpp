#include "header.h"

// Branch
int BR(instruction *inst) // Branch unconditional
{
  inst->op_text = "BR";
  inst->is_branch = true;
  inst->branch_taken = true;

  uint16_t branchOffset;
  if ((inst->offset & 0x80) == 0)
  {
    branchOffset = inst->offset;
  }
  else
  {
    branchOffset = inst->offset | 0xFF00;
  }
  
  PC += (branchOffset << 1);
  inst->branch_target = PC;

  return 0;
}

int BNE(instruction *inst) // Branch if not equal (to zero)
{
  inst->op_text = "BNE";
  inst->is_branch = true;
  uint16_t branchOffset;

  if ((inst->offset & 0x80) == 0)
  {
    branchOffset = inst->offset;
  }
  else
  {
    branchOffset = inst->offset | 0xFF00;
  }
  inst->branch_target = PC + (branchOffset << 1);
  if(!(inst->Z)) {
    inst->branch_taken = true;
    PC += (branchOffset << 1); 
  }

  return 0;
}

int BEQ(instruction *inst) // Branch if equal (to zero)
{
  inst->op_text = "BEQ";
  inst->is_branch = true;
  uint16_t branchOffset;

  if ((inst->offset & 0x80) == 0)
  {
    branchOffset = inst->offset;
  }
  else
  {
    branchOffset = inst->offset | 0xFF00;
  }
  inst->branch_target = PC + (branchOffset << 1);
  if(inst->Z) {
    inst->branch_taken = true;
    PC += (branchOffset << 1);  
  }

  return 0;
}

int BPL(instruction *inst) // Branch if plus
{
  inst->op_text = "BPL";
  inst->is_branch = true;
  uint16_t branchOffset;

  if ((inst->offset & 0x80) == 0)
  {
    branchOffset = inst->offset;
  }
  else
  {
    branchOffset = inst->offset | 0xFF00;
  }
  inst->branch_target = PC + (branchOffset << 1);
  if(!(inst->N)) {
    inst->branch_taken = true;
    PC += (branchOffset << 1); 
  }

  return 0;
}

int BMI(instruction *inst) // Branch if minus
{
  inst->op_text = "BMI";
  inst->is_branch = true;
  uint16_t branchOffset;

  if ((inst->offset & 0x80) == 0)
  {
    branchOffset = inst->offset;
  }
  else
  {
    branchOffset = inst->offset | 0xFF00;
  }
  inst->branch_target = PC + (branchOffset << 1);
  if(inst->N) {
    inst->branch_taken = true;
    PC += (branchOffset << 1);  
  }

  return 0;
}

int BVC(instruction *inst) // Branch if overflow is clear
{
  inst->op_text = "BVC";
  inst->is_branch = true;
  uint16_t branchOffset;

  if ((inst->offset & 0x80) == 0)
  {
    branchOffset = inst->offset;
  }
  else
  {
    branchOffset = inst->offset | 0xFF00;
  }
  inst->branch_target = PC + (branchOffset << 1);
  if(!(inst->V)) {
    inst->branch_taken = true;
    PC += (branchOffset << 1); 
  }

  return 0;
}

int BVS(instruction *inst) // Branch if overflow is set
{
  inst->op_text = "BVS";
  inst->is_branch = true;
  uint16_t branchOffset;

  if ((inst->offset & 0x80) == 0)
  {
    branchOffset = inst->offset;
  }
  else
  {
    branchOffset = inst->offset | 0xFF00;
  }
  inst->branch_target = PC + (branchOffset << 1);
  if(inst->V) {
    inst->branch_taken = true;
    PC += (branchOffset << 1);  
  }

  return 0;
}

int BCC(instruction *inst) // Branch if carry is clear
{
  inst->op_text = "BCC";
  inst->is_branch = true;
  uint16_t branchOffset;

  if ((inst->offset & 0x80) == 0)
  {
    branchOffset = inst->offset;
  }
  else
  {
    branchOffset = inst->offset | 0xFF00;
  }
  inst->branch_target = PC + (branchOffset << 1);
  if(!inst->C) {
    inst->branch_taken = true;
    PC += (branchOffset << 1);  
  }

  return 0;
}

int BCS(instruction *inst) // Branch if carry is set
{
  inst->op_text = "BCS";
  inst->is_branch = true;
  uint16_t branchOffset;

  if ((inst->offset & 0x80) == 0)
  {
    branchOffset = inst->offset;
  }
  else
  {
    branchOffset = inst->offset | 0xFF00;
  }
  inst->branch_target = PC + (branchOffset << 1);
  if(inst->C) {
    inst->branch_taken = true;
    PC += (branchOffset << 1); 
  }

  return 0;
}

// Signed Conditional Branch
int BGE(instruction *inst) // Branch if greater than or equal (to zero)
{
  inst->op_text = "BGE";
  inst->is_branch = true;
  uint16_t branchOffset;
  // Branch if N XNOR V 

  if ((inst->offset & 0x80) == 0)
  {
    branchOffset = inst->offset;
  }
  else
  {
    branchOffset = inst->offset | 0xFF00;
  }
  inst->branch_target = PC + (branchOffset << 1);
  if(inst->N == inst->V) {
    inst->branch_taken = true;
    PC += (branchOffset << 1);  
  }

  return 0;
}

int BLT(instruction *inst) // Branch if less than (zero)
{
  inst->op_text = "BLT";
  inst->is_branch = true;
  uint16_t branchOffset;
  // Branch if N XOR V

  if ((inst->offset & 0x80) == 0)
  {
    branchOffset = inst->offset;
  }
  else
  {
    branchOffset = inst->offset | 0xFF00;
  }
  inst->branch_target = PC + (branchOffset << 1);
  if(inst->N != inst->V) {
    inst->branch_taken = true;
    PC += (branchOffset << 1);  
  }

  return 0;
}

int BGT(instruction *inst) // Branch if greater than (zero)
{
  inst->op_text = "BGT";
  inst->is_branch = true;
  uint16_t branchOffset;
  // Branch if (N XNOR V) && !Z

  if ((inst->offset & 0x80) == 0)
  {
    branchOffset = inst->offset;
  }
  else
  {
    branchOffset = inst->offset | 0xFF00;
  }
  inst->branch_target = PC + (branchOffset << 1);
  if((inst->N == inst->V) && !(inst->Z)) {
    inst->branch_taken = true;
    PC += (branchOffset << 1); 
  }

  return 0;
}

int BLE(instruction *inst) // Branch if less than or equal (to zero)
{
  inst->op_text = "BLE";
  inst->is_branch = true;
  uint16_t branchOffset;
  // Branch if (N XOR V) || Z

  if ((inst->offset & 0x80) == 0)
  {
    branchOffset = inst->offset;
  }
  else
  {
    branchOffset = inst->offset | 0xFF00;
  }
  inst->branch_target = PC + (branchOffset << 1);
  if(inst->Z || (inst->N != inst->V)) {
    inst->branch_taken = true;
    PC += (branchOffset << 1);  
    if(verbosity_level > HIGH_VERBOSITY) cout << "In BLE: branch offset = " << branchOffset << "\n";
  }

  return 0;
}

// Unsigned Conditional Branch
int BHI(instruction *inst) // Branch if higher
{
  inst->op_text = "BHI";
  inst->is_branch = true;
  uint16_t branchOffset;

  if ((inst->offset & 0x80) == 0)
  {
    branchOffset = inst->offset;
  }
  else
  {
    branchOffset = inst->offset | 0xFF00;
  }
  inst->branch_target = PC + (branchOffset << 1);
  if(!(inst->C) || !(inst->Z)) {
    inst->branch_taken = true;
    PC += (branchOffset << 1);  
  }

  return 0;
}


int BLOS(instruction *inst) // Branch if lower or same
{
  inst->op_text = "BLOS";
  inst->is_branch = true;
  uint16_t branchOffset;
  if ((inst->offset & 0x80) == 0)
  {
    branchOffset = inst->offset;
  }
  else
  {
    branchOffset = inst->offset | 0xFF00;
  }
  inst->branch_target = PC + (branchOffset << 1);
  if(inst->C || inst->Z) {
    inst->branch_taken = true;
    PC += (branchOffset << 1); 
  }

  return 0;
}

// Duplicate of BCC
int BHIS(instruction *inst) // Branch if higher or same
{
  inst->op_text = "BHIS/BCC";
  inst->is_branch = true;
  uint16_t branchOffset;
  if ((inst->offset & 0x80) == 0)
  {
    branchOffset = inst->offset;
  }
  else
  {
    branchOffset = inst->offset | 0xFF00;
  }
  inst->branch_target = PC + (branchOffset << 1);
  if(!(inst->C)) {
    inst->branch_taken = true;
    PC += (branchOffset << 1);  
  }

  return 0;
}

// Duplicate of BCS
int BLO(instruction *inst) // Branch if lower
{
  inst->op_text = "BLO/BCS";
  inst->is_branch = true;
  uint16_t branchOffset;
  if ((inst->offset & 0x80) == 0)
  {
    branchOffset = inst->offset;
  }
  else
  {
    branchOffset = inst->offset | 0xFF00;
  }
  inst->branch_target = PC + (branchOffset << 1);
  if(inst->C) {
    inst->branch_taken = true;
    PC += (branchOffset << 1); 
  }

  return 0;
}


