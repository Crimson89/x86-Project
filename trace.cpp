#include "header.h"

int write_line(string text) {
	ofstream outfile;
	outfile.open(trace_file, ios_base::app);
	if(outfile.is_open()) { //Open/create file and append this line
		outfile << text << endl;
		return 0;
	}
	cerr << "Failed to open file: "<< trace_file <<endl;
	return 1;
}

int clear_trace() {
	ofstream outfile;
	outfile.open(trace_file);
	if(outfile.is_open()) //Open/create file and erase contents
		return 0;
	cerr << "Failed to open file: "<< trace_file <<endl;
	return 1;
}

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

int read_trace(uint16_t address, uint16_t value, bool is_instruction) {
	if(is_instruction) {
		if(verbosity_level >= HIGH_VERBOSITY) cout << "Instr fetch from address @"<<octal_to_string(address)<<": "<<octal_to_string(value)<<endl;
		return write_line(("2 "+octal_to_string(address)));
	}
	if(verbosity_level >= HIGH_VERBOSITY) cout << "Data read from address   @"<<octal_to_string(address)<<": "<<octal_to_string(value)<<endl;
	return write_line(("0 "+octal_to_string(address)));
}

int write_trace(uint16_t address, uint16_t value) {
	if(verbosity_level >= HIGH_VERBOSITY) cout << "Data write to address    @"<<octal_to_string(address)<<": "<<octal_to_string(value)<<endl;
	return write_line(("1 "+octal_to_string(address)) );
}
