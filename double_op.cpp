#include "header.h"

// General Instructions
int MOV(instruction *inst) // Move source to destination (B)
{
  if(inst->byteMode)
  {
    uint16_t a_src = read_byte(inst->src);
    write_byte(inst->dest, a_src);
  }
  else
  {
    uint16_t a_src = read_word(inst->src);
    write_word(inst->dest, a_src);
  }
  uint16_t v_src = get_value(inst->src);
  inst->N = (v_src < 0)? 1:0; // Set Negative flag if below zero
  inst->Z = (v_src == 0)? 1:0; // Set Zero flag if zero
  inst->V = 0; // Clear
  return 0;
}

int ADD(instruction *inst) // Add source to destination
{
  // Read values from memory
  uint16_t v_dest = get_value(0, inst->dest);
  uint16_t v_src = get_value(0, inst->src);
  uint16_t temp = v_dest;

  // Do the operation and write to memory
  v_dest += v_src;
  set_value(v_dest, inst->dest);

  // Set NZVC flags
  inst->N = (v_dest < 0)? 1:0;
  inst->Z = (v_dest == 0)? 1:0;

  // Overflow, i.e. pos + pos = neg, neg + neg = pos
  inst->V = ((temp > 0 && v_src > 0 && v_dest < 0) || (temp < 0 && v_src < 0 && v_dest > 0))? 1:0;

  // Set C flag if there was a carry to the MSB
  inst->C = (v_dest > 0xFFFF) ? 1:0;
  return 0;
}

int SUB(instruction *inst) // Subtract source from destination
{
  uint16_t v_dest = get_value(0, inst->dest);
  uint16_t v_src = get_value(0, inst->src);
  uint16_t temp = v_dest;

  v_dest -= v_src;
  set_value(v_dest, inst->dest);

  // Set NZVC flags
  inst->N = (v_dest < 0)? 1:0;
  inst->Z = (v_dest == 0)? 1:0;

  // Overflow, i.e. pos - neg = neg or neg - pos = pos
  inst->V = ((temp > 0 && v_src < 0 && v_dest < 0) || (temp < 0 && v_src > 0 && v_dest > 0))? 1:0;

  // Set C flag if there was a carry from the MSB
  inst->C = (((temp > 0xFFFF) || (v_src > 0xFFFF)) && (v_dest <= 0xFFFF))? 1:0;
  return 0;
}

int CMP(instruction *inst) // Compare source to destination (B)
{
  uint16_t v_dest = get_value(inst->byteMode, inst->dest);
  uint16_t v_src = get_value(inst->byteMode, inst->src);

  uint16_t temp = v_src - v_dest;
  inst->N = (temp < 0)? 1:0;
  inst->Z = (temp == 0)? 1:0;

  // Overflow, i.e. pos - neg = neg or neg - pos = pos
  inst->V = ((v_src > 0 && v_dest < 0 && temp < 0) || (v_src < 0 && v_dest > 0 && temp > 0))? 1:0;

  // Set C flag if there was a carry from the MSB
  if(inst->byteMode)
    inst->C = (((temp > 0xFF) || (v_src > 0xFF)) && (v_dest <= 0xFF))? 1:0;
  else
    inst->C = (((temp > 0xFFFF) || (v_src > 0xFFFF)) && (v_dest <= 0xFFFF))? 1:0;
  return 0;
}

// Logical
int BIT(instruction *inst) // Bit test (B)
{
  uint16_t v_dest = get_value(inst->byteMode, inst->dest);
  uint16_t v_src = get_value(inst->byteMode, inst->src);

  uint16_t temp = v_dest && v_src;
  inst->N = (temp < 0)? 1:0;
  inst->Z = (temp == 0)? 1:0;
  inst->V = 0;
  return 0;
}

int BIC(instruction *inst) // Bit clear (B)
{
  uint16_t v_dest = get_value(inst->byteMode, inst->dest);
  uint16_t v_src = get_value(inst->byteMode, inst->src);

  v_dest = ~(v_src) && v_dest;
  set_value(v_dest, inst->dest);

  inst->N = (v_dest < 0)? 1:0;
  inst->Z = (v_dest == 0)? 1:0;
  inst->V = 0;
  return 0;
}

int BIS(instruction *inst) // Bit set (B)
{
  uint16_t v_dest = get_value(inst->byteMode, inst->dest);
  uint16_t v_src = get_value(inst->byteMode, inst->src);

  v_dest = v_src || v_dest;
  set_value(v_dest, inst->dest);

  inst->N = (v_dest < 0)? 1:0;
  inst->Z = (v_dest == 0)? 1:0;
  inst->V = 0;
  return 0;
}
