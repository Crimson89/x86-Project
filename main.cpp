#include "header.h"

uint16_t REGS[7];                  //CPU Registers
uint8_t MEM[MEMORY_SPACE];         //Byte memory array, index is memory address
bool MEM_USED_FLAGS[MEMORY_SPACE]; //Memory used array, true==memory used/valid, false==invalid
bool BREAK_POINT[MEMORY_SPACE/2];    //Breakpoint address, when PC == this value the program pauses
uint16_t& R0 = REGS[0];
uint16_t& R1 = REGS[1];
uint16_t& R2 = REGS[2];
uint16_t& R3 = REGS[3];
uint16_t& R4 = REGS[4];
uint16_t& R5 = REGS[5];
uint16_t& R6 = REGS[6];
uint16_t& R7 = REGS[7];
uint16_t& SP = REGS[6];
uint16_t& PC = REGS[7];
uint16_t starting_pc;
instruction * current_instruction;	// decoded instruction information
int verbosity_level;                // Level of verbosity in print statements
string trace_file;
string data_file;
string branch_trace_file;
PSW_t PSW;


int main(int argc, char ** argv)
{
	verbosity_level = 0; // Level of verbosity in print statements, default to low 
	int program_execution_control = 0;
	int err;							// error checking
	uint16_t instruction_code;			// 16-bit instruction
	bool at_breakpoint = false;         // Current PC triggered breakpoint
	bool program_step_mode = false;     // Stepping mode enabled, breakpoint at each new instruction
	uint16_t breakpoint_pc;             // PC when breakpoint was triggered
	int num_instructions_executed = 0;  // The number of instructions executed by this program
	
	trace_file = "test_trace.txt";
	data_file = "FALSE";
	branch_trace_file = "FALSE";
	SP = 0xFFFE; // Assign at start to invalid value, detection of unassigned SP
	PC = 0xFFFE; // Assign at start to invalid value, detection of unassigned PC
	starting_pc = PC;
	initializeMemory();
	get_cmd_options(argc, argv, branch_trace_file, data_file, trace_file); // Read command line options
	current_instruction = new instruction;
	clearInstruction(current_instruction);
	bool bp_print_mem = false;  // Hitting a breakpoint prints all valid memory contents
	bool bp_print_regs = false; // Hitting a breakpoint prints all register contents 
	clear_psw(PSW);
	
	
	for(;;){
		program_execution_control = menu_function(bp_print_mem, bp_print_regs, trace_file);
		if(program_execution_control == LOAD_DATA) {
			cout << "Data File " << data_file << endl;
			if(readData(data_file)) {
				cerr << "failed to open file : " << data_file << endl;
			}
			program_execution_control = PRINT_MENU;
		}
		else if((program_execution_control == RUN_PROGRAM) && (PC != 0xFFFF)) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "                     Press ENTER to begin program" << endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			cin.get();
			cout << "                            Executing program" <<endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			program_step_mode = false;
			num_instructions_executed = 0;

			// Main program loop
			while(program_execution_control == RUN_PROGRAM) {

				// IF and evaluate current PC for matching breakpoint, then increment PC
				if(instruction_fetch(at_breakpoint, instruction_code, breakpoint_pc, PC)) {
					cerr << "\n\n-------------------------------------------------------------------------" <<endl;
					cerr << "-------------------------------------------------------------------------" <<endl;
					cerr << "\n\n\t\tInstruction Fetch Fault, Terminating Program!!\n\n" << endl;
					cerr << "\t\t\tPress ENTER to return to menu" << endl;
					cerr << "-------------------------------------------------------------------------" <<endl;
					cerr << "-------------------------------------------------------------------------\n\n" <<endl;
					program_execution_control = PRINT_MENU;
					program_step_mode = false;
					cin.get();
					break;
				}

				// ID
				if(parseInstruction(instruction_code, current_instruction)) {
					cerr << "\n\n-------------------------------------------------------------------------" <<endl;
					cerr << "-------------------------------------------------------------------------" <<endl;
					cerr << "\n\n\t\tInstruction Decode Fault, Terminating Program!!\n\n" << endl;
					cerr << "\t\t\tPress ENTER to return to menu" << endl;
					cerr << "-------------------------------------------------------------------------" <<endl;
					cerr << "-------------------------------------------------------------------------\n\n" <<endl;
					program_execution_control = PRINT_MENU;
					program_step_mode = false;
					cin.get();
					break;
				}
				
				// If this is a breakpoint, then print breakpoint information
				if(at_breakpoint || program_step_mode){
					program_step_mode = handle_breakpoint(breakpoint_pc, instruction_code, program_step_mode, bp_print_mem, bp_print_regs);
					at_breakpoint = false;
				}
				else if(verbosity_level >= HIGH_VERBOSITY) {
					cout << "Instruction: " << op_formatted(current_instruction) << endl;
					cout << "Current Operation PSW:" << oct << uint16_t(current_instruction->PSW) << endl;
					cout << "Print Register Contents" << endl;
					print_all_registers();
					cout << "End of Registers" << endl;
				}

				// EX
				if(dispatch(current_instruction)){
					cerr << "\n\n-------------------------------------------------------------------------" <<endl;
					cerr << "-------------------------------------------------------------------------" <<endl;
					cerr << "\n\n\t\tInstruction Execution/Dispatch Fault\n\n" << endl;
					cerr << "\t\t\tPress ENTER to continue" << endl;
					cerr << "-------------------------------------------------------------------------" <<endl;
					cerr << "-------------------------------------------------------------------------\n\n" <<endl;
					program_step_mode = false;
					cin.get();
					break;
				}

				// WB
				if(write_back(PSW, current_instruction)) {
					cerr << "\n\n-------------------------------------------------------------------------" <<endl;
					cerr << "-------------------------------------------------------------------------" <<endl;
					cerr << "\n\n   PSW Write Back Fault, Terminating Program!!\n\n" << endl;
					cerr << "              Press ENTER to return to menu" << endl;
					cerr << "-------------------------------------------------------------------------" <<endl;
					cerr << "-------------------------------------------------------------------------\n\n" <<endl;
					program_execution_control = PRINT_MENU;
					program_step_mode = false;
					cin.get();
				}
				
				if(current_instruction->opcode == m_HALT) {
					program_step_mode = false;
					program_execution_control = PRINT_MENU;
				}
				
				num_instructions_executed++;
				clearInstruction(current_instruction);
			}
			cout << "-------------------------------------------------------------------------" <<endl;
			cout << "                            Program Completed!" <<endl;
			cout << "                          Executed " << dec << num_instructions_executed << " instructions." <<endl;
			cout << "                     Press ENTER to return to menu" << endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			cin.get();
		}
		else {
			cerr << "-------------------------------------------------------------------------" <<endl;
			cerr << "              Unhandled condition, is PC set to valid address?!" <<endl;
			cerr << "                 Press ENTER to return to menu" << endl;
			cerr << "-------------------------------------------------------------------------" <<endl;
			cin.get();
			program_execution_control = PRINT_MENU;
			program_step_mode = false;
		}
	}
	delete current_instruction;
	return 0; // never hit, program exits from menu function
}

void get_user_octal(string prompt, string error_text, uint16_t &word){
	string input = "FALSE";
	string temp = "FALSE";
	while(input == "FALSE") {
		cout << prompt;
		cin >> input;
		for(int i = 0; i < input.length(); i++) {            //Iterate through read line and copy out characters that are valid octal
			if (isxdigit(input[i]) && ((input[i]-48) < 8)) { //Valid octal character
				if(temp.compare("FALSE") == 0)               //If this is the first valid character
					temp = input[i];
				else
					temp += input[i];
			}
		}
		if( temp.compare("FALSE") != 0 && (temp.length() == 6) ) {
			word = string_to_octal(temp);
		}
		else {
			cerr << error_text << endl;
			input = "FALSE";
			temp  = "FALSE";
		}
	}
}


int menu_function(bool & bp_print_mem, bool & bp_print_regs, string & trace_file) {
	string input;
	char input_char;
	int menu_continue = 1;
	uint16_t bp_address;
	while(menu_continue) {
		cout << "-------------------------------------------------------------------------" <<endl;
		cout << "-------------------------------------------------------------------------" <<endl;
		cout << "MENU: Make selection from choices below" << endl;
		cout << "Q(q) to quit" << endl;
		cout << "R(r) to Restart current application at initial PC" << endl;
		cout << "S(s) to manually set PC, (Will not automatically start program)" << endl;
		cout << "E(e) to start application at current PC" << endl;
		cout << "P(p) to print all register contents" << endl;
		cout << "M(m) to print all valid memory contents" << endl;
		cout << "L(l) to load new application" << endl;
		cout << "T(t) to print current trace file" << endl;
		cout << "Y(y) to clear old trace file" << endl;
		cout << "B(b) to set break point" << endl;
		cout << "C(c) to clear breakpoint" << endl;
		cout << "V(v) to clear all breakpoints" << endl;
		cout << "N(n) to print all breakpoints" << endl;
		cout << "Z(z) to toggle \"breakpoints print register contents\" option" << endl;
		cout << "X(x) to toggle \"breakpoints print all memory contents\" option" << endl;
		cout << "-------------------------------------------------------------------------" <<endl;
		cout << "-------------------------------------------------------------------------" <<endl;
		cout << "\n\nSelection: ";
		cin >> input;
		input_char = tolower(input[0]);
		if ( ( (input_char == 'e') || (input_char == 'r') ) && (PC == 0xFFFF) ){
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "  \n\nMust load program into memory first before attempting to execute a program" << endl;
				cout << "                     Press ENTER to continue" << endl;
				cout << "-------------------------------------------------------------------------" <<endl;
				cin.get();
		}
		else {
			switch(input_char){
				case 'q':
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                          Press ENTER to exit" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					delete current_instruction;
					exit(EXIT_SUCCESS);
					break;
				case 'z':
					cout << "\n\n-------------------------------------------------------------------------" <<endl;
					cout << "                     Toggle breakpoint register print";
					if(bp_print_regs)
						bp_print_regs = false;
					else
						bp_print_regs = true;
					if(!bp_print_regs)
							cout << " to false" << endl;
					else
							cout << " to true" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                          Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'x':
					cout << "\n\n-------------------------------------------------------------------------" <<endl;
					cout << "                     Toggle breakpoint memory print";
					if(bp_print_mem)
						bp_print_mem = false;
					else
						bp_print_mem = true;
					if(!bp_print_mem)
							cout << " to false" << endl;
					else
							cout << " to true" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                          Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 't':
					cout << "\n\n-------------------------------------------------------------------------" <<endl;
					cout << "                     Print trace file, " << trace_file << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					print_trace();
					cout << "                          Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'y':
					cout << "\n\n-------------------------------------------------------------------------" <<endl;
					cout << "                     Delete old trace file, " << trace_file << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					clear_trace();
					cout << "                          Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'p':
					cout << "All registers' content:" <<endl;
					print_all_registers();
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                     Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'm':
					cout << "All valid memory contents:" <<endl;
					print_all_memory();
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                     Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'r':
					cout << "                  Restarting current application" << endl;
					PC = starting_pc;
					cout << "                        Set PC to " << PC << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					return RUN_PROGRAM;
					break;
				case 'e':
					cout << "                        Starting application" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					return RUN_PROGRAM;
					break;
				case 'b':
					get_user_octal("Input octal breakpoint address(6 digits), then press ENTER:",
					                   "ERROR: Invalid input\n\nPlease input address (6 octal digits), then press ENTER:",
									   bp_address);
					set_breakpoint(bp_address);
					cout <<endl;
					cout << "                          Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'c':
					get_user_octal("Input octal breakpoint address to clear (6 digits), then press ENTER:",
					                   "ERROR: Invalid input\n\nPlease input address (6 octal digits), then press ENTER:",
									   bp_address);
					clear_breakpoint(bp_address);
					cout <<endl;
					cout << "                          Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'v':
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                     Clearing all breakpoints" << endl;
					clear_all_breakpoints();
					cout <<endl;
					cout << "                      Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'n':
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                     Printing all breakpoints" << endl;
					print_all_breakpoints(bp_print_mem, bp_print_regs);
					cout <<endl;
					cout << "                      Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 's':
					get_user_octal("Input octal address to load into PC (6 digits), then press ENTER:",
					                   "ERROR: Invalid input\n\nPlease input address to load into PC (6 octal digits), then press ENTER:",
									   PC);
					cout << "Set PC to start @ address: ";
					print_octal(PC);
					cout <<endl;
					cout << "                          Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'l':
					cout << "                Press ENTER to load new application" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.get();
					return LOAD_DATA;
					break;
				default:
					cout << "                     ERROR: Invalid input\n\n" << endl;
					break;
			}
		}
	}
	return 0;
}

int instruction_fetch(bool & at_breakpoint, uint16_t & instruction_code,  uint16_t & breakpoint_pc, uint16_t & PC){
	instruction_code = read_word(MEMORY_READ, PC, READ_TRACE, READ_INSTR_FETCH);				
	if(check_breakpoint(PC)){ // Check for a breakpoint pointing to this memory location
		breakpoint_pc = PC;
		at_breakpoint = true;
	}
	else
		at_breakpoint = false;		
	PC += 2;
	if(PC%2 == 1) {//IF at odd memory address not allowed
		cerr << "Error: Instruction fetch from odd address is not permitted, halting program!" <<endl;
		return 1;
	}
	return 0;
}

void clear_psw(PSW_t & PSW) {
	PSW.PSW_BYTE = 0;
}

void print_psw(PSW_t & PSW) {
	cout << "Global PSW: " << setfill('0') << setw(3) << oct << uint16_t(PSW.PSW_BYTE) << endl;
	cout << "Global PSW.SPL: " << setfill('0') << setw(1) << uint16_t(PSW.SPL) << endl;
	cout << "Global PSW.T: " << setfill('0') << setw(1) << uint16_t(PSW.T) << endl;
	cout << "Global PSW.N: " << setfill('0') << setw(1) << uint16_t(PSW.N) << endl;
	cout << "Global PSW.Z: " << setfill('0') << setw(1) << uint16_t(PSW.Z) << endl;
	cout << "Global PSW.V: " << setfill('0') << setw(1) << uint16_t(PSW.V) << endl;
	cout << "Global PSW.C: " << setfill('0') << setw(1) << uint16_t(PSW.C) << endl;
}

int write_back(PSW_t & PSW, instruction * inst) {
	PSW.PSW_BYTE = inst->PSW;
	if(PSW.PSW_BYTE != inst->PSW) { // I'm not sure how this would happen, but to allow some type of fault return
		cerr << "Failed to update PSW from current operation!" <<endl;
		return 1;
	}
	return 0;
}
