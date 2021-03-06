#include "header.h"

/*

Sets a breakpoint at a provided address

Arguments:
	address: uint16_t octal memory address
	
Return: 0 if breakpoint set, 1 if breakpoint already at this location

*/
int set_breakpoint(uint16_t address){
	if(BREAK_POINT[(address/2)]) {
		cout <<"Breakpoint already set @ADDR="; print_octal(address); cout << endl;
		return 0;
	}
	BREAK_POINT[(address/2)] = true;
	cout <<"Breakpoint set @ADDR="; print_octal(address); cout << endl;
	return 1;
}

/*

Clears all breakpoints

Arguments: None
	
Return: 0 if breakpoint was set and has been cleared, 1 if no breakpoints at this location

*/
int clear_all_breakpoints(void) {
	bool breakpoints = false;
	for(int i = 0; i < (MEMORY_SPACE/2); i+=1) {
		if(BREAK_POINT[i])
			breakpoints = true;
		BREAK_POINT[i] = false;
	}
	if(breakpoints) {
		cout <<"Cleared all breakpoints" << endl;
		return 0;
	}
	cout <<"No breakpoints to clear" << endl;
	return 1;	
}

/*

Clears a particular breakpoint

Arguments: address - uint16_t address to set a breakpoint at
	
Return: 0 if breakpoint was set at this location and has been cleared, 1 if no breakpoints at this location

*/
int clear_breakpoint(uint16_t address) {
	if(BREAK_POINT[(address/2)]) {
		BREAK_POINT[(address/2)] = false;
		cout <<"Clear breakpoint @ADDR="; print_octal(address); cout << endl;
		return 0;
	}
	cout <<"No breakpoint @ADDR="; print_octal(address); cout << endl;
	return 1;
}

/*
Determine if there is a breakpoint at this address

Argument:	address - uint16_t address to inspect breakpoint array for

Return: bool value indicating if breakpoint found, true indicates breakpoint, false indicates no breakpoint
*/
bool check_breakpoint(uint16_t address){
	if(BREAK_POINT[(address/2)]) {
		return true;
	}
	return false;
}

/*

The location that an instruction was fetched at was determined to be a breakpoint
Print breakpoint details
May include register print and valid memory print, always includes operation details and global PSW

Arguments:
	address - location in memory that this breakpoint is for
	instruction_code - raw 16-bit instruction value, prior to parsing and stripping arguments
	old_program_step_mode - bool indicating if the previous invication was in program step mode. Allows alterations of displayed text
	bp_print_mem  - bool indicating that the breakpoint should print memory contents if set true, if false no memory print is included
	bp_print_regs - bool indicating that the breakpoint should print register contents if set true, if false no register print is included
	
Return:
	true  - if user seleted step mode, this is used later as the new value of "old_program_step_mode" and forces a breakpoint after the next IF/decode completes
	false - Stepping mode disabled, continue normal execution

*/
bool handle_breakpoint(uint16_t address, uint16_t instruction_code, bool old_program_step_mode, bool & bp_print_mem, bool & bp_print_regs){
	string op_name;
	string mode;
	cout << "\n\n-------------------------------------------------------------------------" <<endl;
	cout <<"Hit breakpoint @ADDR="; print_octal(address); cout << endl;
	op_name = get_op_name();
	cout <<"Operation Name: \'" << op_name << "\', raw op-code value :" << octal_to_string(instruction_code) << endl;
	cout <<"Parsed Operation with Operands :" << op_formatted(current_instruction) << endl;
	cout <<"Processor Status Word :" << endl;
	print_psw(PSW) ;
	cout << "-------------------------------------------------------------------------\n\n" <<endl;
	if(bp_print_mem) {
		cout << "Print Memory Contents" << endl;
		print_all_memory();
		cout << "End of Valid Memory" << endl;
	}
	if(bp_print_regs) {
		cout << "Print Register Contents" << endl;
		print_all_registers();
		cout << "End of Registers" << endl;
	}
	cout << "-------------------------------------------------------------------------" <<endl;
	if(old_program_step_mode)
		cout << "\n\n\t\tPress C(c) to continue  normal execution, or S(s) to advance one more step" << endl;
	else
		cout << "\n\n\t\tPress C(c) to continue  normal execution, or S(s) to enter \"Single Instruction Stepping\" mode" << endl;
	cin >> mode;
	cin.get();
	if(tolower(mode.front()) == 's') {
		cout << "Stepping mode" <<endl;
		return true;
	}
	return false;
	cout << "No stepping mode" <<endl;
}

/*

Print breakpoint config. Prints whether or not breakpoints will trigger a print of system memory or a print of CPU registers

Arguments:
	bp_print_mem  - bool indicating that the breakpoint should print memory contents if set true, if false no memory print is included
	bp_print_regs - bool indicating that the breakpoint should print register contents if set true, if false no register print is included
	
Return: None

*/
void print_bp_config(bool & bp_print_mem, bool & bp_print_regs){
	if(bp_print_mem)
		cout << "Breakpoints will print all valid memory contents" << endl;
	else
		cout << "Breakpoints will NOT print all valid memory contents" << endl;
	if(bp_print_regs)
		cout << "Breakpoints will print register contents" << endl;
	else
		cout << "Breakpoints will NOT print regiser contents" << endl;
}

/*

Print all breakpoint addresses and calls print_bp_config

Arguments:
	bp_print_mem  - bool indicating that the breakpoint should print memory contents if set true, if false no memory print is included
	bp_print_regs - bool indicating that the breakpoint should print register contents if set true, if false no register print is included
	
Return: None

*/
void print_all_breakpoints(bool & bp_print_mem, bool & bp_print_regs){
for(int i = 0; i < (MEMORY_SPACE/2); i+=1)
	if(BREAK_POINT[i]) {
		cout <<"Breakpoint @ADDR="; print_octal(i*2);
		cout <<", Contents=";
		print_octal(read_word(1, i*2, false, false));
		cout << endl;
	}
	print_bp_config(bp_print_mem, bp_print_regs);
}

/*

Takes current_instruction struct opCode value and returns a string indicating the instruction mnemonic

Arguments: None - current_instruction struct is a global value
	
Return: String with operation name, or "UNKNOWN" if no match found

*/
string get_op_name(void) {
	string op_name;
	uint16_t opCode = current_instruction->opcode;
	if(opCode == m_SCCNOP) // Put masked off bits back onto SCC and CCC to make the entire operation code
		opCode|=current_instruction->offset;
	if(opCode == m_CCCNOP)
		opCode|=current_instruction->offset;
	switch(opCode){
		case m_CLR		: op_name = "CLR";          break;
		case m_CLRB		: op_name = "CLRB";         break;
		case m_COM		: op_name = "COM";          break;
		case m_COMB		: op_name = "COMB";         break;
		case m_INC		: op_name = "INC";          break;
		case m_INCB		: op_name = "INCB";         break;
		case m_DEC		: op_name = "DEC";          break;
		case m_DECB		: op_name = "DECB";         break;
		case m_NEG		: op_name = "NEG";          break;
		case m_NEGB		: op_name = "NEGB";         break;
		case m_TST		: op_name = "TST";          break;
		case m_TSTB		: op_name = "TSTB";         break;
		case m_ASR		: op_name = "ASR";          break;
		case m_ASRB		: op_name = "ASRB";         break;
		case m_ASL		: op_name = "ASL";          break;
		case m_ASLB		: op_name = "ASLB";         break;
		case m_ROR		: op_name = "ROR";          break;
		case m_RORB		: op_name = "RORB";         break;
		case m_ROL		: op_name = "ROL";          break;
		case m_ROLB		: op_name = "ROLB";         break;
		case m_SWAB		: op_name = "SWAB";         break;
		case m_ADC		: op_name = "ADC";          break;
		case m_ADCB		: op_name = "ADCB";         break;
		case m_SBC		: op_name = "SBC";          break;
		case m_SBCB		: op_name = "SBCB";         break;
		case m_SXT		: op_name = "SXT";          break;
		case m_BIT		: op_name = "BIT";          break;
		case m_BITB		: op_name = "BITB";         break;
		case m_BIC		: op_name = "BIC";          break;
		case m_BICB		: op_name = "BICB";         break;
		case m_BIS		: op_name = "BIS";          break;
		case m_BISB		: op_name = "BISB";         break;
		case m_XOR		: op_name = "XOR";          break;
		case m_MOV		: op_name = "MOV";          break;
		case m_MOVB		: op_name = "MOVB";         break;
		case m_ADD		: op_name = "ADD";          break;
		case m_SUB		: op_name = "SUB";          break;
		case m_CMP		: op_name = "CMP";          break;
		case m_CMPB		: op_name = "CMPB";         break;
		case m_ASH		: op_name = "ASH";          break;
		case m_ASHC		: op_name = "ASHC";         break;
		case m_MUL		: op_name = "MUL";          break;
		case m_DIV		: op_name = "DIV";          break;
		case m_BR		: op_name = "BR";           break;
		case m_BNE		: op_name = "BNE";          break;
		case m_BEQ		: op_name = "BEQ";          break;
		case m_BPL		: op_name = "BPL";          break;
		case m_BMI		: op_name = "BMI";          break;
		case m_BVC		: op_name = "BVC";          break;
		case m_BVS		: op_name = "BVS";          break;
		case m_BCC		: op_name = "BCC";          break;
		case m_BCS		: op_name = "BCS";          break;
		case m_BGE		: op_name = "BGE";          break;
		case m_BLT		: op_name = "BLT";          break;
		case m_BGT		: op_name = "BGT";          break;
		case m_BLE		: op_name = "BLE";          break;
		case m_SOB		: op_name = "SOB";          break;
		case m_BHI		: op_name = "BHI";          break;
		case m_BLOS		: op_name = "BLOS";         break;
		case m_JMP		: op_name = "JMP";          break;
		case m_JSR		: op_name = "JSR";          break;
		case m_RTS		: op_name = "RTS";          break;
		case m_MARK		: op_name = "MARK";         break;
		case m_EMT		: op_name = "EMT";          break;
		case m_TRAP		: op_name = "TRAP";         break;
		case m_BPT		: op_name = "BPT";          break;
		case m_IOT		: op_name = "IOT";          break;
		case m_CSM		: op_name = "CSM";          break;
		case m_RTI		: op_name = "RTI";          break;
		case m_RTT		: op_name = "RTT";          break;
		case m_HALT		: op_name = "HALT";         break;
		case m_WAIT		: op_name = "WAIT";         break;
		case m_RESET	: op_name = "RESET";        break;
		case m_MTPD		: op_name = "MTPD";         break;
		case m_MTPI		: op_name = "MTPI";         break;
		case m_MFPD		: op_name = "MFPD";         break;
		case m_MFPI		: op_name = "MFPI";         break;
		case m_MTPS		: op_name = "MTPS";         break;
		case m_MFPS		: op_name = "MFPS";         break;
		case m_TSTSET	: op_name = "TSTSET";       break;
		case m_WRTLCK	: op_name = "WRTLCK";       break;
		case m_SPL		: op_name = "SPL";          break;
		case m_CCCNOP   : op_name = "NOP"         ; break;
		case m_CLC      : op_name = "CLC"         ; break;
		case m_CLV      : op_name = "CLV"         ; break;
		case m_CLVnC    : op_name = "CLV|CLC"     ; break;
		case m_CLZ      : op_name = "CLZ"         ; break;
		case m_CLZnC    : op_name = "CLZ|CLC"     ; break;
		case m_CLZnV    : op_name = "CLZ|CLV"     ; break;
		case m_CLZnVnC  : op_name = "CLZ|CLV|CLC" ; break;
		case m_CLN      : op_name = "CLN"         ; break;
		case m_CLNnC    : op_name = "CLN|CLC"     ; break;
		case m_CLNnV    : op_name = "CLN|CLV"     ; break;
		case m_CLNnVnC  : op_name = "CLN|CLV|CLC" ; break;
		case m_CLNnZ    : op_name = "CLN|CLZ"     ; break;
		case m_CLNnZnC  : op_name = "CLN|CLZ|CLC" ; break;
		case m_CLNnZnV  : op_name = "CLN|CLZ|CLV" ; break;
		case m_CCC      : op_name = "CCC"         ; break;
		case m_SCCNOP   : op_name = "NOP"         ; break;
		case m_SEC      : op_name = "SEC"         ; break;
		case m_SEV      : op_name = "SEV"         ; break;
		case m_SEVnC    : op_name = "SEV|SEC"     ; break;
		case m_SEZ      : op_name = "SEZ"         ; break;
		case m_SEZnC    : op_name = "SEZ|SEC"     ; break;
		case m_SEZnV    : op_name = "SEZ|SEV"     ; break;
		case m_SEZnVnC  : op_name = "SEZ|SEV|SEC" ; break;
		case m_SEN      : op_name = "SEN"         ; break;
		case m_SENnC    : op_name = "SEN|SEC"     ; break;
		case m_SENnV    : op_name = "SEN|SEV"     ; break;
		case m_SENnVnC  : op_name = "SEN|SEV|SEC" ; break;
		case m_SENnZ    : op_name = "SEN|SEZ"     ; break;
		case m_SENnZnC  : op_name = "SEN|SEZ|SEC" ; break;
		case m_SENnZnV  : op_name = "SEN|SEZ|SEV" ; break;
		case m_SCC      : op_name = "SCC"         ; break;
		default			: op_name = "UNKNOWN"     ; break;
	}
	return op_name;
}

/*

Parses current_instruction struct values and regenerates a single instruction argument with mode syntax

Arguments:
	reg - the register number for this argument
	mode - the addressing mode, 0 - 7
	immediate - the immediate value of this instruction, if there is one. 0 if there isn't.
	
Return: Single argument string with mode syntax

*/
string format_arg(uint8_t reg, uint8_t mode, uint16_t immediate) {
	stringstream temp;
	stringstream temp_reg;
	if (reg <= 5) {
		temp_reg << "R" << to_string(reg);
		switch(mode){
				case 0: temp << temp_reg.str();                                                      break;
				case 1: temp << "("       << temp_reg.str()      << ")";                             break;
				case 2: temp << "("       << temp_reg.str()      << ")+";                            break;
				case 3: temp << "@("      << temp_reg.str()      << ")+";                            break;
				case 4: temp << "-("      << temp_reg.str()      << ")";                             break;
				case 5: temp << "@-("     << temp_reg.str()      << ")";                             break;
				case 6: temp << immediate << "("             << temp_reg.str() << ")";               break;
				case 7: temp << "@"       << immediate       << "("        << temp_reg.str() << ")"; break;
		}
	}
	else if(reg == 6) {
		switch(mode){
				case 0: temp << "SP";                                                            break;
				case 1: temp << "("       << "SP"      << ")";                                   break;
				case 2: temp << "("       << "SP"      << ")+";                                  break;
				case 3: temp << "@("      << "SP"      << ")+";                                  break;
				case 4: temp << "-("      << "SP"      << ")";                                   break;
				case 5: temp << "@-("     << "SP"      << ")";                                   break;
				case 6: temp << oct       << immediate << "("       << "SP" << ")";              break;
				case 7: temp << "@"       << oct       << immediate << "("  << "SP" << ")";      break;
		}                                                                                        
	}                                                                                            
	else if(reg == 7) {                                                                          
		switch(mode){                                                                            
				case 0: temp << "PC";                                                            break;
				case 1: temp << "("       << "PC"      << ")";                                   break;
				case 2: temp << "("       << "PC"      << ")+";                                  break;
				case 3: temp << "@("      << "PC"      << ")+";                                  break;
				case 4: temp << "-("      << "PC"      << ")";                                   break;
				case 5: temp << "@-("     << "PC"      << ")";                                   break;
				case 6: temp << oct       << immediate << "("       << "PC" << ")";              break;
				case 7: temp << "@"       << oct       << immediate << "("  << "PC" << ")";      break;
		}
	}
	else
		return " ";
	return temp.str();
}

/*

Parses current_instruction struct values and regenerates a single instruction mnemonic with arguments with mode syntax

Arguments:
	op - pointer to the instruction struct containing the information for this particular operation

Return: String with operation mnumonic and argument strings with mode syntax, "??????" if no instruction matched

*/
string op_formatted(instruction * op) {
	stringstream temp;
	string byteMode = " ";
	uint16_t opCode = current_instruction->opcode;
	temp << get_op_name();
	if(opCode == m_SCCNOP) // Put masked off bits back onto SCC and CCC to make the entire operation code
		opCode|=current_instruction->offset;
	if(opCode == m_CCCNOP)
		opCode|=current_instruction->offset;
	if(op->byteMode) // If this isn't a byteMode operation then append " ", if it is, append "B "
		byteMode = "B ";
	switch(opCode){
						//Single Operand
		case m_CLR    :
		case m_CLRB   :
		case m_COM    :
		case m_COMB   :
		case m_INC    :
		case m_INCB   :
		case m_DEC    :
		case m_DECB   :
		case m_NEG    :
		case m_NEGB   :
		case m_TST    :
		case m_TSTB   :
		case m_ASR    :
		case m_ASRB   :
		case m_ASL    :
		case m_ASLB   :
		case m_JMP    :
		case m_ROR    :
		case m_RORB   :
		case m_ROL    :
		case m_ROLB   :
		case m_SWAB   :
		case m_ADC    :
		case m_ADCB   :
		case m_SBC    :
		case m_SBCB   :
		case m_SXT    :
		case m_XOR    : temp << byteMode << 
						format_arg(op->regBase,op->addressingModeReg,op->immediate);
						break;
							// Double Operand Group
		case m_BIT    :
		case m_BITB   :
		case m_BIC    :
		case m_BICB   :
		case m_BIS    :
		case m_BISB   :
		case m_MOV    :
		case m_MOVB   :
		case m_ADD    :
		case m_SUB    :
		case m_CMP    :
		case m_CMPB   :	temp << byteMode << 
						format_arg(op->srcBase,op->addressingModeSrc,op->immediate)
						<< ", " << 
						format_arg(op->destBase,op->addressingModeDest,op->immediate); 
						break;
							//Program Control Group
		case m_BR     :
		case m_BNE    :
		case m_BEQ    :
		case m_BPL    :
		case m_BMI    :
		case m_BVC    :
		case m_BVS    :
		case m_BCC    :
		case m_BCS    :
		case m_BGE    :
		case m_BLT    :
		case m_BGT    :
		case m_BLE    :
		case m_BHI    :
		case m_BLOS   : temp << " " << octal_to_string(op->offset); 
						break;
							// JSR
		case m_JSR    :	temp << " " << 
						format_arg(op->regBase,op->addressingModeReg,op->immediate)
						<< ", " << 
						format_arg(op->destBase,op->addressingModeDest,op->immediate); 
						break;
							// RTS
		case m_RTS    :
						if     (op->rtsReg <= 5)
							temp << " R" << to_string(op->rtsReg);
						else if(op->rtsReg == 6)
							temp << " SP"; 
						else if(op->rtsReg == 7)
							temp << " PC"; 
						break;
							// Mark
		case m_MARK   :temp << " " << "VALUE"; break; // Cheating here, we didn't implement this, so I use "value" for the MARK "NM" value
							// SOB
		case m_SOB    :temp << " Rn VALUE"; break; // Cheating here, we didn't implement this, so I use dummy values for the SOB "NM" and R values
							// Trap, Operate Group, Condition Code Operators, and Other stuff
		case m_EMT    :
		case m_TRAP   :
		case m_BPT    :
		case m_IOT    : 
		case m_HALT   :
		case m_WAIT   :
		case m_RESET  :
		case m_RTI    :
		case m_RTT    :
		case m_MTPD   :
		case m_MTPI   :
		case m_MFPD   :
		case m_MFPI   :
		case m_MTPS   :
		case m_MFPS   :
		case m_MFPT   :
		case m_TSTSET :
		case m_WRTLCK :
		case m_SPL    :
		case m_CSM    : 
		case m_CCCNOP :// Condition codes
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
		case m_SCC    : break;  //Nothing to do, just leave temp as it is and return the operation name
							// FP Operations
		case m_ASH   :
		case m_ASHC  :
		case m_MUL   :
		case m_DIV   : temp << " Rn"; break; // Cheating here, we didn't implement this, so I use dummy value for the FP op R values
		default      : if(verbosity_level > HIGH_VERBOSITY) cerr << "Debug operation failed, unknown masked opCode "<< opCode << endl; temp << "?????";   break;
	}
	return temp.str();
}
