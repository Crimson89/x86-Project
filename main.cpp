#include "header.h"

//Make REGS and MEM global
uint16_t REGS[7];
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
uint8_t MEM[MEMORY_SPACE];
uint16_t starting_pc;
instruction current_instruction;	// decoded instruction information
int verbosity_level = 0;            // Level of verbosity in print statements

int main(int argc, char ** argv)
{
  //int test;
  //test = parseTest();
  
	uint16_t operation;
	int program_execution_control = 0;
	SP = 0xFFFF; // Assign at start to invalid value, detection of unassigned SP
	PC = 0xFFFF; // Assign at start to invalid value, detection of unassigned PC
	starting_pc = PC;
	initializeMemory();
	int err;							// error checking
	uint16_t instruction_code;			// 16-bit instruction
	
	for(;;){
		program_execution_control = menu_function();
		if(program_execution_control == LOAD_DATA) {
			loadData(argc, argv);
			program_execution_control = PRINT_MENU;
		}
		else if((program_execution_control == RUN_PROGRAM) && (PC != 0xFFFF)) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "                     Press ENTER to begin program" << endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			cin.get();
			cout << "                            Executing program" <<endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			cout << "Assigning registers R0 to R5 with data from even memory addresses 0000002 to 0000014 (Octal)" <<endl;
			R0 = read_word(2);
			R1 = read_word(4);
			R2 = read_word(6);
			R3 = read_word(8);
			R4 = read_word(10);
			R5 = read_word(12);
			cout << "Set SP to memory index 36 (Octal address 000044)" <<endl;
			SP = 36;
	
			// Main program loop
			while(program_execution_control == RUN_PROGRAM) {

				cout << "All registers' content:" <<endl;
				print_all_registers();
				cout << "All valid memory contents:" <<endl;
				print_all_memory();
				operation = m_HALT;

				// IF
				instruction_code = read_word(PC);
				PC += 2;	

				// ID
				err = parseInstruction(instruction_code, &current_instruction); 
				// check error code
				
				preIncrement();

				err = loadOperands();	// deal w/ addressing modes
				
				// updateTracefile();

				// EX
				// call appropriate function				

				// WB
				postIncrement();

				// updateTracefile();
				

				if(operation == m_HALT)
					program_execution_control = PRINT_MENU;
			}
			cout << "-------------------------------------------------------------------------" <<endl;
			cout << "                            Program Completed!" <<endl;
			cout << "                     Press ENTER to return to menu" << endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			cin.get();
		}
		else {
			cout << "-------------------------------------------------------------------------" <<endl;
			cout << "                  Unhandled condition, is PC set to valid address?!" <<endl;
			cout << "                     Press ENTER to return to menu" << endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			cin.get();
			program_execution_control = PRINT_MENU;
		}
	}
	return 0; // never hit, program exits from menu function
}

int preIncrement() {

}

int postIncrement() {

}

int storeOperands() {

}

int loadOperands() {

}


uint16_t string_to_octal(string this_string){
	return stoul(this_string, NULL, 8);
}

uint16_t read_byte(uint16_t address) {
	return (0x00FF&MEM[address]);
}

uint16_t read_word(uint16_t address) {
	return ( (MEM[address+1]<<8) | (MEM[address]) );
}

void write_byte(uint16_t address, uint16_t byte) {
	MEM[address] = 0x00FF&byte;
}

void write_word(int address, uint16_t word) {
	MEM[address]   = (0x00FF&word);
	MEM[address+1] = ((0xFF00&word)>>8);
}

void print_octal(uint16_t operation){
	cout <<setfill('0')<<setw(6)<<oct<<operation;
}


void print_all_memory(void) {
	uint16_t hasContent = 0;
	uint8_t memory_word;
	for(int i = 0; i < MEMORY_SPACE; i+=2) {
		memory_word = read_word(i);
		if(memory_word!=0xFFFF) {
			hasContent+=1;
			cout <<"@ADDR="; print_octal(i); cout <<", Contents="; print_octal(memory_word);
			if( (PC != 0xFFFF) && (i == PC) )
				cout << " <-- PC";
			if( (SP != 0xFFFF) && (i == SP) )
				cout << " <-- SP";
			cout<< endl;
		}
	}
	if(hasContent) {
		cout << "Memory contains " <<dec<<hasContent*2 << " bytes of information" << endl;
	}
	else
		cout << "Memory is empty" << endl;
}


void initializeMemory(){
	for(int i = 0; i < MEMORY_SPACE; i++) { //Initialize memory space
		write_byte(i,0xFF);
	}
}

void print_all_registers(void) {
	for(int i = 0; i < 5; i++) {
		cout <<"R"<<i<<":";
		print_octal(REGS[i]);
		cout<< endl;
	}
	cout <<"SP"<<":";
	print_octal(SP);
	if(SP == 0xFFFF)
		cout<< "  <-- This is an invalid SP, 0xFFFF";
	cout<< endl;
	cout <<"PC"<<":";
	print_octal(PC);
	if(PC == 0xFFFF)
		cout<< "  <-- This is an invalid PC, 0xFFFF";
	cout<< endl;
}


int get_user_octal(string prompt, string error_text, uint16_t &word){
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
			cerr << error_text;
			input = "FALSE";
			temp  = "FALSE";
		}
	}
}

int menu_function() {
	string input;
	char input_char;
	int menu_continue = 1;
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
					cin.get();
					exit(EXIT_SUCCESS);
					break;
				case 'p':
					cout << "All registers' content:" <<endl;
					print_all_registers();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                     Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.get();
					break;
				case 'm':
					cout << "All valid memory contents:" <<endl;
					print_all_memory();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                     Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
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
				case 's':
					if(!get_user_octal("Input octal address to load into PC (6 digits), then press ENTER:",
					                   "ERROR: Invalid input\n\nPlease input address to load into PC (6 octal digits), then press ENTER:",
									   PC)) {
						cout << "Set PC to start @ address: ";
						print_octal(PC);
						cout <<endl;
						cout << "                          Press ENTER to continue" << endl;
						cout << "-------------------------------------------------------------------------" <<endl;
						cin.get();
					}
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

int loadData(int argc, char ** argv){
	int return_val = 1;
	string fileName;	//File name of data file
	int opt;
	optind = 0;			//force getOpt to restart for repeated loading from file
	verbosity_level = -1;
	while ((opt = getopt(argc, argv, "f:m:")) != -1) {
		switch (opt) {
		case 'f':
			fileName = string(optarg);
			break;
		case 'm':
			if (string(optarg) == "simple")
				verbosity_level = LOW_VERBOSITY;
			else if (string(optarg) == "verbose")
				verbosity_level = HIGH_VERBOSITY;
			else if (string(optarg) == "debug")
				verbosity_level = DEBUG_VERBOSITY;
			else	 //Default to invalid mode
				verbosity_level = -1;
			break;
		default:
			cerr << "Error, invalid option" << endl;
			cerr << "Usage: " << argv[0] << " [-f dataFileName] [-m simple|verbose]" << endl;
			cerr << "           Press ENTER to exit and close application" << endl;
			cerr << "-------------------------------------------------------------------------" <<endl;
			cerr << "-------------------------------------------------------------------------" <<endl;
			cin.get();	//Wait for ENTER key press
			exit(EXIT_FAILURE);
		}
	}
	if (verbosity_level <= -1) { //Print error and return
		cerr << "Invalid mode:" << endl;
		cerr << "Usage: " << argv[0] << " [-f dataFileName] [-m simple|verbose]" << endl;
		cerr << "             Press ENTER to exit and close application" << endl;
		cerr << "-------------------------------------------------------------------------" <<endl;
		cerr << "-------------------------------------------------------------------------" <<endl;
		cin.get();	//Wait for ENTER key press
		exit(EXIT_FAILURE);
	}
	return_val = readData(fileName);
	return return_val;
}


int readData(string fileName){
	string temp;							//File read temp string
	string input_word;						//Store input_word octal characters
	int stringLength = 0;					//Length of temp string
	char line_type = 'z';					//The type of thing on this line, valid options are '*', '@', and '-'
	int address = 0;					    //The address of this input_word in MEM
	int file_line_num = 0;					//Line number in the file being read
	uint16_t temporary_addr = 0;			//Used if this word turns out to be a memory address
	temp.clear();							//initialize temp
	input_word.clear();						//initialize input_word
	ifstream fileInput(fileName);			//File ifstream handle, and open file

	initializeMemory();
	if(!fileInput.is_open()) {
		cerr << "failed to open file : " << fileName << endl;
		return 1;
	}
	cout << "Reading from:" << fileName << "\n";
	while(getline(fileInput, temp))			// continue reading consecutive lines until EOF is reached
	{
		stringLength = temp.length();
		if( stringLength > 1) {
			line_type = temp[0];
			if(verbosity_level >= HIGH_VERBOSITY) cout << "Reading data on line #"<<file_line_num<<",\t\""<<temp<<"\""<<endl;
			if(verbosity_level >= DEBUG_VERBOSITY) cout << "Line type "<<line_type<<endl;
			for(int i = 1; i < stringLength; i++) { 			//Iterate through read line and copy out characters that are valid octal
				if (isxdigit(temp[i]) && ((temp[i]-48) < 8))	//Valid octal character
					input_word += temp[i];
				else  //If invalid input received, break out of for loop, stop parsing this line, move to next line in file
					break;
			}
			if(verbosity_level >= DEBUG_VERBOSITY) cout << "input_word is "<<input_word<<endl;
			if(input_word.length() == 6 && line_type == '-') {
				if(verbosity_level >= DEBUG_VERBOSITY) cout << "Memory load"<<endl;
				if(verbosity_level >= DEBUG_VERBOSITY) {cout << "Loading:  ";print_octal(string_to_octal(input_word));cout<<endl;}
				if(verbosity_level >= DEBUG_VERBOSITY) {cout << "@address: ";print_octal(address);cout<<endl;}
				write_word(address,string_to_octal(input_word)); //Read string, convert uint16, and write to memory
				if(verbosity_level >= DEBUG_VERBOSITY) {cout << "Loaded to memory, Read Back: "; print_octal(read_word(address)); cout<<endl;}
				address+=2;
			}
			else {
				temporary_addr = string_to_octal(input_word);
				if (line_type == '*') {
					starting_pc = temporary_addr;
					PC = starting_pc;
					if(verbosity_level >= DEBUG_VERBOSITY) {cout << "Set PC to start @ address: "; print_octal(PC); cout <<endl;}
				}
				else if (line_type == '@') {
					address = temporary_addr;
				if(verbosity_level == DEBUG_VERBOSITY) {cout << "Set current memory location to address: "; print_octal(address); cout << endl;}
				}
				else
					cerr << "Skipping invalid input in file: " << input_word << " at line#" <<dec<< file_line_num << endl;
			}
		}
		else
			cerr << "Skipping empty line in file: " << temp << " at line#" <<dec<< file_line_num << endl;
		//Clear and set vars for next line in file
		file_line_num++;
		temp.clear();						
		input_word.clear();
		line_type = 'z';
		stringLength = 0;
		temporary_addr = 0;
	}
	fileInput.close(); //done reading, close the file
	//cout << "Done reading file, printing valid memory contents:" <<endl;
	//print_all_memory();
	cout << "-------------------------------------------------------------------------" <<endl;
	cout << "                     Loading memory completed" << endl;
	cout << "                     Press ENTER to continue" << endl;
	cout << "-------------------------------------------------------------------------" <<endl;
	cin.get();
	return 0;
}


