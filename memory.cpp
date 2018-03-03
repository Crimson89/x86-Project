#include "header.h"

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

//print_octal(read_word(address))
void print_all_memory(void) {
	uint16_t hasContent = 0;
	uint16_t memory_word;
	for(uint16_t i = 0; i < MEMORY_SPACE; i+=2) {
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
	cout << "Done reading file";
	if(verbosity_level == HIGH_VERBOSITY) { cout << ", printing valid memory contents: "; print_all_memory();}
	cout <<endl;
	cout << "-------------------------------------------------------------------------" <<endl;
	cout << "                     Loading memory completed" << endl;
	cout << "                     Press ENTER to continue" << endl;
	cout << "-------------------------------------------------------------------------" <<endl;
	cin.get();
	return 0;
}


