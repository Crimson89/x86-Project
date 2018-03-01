#include "header.h"

// General
int MOV(instruction *inst) // Move source to destination (B)
{
  inst->dest = (byteMode)? (inst->src&0x00FF):inst->src;
  inst->N = (inst->src < 0)? 1:0; // Set Negative flag if below zero
  inst->Z = (inst->src == 0)? 1:0; // Set Zero flag if zero
  inst->V = 0; // Clear
  return 0;
}

int ADD(instruction *inst) // Add source to destination
{
  int temp = inst->dest;
  inst->dest = inst->dest + inst->src; // Do the add
  inst->N = (inst->dest < 0)? 1:0;
  inst->Z = (inst->dest == 0)? 1:0;

  // Overflow, i.e. pos + pos = neg
  inst->V = ((temp > 0 && inst->src > 0 && inst->dest < 0) || (temp < 0 && inst->src < 0 && inst->dest > 0))? 1:0;

  // Set C flag if there was a carry from the MSB
  inst->C = (temp && 0x00010000) ? 1:0;
  return 0;
}

int SUB(instruction *inst) // Subtract source from destination
{
  int temp = inst->dest;
  inst->dest = inst->dest - inst->src;
  inst->N = (inst->dest < 0)? 1:0;
  inst->Z = (inst->dest == 0)? 1:0;

  // Overflow, i.e. pos - neg = neg or neg - pos = pos
  inst->V = ((temp > 0 && inst->src < 0 && inst->dest < 0) || (temp < 0 && inst->src > 0 && inst->dest > 0))? 1:0;

  // Set C flag if there was a carry from the MSB
  // inst->C = ()? 1:0;
  return 0;
}

int CMP(instruction *inst) // Compare source to destination (B)
{
  int temp = inst->src - inst->dest;
  inst->N = (temp < 0)? 1:0;
  inst->Z = (temp == 0)? 1:0;

  // Overflow, i.e. pos - neg = neg or neg - pos = pos
  inst->V = ((inst->src > 0 && inst->dest < 0 && temp < 0) || (inst->src < 0 && inst->dest > 0 && temp > 0))? 1:0;

  // Set C flag if there was a carry from the MSB
  // inst->C = ()? 1:0;
  return 0;
}

int ASH(instruction *inst) // Shift arithmetically
{
}

int ASHC(instruction *inst) // Arithmetic shift combined
{
}

int MUL(instruction *inst) // Multiply
{
}

int DIV(instruction *inst) // Divide
{
}

// Logical
int BIT(instruction *inst) // Bit test (B)
{
  int temp = inst->dest && inst->src;
  inst->N = (temp < 0)? 1:0;
  inst->Z = (temp == 0)? 1:0;
  inst->V = 0;
  return 0;
}

int BIC(instruction *inst) // Bit clear (B)
{
  inst->dest = ~(inst->src) && inst->dest;
  inst->N = (inst->dest < 0)? 1:0;
  inst->Z = (inst->dest == 0)? 1:0;
  inst->V = 0;
  return 0;
}

int BIS(instruction *inst) // Bit set (B)
{
  inst->dest = inst->src || inst->dest;
  inst->N = (inst->dest < 0)? 1:0;
  inst->Z = (inst->dest == 0)? 1:0;
  inst->V = 0;
  return 0;
}

int XOR(instruction *inst) // Exclusive OR
{
}
