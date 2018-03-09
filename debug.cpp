#include "header.h"

int set_breakpoint(uint16_t address){
	if(BREAK_POINT[(address/2)]) {
		cout <<"Breakpoint already set @ADDR="; print_octal(address); cout << endl;
		return 0;
	}
	BREAK_POINT[(address/2)] = true;
	cout <<"Breakpoint set @ADDR="; print_octal(address); cout << endl;
	return 1;
}

int clear_all_breakpoints(void) {
	bool breakpoints = false;
	for(int i = 0; i < (MEMORY_SPACE/2); i+=1) {
		if(BREAK_POINT[i])
			breakpoints = true;
		BREAK_POINT[i] = false;
	}
	if(breakpoints) {
		cout <<"Cleared all breakpoints" << endl;
		return 1;
	}
	cout <<"No breakpoints to clear" << endl;
	return 0;	
}

int clear_breakpoint(uint16_t address) {
	if(BREAK_POINT[(address/2)]) {
		BREAK_POINT[(address/2)] = false;
		cout <<"Clear breakpoint @ADDR="; print_octal(address); cout << endl;
		return 1;
	}
	cout <<"No breakpoint @ADDR="; print_octal(address); cout << endl;
	return 0;
}

bool check_breakpoint(uint16_t address){
	if(BREAK_POINT[(address/2)]) {
		return true;
	}
	return false;
}

void handle_breakpoint(uint16_t address, uint16_t instruction_code){
	string op_name;
	cout << "\n\n-------------------------------------------------------------------------" <<endl;
	cout <<"Hit breakpoint @ADDR="; print_octal(address); cout << endl;
	op_name = get_op_name();
	cout <<"Operation: \'" << op_name << "\', raw op-code value :" << octal_to_string(instruction_code) << endl;
	cout << "-------------------------------------------------------------------------\n\n" <<endl;
	cout << "Print Memory Contents" << endl;
	print_all_memory();
	cout << "End of Valid Memory" << endl;
	cout << "Print Register Contents" << endl;
	print_all_registers();
	cout << "End of Registers" << endl;
	cout << "\n\n                      Press ENTER to continue" << endl;
	cout << "-------------------------------------------------------------------------" <<endl;
	cin.get();
}

void print_all_breakpoints(void){
for(int i = 0; i < (MEMORY_SPACE/2); i+=1)
	if(BREAK_POINT[i]) {
		cout <<"Breakpoint @ADDR="; print_octal(i*2);
		cout <<", Contents=";
		print_octal(read_word(i*2, false, false));
		cout << endl;
	}
}

string get_op_name(void) {
	string op_name;
	switch(current_instruction->opcode){
		case m_CLR		: op_name = "CLR";     break;
		case m_CLRB		: op_name = "CLRB";    break;
		case m_COM		: op_name = "COM";     break;
		case m_COMB		: op_name = "COMB";    break;
		case m_INC		: op_name = "INC";     break;
		case m_INCB		: op_name = "INCB";    break;
		case m_DEC		: op_name = "DEC";     break;
		case m_DECB		: op_name = "DECB";    break;
		case m_NEG		: op_name = "NEG";     break;
		case m_NEGB		: op_name = "NEGB";    break;
		case m_NOP		: op_name = "NOP";     break;
		case m_TST		: op_name = "TST";     break;
		case m_TSTB		: op_name = "TSTB";    break;
		case m_ASR		: op_name = "ASR";     break;
		case m_ASRB		: op_name = "ASRB";    break;
		case m_ASL		: op_name = "ASL";     break;
		case m_ASLB		: op_name = "ASLB";    break;
		case m_ROR		: op_name = "ROR";     break;
		case m_RORB		: op_name = "RORB";    break;
		case m_ROL		: op_name = "ROL";     break;
		case m_ROLB		: op_name = "ROLB";    break;
		case m_SWAB		: op_name = "SWAB";    break;
		case m_ADC		: op_name = "ADC";     break;
		case m_ADCB		: op_name = "ADCB";    break;
		case m_SBC		: op_name = "SBC";     break;
		case m_SBCB		: op_name = "SBCB";    break;
		case m_SXT		: op_name = "SXT";     break;
		case m_BIT		: op_name = "BIT";     break;
		case m_BITB		: op_name = "BITB";    break;
		case m_BIC		: op_name = "BIC";     break;
		case m_BICB		: op_name = "BICB";    break;
		case m_BIS		: op_name = "BIS";     break;
		case m_BISB		: op_name = "BISB";    break;
		case m_XOR		: op_name = "XOR";     break;
		case m_MOV		: op_name = "MOV";     break;
		case m_MOVB		: op_name = "MOVB";    break;
		case m_ADD		: op_name = "ADD";     break;
		case m_SUB		: op_name = "SUB";     break;
		case m_CMP		: op_name = "CMP";     break;
		case m_CMPB		: op_name = "CMPB";    break;
		case m_ASH		: op_name = "ASH";     break;
		case m_ASHC		: op_name = "ASHC";    break;
		case m_MUL		: op_name = "MUL";     break;
		case m_DIV		: op_name = "DIV";     break;
		case m_BR		: op_name = "BR";      break;
		case m_BNE		: op_name = "BNE";     break;
		case m_BEQ		: op_name = "BEQ";     break;
		case m_BPL		: op_name = "BPL";     break;
		case m_BMI		: op_name = "BMI";     break;
		case m_BVC		: op_name = "BVC";     break;
		case m_BVS		: op_name = "BVS";     break;
		case m_BCC		: op_name = "BCC";     break;
		case m_BCS		: op_name = "BCS";     break;
		case m_BGE		: op_name = "BGE";     break;
		case m_BLT		: op_name = "BLT";     break;
		case m_BGT		: op_name = "BGT";     break;
		case m_BLE		: op_name = "BLE";     break;
		case m_SOB		: op_name = "SOB";     break;
		case m_BHI		: op_name = "BHI";     break;
		case m_BLOS		: op_name = "BLOS";    break;
		case m_JMP		: op_name = "JMP";     break;
		case m_JSR		: op_name = "JSR";     break;
		case m_RTS		: op_name = "RTS";     break;
		case m_MARK		: op_name = "MARK";    break;
		case m_EMT		: op_name = "EMT";     break;
		case m_TRAP		: op_name = "TRAP";    break;
		case m_BPT		: op_name = "BPT";     break;
		case m_IOT		: op_name = "IOT";     break;
		case m_CSM		: op_name = "CSM";     break;
		case m_RTI		: op_name = "RTI";     break;
		case m_RTT		: op_name = "RTT";     break;
		case m_HALT		: op_name = "HALT";    break;
		case m_WAIT		: op_name = "WAIT";    break;
		case m_RESET	: op_name = "RESET";   break;
		case m_MTPD		: op_name = "MTPD";    break;
		case m_MTPI		: op_name = "MTPI";    break;
		case m_MFPD		: op_name = "MFPD";    break;
		case m_MFPI		: op_name = "MFPI";    break;
		case m_MTPS		: op_name = "MTPS";    break;
		case m_MFPS		: op_name = "MFPS";    break;
		case m_SPL		: op_name = "SPL";     break;
		case m_CLC		: op_name = "CLC";     break;
		case m_CLV		: op_name = "CLV";     break;
		case m_CLZ		: op_name = "CLZ";     break;
		case m_CLN		: op_name = "CLN";     break;
		case m_CCC		: op_name = "CCC";     break;
		case m_SEC		: op_name = "SEC";     break;
		case m_SEV		: op_name = "SEV";     break;
		case m_SEZ		: op_name = "SEZ";     break;
		case m_SEN		: op_name = "SEN";     break;
		case m_SCC		: op_name = "SCC";     break;
		default			: op_name = "UNKNOWN"; break;
	}
	return op_name;
}