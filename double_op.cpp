#include "header.h"

// General Instructions
int MOV(instruction *inst) // Move source to destination (B)
{
  uint16_t temp = get_value(inst->addressingModeSrc, inst->srcBase);
  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, temp);
  }
  else
  {
    write_word(inst->addressingModeDest, inst->destBase, temp);
  }

  // Check MSB for sign
  if (inst->byteMode)
    inst->N = IS_NEGATIVE_BYTE(temp) ? 1:0;
  else
    inst->N = IS_NEGATIVE_WORD(temp)? 1:0;

  inst->Z = IS_ZERO(temp)? 1:0; // Set Zero flag if zero
  inst->V = 0; // Clear
  return 0;
}

int ADD(instruction *inst) // Add source to destination
{
  // Read values from memory
  uint16_t destAddress = get_address(inst->addressingModeDest, inst->destBase, inst->addressingModeSrc);
  cout << "destAddr" << destAddress << "\n";
  uint16_t src = get_value(inst->addressingModeSrc, inst->srcBase);
  cout << "src" << src << "\n";
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  cout << "dest" << dest << "\n";
  bool msb_dest = EXTRACT_BIT(dest,WORD_MSB_INDEX);
  bool msb_src = EXTRACT_BIT(src,WORD_MSB_INDEX);
  uint32_t temp = dest + src;

  // Do the add and store
  dest += src;
  bool msb_result = EXTRACT_BIT(dest,WORD_MSB_INDEX);
  write_word(inst->addressingModeDest, destAddress, dest);

  // Check MSB for sign
  inst->N = IS_NEGATIVE_WORD(dest)? 1:0;

  // Overflow, i.e. pos + pos = neg, neg + neg = pos
  inst->V =IS_OVERFLOW(src,dest,temp)? 1:0;

  // Set C flag if value is larger than 16 bits
  inst->C = CARRY_MSB_WORD(temp) ? 1:0;
  inst->Z = IS_ZERO(dest) ? 1:0;
  return 0;
}

int SUB(instruction *inst) // Subtract source from destination
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t src = get_value(inst->addressingModeSrc, inst->srcBase);
  bool msb_dest = EXTRACT_BIT(dest,WORD_MSB_INDEX);
  bool msb_src = EXTRACT_BIT(src,WORD_MSB_INDEX);
  uint32_t temp = dest - src;

  bool msb_result = EXTRACT_BIT(temp,WORD_MSB_INDEX);
  write_word(inst->addressingModeDest, inst->destBase, temp);

  // Set NZVC flags
  inst->N = IS_NEGATIVE_WORD(temp)? 1:0;

  // Overflow, i.e. pos - neg = neg or neg - pos = pos
  inst->V =IS_OVERFLOW(src,dest,temp)? 0:1;

  // Set C flag if there was a carry from the MSB
  inst->C = CARRY_MSB_WORD(temp) ? 0:1;
  inst->Z = IS_ZERO(temp) ? 1:0;
  return 0;
}

int CMP(instruction *inst) // Compare source to destination (B)
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t src = get_value(inst->addressingModeSrc, inst->srcBase);

  uint32_t temp = src - dest;

  inst->Z = IS_ZERO(temp) ? 1:0;

  if(inst->byteMode)
  {
	bool msb_dest   = EXTRACT_BIT(dest,BYTE_MSB_INDEX);
	bool msb_src    = EXTRACT_BIT(src,BYTE_MSB_INDEX);
    bool msb_result = EXTRACT_BIT(temp,BYTE_MSB_INDEX);

    inst->C = CARRY_MSB_BYTE(temp) ? 1:0;
    inst->N = IS_NEGATIVE_BYTE(dest)? 1:0;

    // Overflow, i.e. pos - neg = neg or neg - pos = pos
    inst->V =IS_OVERFLOW(src,dest,temp)? 1:0;

 }
  else
  {
	bool msb_dest   = EXTRACT_BIT(dest,WORD_MSB_INDEX);
	bool msb_src    = EXTRACT_BIT(src,WORD_MSB_INDEX);
    bool msb_result = EXTRACT_BIT(temp,WORD_MSB_INDEX);

    inst->C = CARRY_MSB_WORD(temp) ? 1:0;
    inst->N = IS_NEGATIVE_WORD(dest)? 1:0;

    // Overflow, i.e. pos - neg = neg or neg - pos = pos
    inst->V =IS_OVERFLOW(src,dest,temp)? 1:0;

 }
  return 0;
}

// Logical
int BIT(instruction *inst) // Bit test (B)
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t src = get_value(inst->addressingModeSrc, inst->srcBase);

  uint16_t temp = dest & src;

  // Check MSB for sign
  if (inst->byteMode)
    inst->N = IS_NEGATIVE_BYTE(temp) ? 1:0;
  else
    inst->N = IS_NEGATIVE_WORD(temp)? 1:0;

  inst->Z = IS_ZERO(temp) ? 1:0;
  inst->V = 0;
  return 0;
}

int BIC(instruction *inst) // Bit clear (B)
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t src = get_value(inst->addressingModeSrc, inst->srcBase);

  dest = (~src) & dest;
  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
  }
  else
  {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
  }

  inst->Z = IS_ZERO(dest) ? 1:0;
  inst->V = 0;
  return 0;
}

int BIS(instruction *inst) // Bit set (B)
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t src = get_value(inst->addressingModeSrc, inst->srcBase);

  dest |= src;
  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
  }
  else
  {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
  }

  inst->Z = (dest == 0)? 1:0;
  inst->Z = IS_ZERO(dest) ? 1:0;
  return 0;
}
