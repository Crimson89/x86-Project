#include "header.h"

//Don't use this anywhere. Removed from main instead of deleting. This will probably be in Max's parser/decoder.

int addr_mode(uint8_t addr_mode, uint16_t & data_in, uint16_t & data_out){
	bool is_pc = false;
	bool is_sp = false;
	uint16_t pointer_word1;
	uint16_t pointer_word2;
	uint16_t index_word;
	if(&data_in == &PC)
		is_pc = true;
	else if(&data_in == &SP)
		is_sp = true;
	if (is_pc) { // This is for PC only adddressing modes
		switch (addr_mode) {
			case PC_IMMEDIATE_MODE:
				data_out = read_word(PC);
				read_trace(PC, data_out);
				PC+=2;
				break;
			case PC_ABSOLUTE_MODE:
				pointer_word1 = read_word(PC);
				read_trace(PC, pointer_word1);
				PC+=2;
				data_out = read_word(pointer_word1);
				read_trace(pointer_word1, data_out);
				break;
			case PC_RELATIVE_MODE:
				index_word = read_word(PC);
				read_trace(PC, index_word);
				PC+=2;
				pointer_word1 = index_word + PC;
				data_out = read_word(pointer_word1);
				read_trace(pointer_word1, data_out);
				break;
			case PC_REL_DEFR_MODE:
				index_word = read_word(PC);
				read_trace(PC, index_word);
				PC+=2;
				pointer_word1 = index_word + PC;
				pointer_word2 = read_word(pointer_word1);
				read_trace(pointer_word1, pointer_word2);
				data_out = read_word(pointer_word2);
				read_trace(pointer_word2, data_out);
				break;
			default:
				cerr << "ERROR: Invalid PC addressing mode : "<< addr_mode << endl;
				return 1;
				break;
		}
	}
	else if (is_sp) { // This is for SP only adddressing modes
		switch (addr_mode) {
			case SP_DEFR_MODE:
				data_out = read_word(SP);
				read_trace(SP, data_out);
				break;
			case SP_AUTO_INC_MODE:
				data_out = read_word(SP);
				read_trace(SP, data_out);
				SP += 2;
				break;
			case SP_AUTO_INC_DEFR_MODE:
				pointer_word1 = read_word(SP); // Get the pointer at the memory location stored in this register
				read_trace(SP, pointer_word1);
				SP += 2;
				data_out= read_word(pointer_word1); // Access the pointed to memory
				read_trace(pointer_word1, data_out);
				break;
			case SP_AUTO_DEC_MODE:
				SP -= 2;
				data_out = read_word(SP);
				read_trace(SP, data_out);
				break;
			case SP_INDEX_MODE:
				index_word = read_word(SP);
				read_trace(SP, index_word);
				SP += 2;
				pointer_word1 = SP+index_word;
				data_out = read_word(pointer_word1);
				read_trace(pointer_word1, data_out);
				break;
			case SP_INDEX_DEFR_MODE:
				index_word = read_word(SP);
				read_trace(SP, index_word);
				SP += 2;
				pointer_word1 = SP+index_word;
				pointer_word2 = read_word(pointer_word1);
				read_trace(pointer_word1, pointer_word2);
				data_out = read_word(pointer_word2);
				read_trace(pointer_word2, data_out);
				break;
			default:
				cerr << "ERROR: Invalid SP addressing mode : "<< addr_mode << endl;
				return 1;
				break;
		}
	}
	else { // All other register addressing modes
		switch (addr_mode) {
			case REGISTER_MODE:
				data_out = data_in;
				break;
			case REGISTER_DEFR_MODE:
				data_out = read_word(data_in);
				read_trace(data_in, data_out);
				break;
			case AUTO_INC_MODE:
				data_out = read_word(data_in);
				read_trace(data_in, data_out);
				data_in += ((current_instruction.byteMode) ? 2:1);
				break;
			case AUTO_INC_DEFR_MODE:
				pointer_word1 = read_word(data_in); // Get the pointer at the memory location stored in this register
				read_trace(data_in, pointer_word1);
				data_in += 2;
				data_out= read_word(pointer_word1); // Access the pointed to memory
				read_trace(pointer_word1, data_out);
				break;
			case AUTO_DEC_MODE:
				data_in -= ((current_instruction.byteMode) ? 2:1);
				data_out = read_word(data_in);
				read_trace(data_in, data_out);
				break;
			case AUTO_DEC_DEFR_MODE:
				data_in -= 2;
				pointer_word1 = read_word(data_in); // Get the pointer at the memory location stored in this register
				read_trace(data_in, pointer_word1);
				data_out= read_word(pointer_word1); // Access the pointed to memory
				read_trace(pointer_word1, data_out);
				break;
			case INDEX_MODE:
				index_word = read_word(PC);
				read_trace(PC, index_word);
				PC += 2;
				pointer_word1 = data_in+index_word;
				data_out = read_word(pointer_word1);
				read_trace(pointer_word1, data_out);
				break;
			case INDEX_DEFR_MODE:
				index_word = read_word(PC);
				read_trace(PC, index_word);
				PC += 2;
				pointer_word1 = data_in+index_word;
				pointer_word2 = read_word(pointer_word1);
				read_trace(pointer_word1, pointer_word2);
				data_out = read_word(pointer_word2);
				read_trace(pointer_word2, data_out);
				break;
			default:
				cerr << "ERROR: Invalid register addressing mode : "<< addr_mode << endl;
				return 1;
				break;
		}
	}
	return 0;
}

