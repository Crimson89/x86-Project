#include "header.h"

/*

Writes a string of text to a file

Arguments:
	text - string of text to write to the file
	rilename - name of the file to write to
	

Return: 
	0 if succcessful
	1 if there was an error

*/
int write_line(string text, string filename) {
	ofstream outfile;
	outfile.open(filename, ios_base::app);
	if(outfile.is_open()) { //Open/create file and append this line
		outfile << text << endl;
		return 0;
	}
	cerr << "Failed to open file: "<< filename <<endl;
	return 1;
}

/*
Clears the memory trace file

Arguments: None - trace filename is a global value
	

Return: 
	0 if succcessful
	1 if there was an error

*/
int clear_trace() {
	ofstream outfile;
	outfile.open(trace_file);
	if(outfile.is_open()) //Open/create file and erase contents
		return 0;
	cerr << "Failed to open file: "<< trace_file <<endl;
	return 1;
}

/*
Prints the contents of the memory trace file

Arguments: None - trace filename is a global value
	

Return: 
	0 if succcessful
	1 if there was an error

*/
int print_trace() {
	string temp;
	temp.clear();
	cout << "Reading trace file :" << trace_file << "\n";
	cout << "\n--- Start of trace file ---\n"<< endl;
	ifstream fileInput(trace_file);
	if(!fileInput.is_open()) {
		return 1;
	}
	while(getline(fileInput, temp)) {
		cout << temp<<endl;
		temp.clear();
	}
	cout << "\n--- End of trace file ---\n"<< endl;
	return 0;
}

/*
Writes a memory read trace entry to the memory trace file

Arguments:
	address - address that was accessed
	value - the value read from this address
	is_instruction - bool value indicating that this read was an instruction fetch or a data read. True indicates instruction fetch, false indicates data read
	

Return: 
	0 if succcessful
	1 if there was an error

*/
int read_trace(uint16_t address, uint16_t value, bool is_instruction) {
	if(is_instruction) {
		if(verbosity_level >= HIGH_VERBOSITY) cout << "Instr fetch from address @"<<octal_to_string(address)<<": "<<octal_to_string(value)<<endl;
		return write_line(("2 "+octal_to_string(address)),  trace_file);
	}
	if(verbosity_level >= HIGH_VERBOSITY) cout << "Data read from address   @"<<octal_to_string(address)<<": "<<octal_to_string(value)<<endl;
	return write_line(("0 "+octal_to_string(address)),  trace_file);
}

/*
Writes a memory write trace entry to the memory trace file

Arguments:
	address - address that was accessed
	value - the value written to this address	

Return: 
	0 if succcessful
	1 if there was an error

*/
int write_trace(uint16_t address, uint16_t value) {
	if(verbosity_level >= HIGH_VERBOSITY) cout << "Data write to address    @"<<octal_to_string(address)<<": "<<octal_to_string(value)<<endl;
	return write_line(("1 "+octal_to_string(address)),  trace_file);
}

/*
Clears the branch trace file

Arguments: None - trace filename is a global value
	

Return: 
	0 if succcessful
	1 if there was an error

*/
int clear_branch_trace() {
	ofstream outfile;
	outfile.open(branch_trace_file);
	if(outfile.is_open()) //Open/create file and erase contents
		return 0;
	cerr << "Failed to open file: "<< branch_trace_file <<endl;
	return 1;
}

/*
Prints the contents of the branch trace file

Arguments: None - trace filename is a global value
	

Return: 
	0 if succcessful
	1 if there was an error

*/
int print_branch_trace() {
	string temp;
	temp.clear();
	cout << "Reading branch trace file :" << branch_trace_file << "\n";
	cout << "\n--- Start of branch trace file ---\n"<< endl;
	ifstream fileInput(branch_trace_file);
	if(!fileInput.is_open()) {
		return 1;
	}
	while(getline(fileInput, temp)) {
		cout << temp<<endl;
		temp.clear();
	}
	cout << "\n--- End of branch trace file ---\n"<< endl;
	return 0;
}

/*
Writes a branch trace entry to the branch trace file

Arguments:
	if_pc_value - fetched address of the branch instruction
	target - the address that the branch would go to if it were taken
	name - the name of the branch instruction (BNE, BR, RTS, etc.)
	taken - bool value indicating that whether this branch was taken or not

Return: 
	0 if succcessful
	1 if there was an error

*/
int branch_trace(uint16_t if_pc_value, uint16_t target, string name, bool taken) {
	string taken_string;
	if(taken)
		taken_string = ", branch taken.";
	else
		taken_string = ", branch NOT taken.";
	if(verbosity_level >= HIGH_VERBOSITY) cout << "Branch Trace: " << name <<" from PC "<<octal_to_string(if_pc_value)<<" to "<<octal_to_string(target) << taken_string << endl;
	return write_line(("Branch Trace: "+name+" from PC "+octal_to_string(if_pc_value)+" to "+octal_to_string(target)+taken_string), branch_trace_file );
}