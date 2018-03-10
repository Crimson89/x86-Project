#include "header.h"

// General Instructions
int MOV(instruction *inst) // Move source to destination (B)
{
  if(inst->byteMode)
  {
    uint16_t src = read_byte(inst->addressingModeSrc, inst->srcBase);
    write_byte(inst->addressingModeDest, inst->destBase, src);
  }
  else
  {
    uint16_t src = read_word(inst->addressingModeSrc, inst->srcBase);
    write_word(inst->addressingModeDest, inst->destBase, src);
  }
  uint16_t temp = get_value(inst->addressingModeSrc, inst->srcBase);
  inst->N = (temp < 0)? 1:0; // Set Negative flag if below zero
  inst->Z = (temp == 0)? 1:0; // Set Zero flag if zero
  inst->V = 0; // Clear
  return 0;
}

int ADD(instruction *inst) // Add source to destination
{
  // Read values from memory
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t src = get_value(inst->addressingModeSrc, inst->srcBase);
  uint16_t msb_dest = (dest && 0x8000);
  uint16_t msb_src = (src && 0x8000);
  uint32_t temp = dest + src;

  dest += src;
  uint16_t msb_result = (dest && 0x8000);
  write_word(inst->addressingModeDest, inst->destBase, dest);

  // Check if MSB is 1 for negative
  inst->N = (msb_result == 0x8000)? 1:0;

  // Overflow, i.e. pos + pos = neg, neg + neg = pos
  inst->V =(((msb_dest == 0x0000) && (msb_src == 0x0000) && (msb_result == 0x8000)) || ((msb_dest == 0x8000) && (msb_src == 0x8000) && (msb_result == 0x0000)))? 1:0;

  // Set C flag if there was a carry to the MSB
  inst->C = (temp > 0xFFFF) ? 1:0;
  inst->Z = (temp == 0) ? 1:0;
  return 0;
}

int SUB(instruction *inst) // Subtract source from destination
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t src = get_value(inst->addressingModeSrc, inst->srcBase);
  uint16_t temp = dest;

  dest -= src;
  write_word(inst->addressingModeDest, inst->destBase, dest);

  // Set NZVC flags
  inst->N = (dest < 0)? 1:0;
  inst->Z = (dest == 0)? 1:0;

  // Overflow, i.e. pos - neg = neg or neg - pos = pos
  inst->V = ((temp > 0 && src < 0 && dest < 0) || (temp < 0 && src > 0 && dest > 0))? 1:0;

  // Set C flag if there was a carry from the MSB
  inst->C = (((temp > 0xFFFF) || (src > 0xFFFF)) && (dest <= 0xFFFF))? 1:0;
  return 0;
}

int CMP(instruction *inst) // Compare source to destination (B)
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t src = get_value(inst->addressingModeSrc, inst->srcBase);

  uint16_t temp = src - dest;
  inst->N = (temp < 0)? 1:0;
  inst->Z = (temp == 0)? 1:0;

  // Overflow, i.e. pos - neg = neg or neg - pos = pos
  inst->V = ((src > 0 && dest < 0 && temp < 0) || (src < 0 && dest > 0 && temp > 0))? 1:0;

  // Set C flag if there was a carry from the MSB
  if(inst->byteMode)
    inst->C = (((temp > 0xFF) || (src > 0xFF)) && (dest <= 0xFF))? 1:0;
  else
    inst->C = (((temp > 0xFFFF) || (src > 0xFFFF)) && (dest <= 0xFFFF))? 1:0;
  return 0;
}

// Logical
int BIT(instruction *inst) // Bit test (B)
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t src = get_value(inst->addressingModeSrc, inst->srcBase);

  uint16_t temp = dest && src;
  inst->N = (temp < 0)? 1:0;
  inst->Z = (temp == 0)? 1:0;
  inst->V = 0;
  return 0;
}

int BIC(instruction *inst) // Bit clear (B)
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t src = get_value(inst->addressingModeSrc, inst->srcBase);

  dest = ~(src) && dest;
  if(inst->byteMode)
    write_byte(inst->addressingModeDest, inst->destBase, dest);
  else
    write_word(inst->addressingModeDest, inst->destBase, dest);

  inst->N = (dest < 0)? 1:0;
  inst->Z = (dest == 0)? 1:0;
  inst->V = 0;
  return 0;
}

int BIS(instruction *inst) // Bit set (B)
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t src = get_value(inst->addressingModeSrc, inst->srcBase);

  dest = src || dest;
  if(inst->byteMode)
    write_byte(inst->addressingModeDest, inst->destBase, dest);
  else
    write_word(inst->addressingModeDest, inst->destBase, dest);

  inst->N = (dest < 0)? 1:0;
  inst->Z = (dest == 0)? 1:0;
  inst->V = 0;
  return 0;
}
