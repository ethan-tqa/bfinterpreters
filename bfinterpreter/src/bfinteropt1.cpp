#include <cassert>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int mem_size = 30000;

void run(const char * code, long code_size);

int main(int argc, char *argv[])
{
	if (argc == 1) {
		cout << "Please provide a source file name.\n";
		return 1;
	}

	char* fileName = argv[1];
	ifstream fs;
	fs.open(fileName);

	if (fs.fail())
	{
		cerr << "Error: " << strerror(errno);
		return 1;
	}

	fs.seekg(0, fs.end);
	long code_size = fs.tellg();
	fs.seekg(0, fs.beg);

	char * code = new char[code_size + 1];
	code[code_size] = 0; // teminate NULL
	fs.read(code, code_size);
	fs.close();

	auto t1 = high_resolution_clock::now();
	for (size_t i = 0; i < 10; i++)
	{
		run(code, code_size);
	}
	auto t2 = high_resolution_clock::now();

	auto duration = duration_cast<nanoseconds>(t2 - t1).count();
	cout << "Execution time (ns): " << duration << endl;
	cin.get();
}

void run(const char * code, long code_size) 
{
	int8_t mem[mem_size] = { 0 }; // memory
	size_t ptr = 0; // memory pointer
	size_t ip = 0; // instruction pointer

	size_t counter = 0; // how many instructions we have gone through

	size_t* jump_table = new size_t[code_size];
	fill_n(jump_table, code_size, 0);

	// scan and build jump table
	for (ip = 0; ip < code_size; ip++) {
		switch (code[ip]) {
		case '[': {
				int brackets = 1;
				size_t tip = ip;
				while (tip < code_size) {
					tip++;
					if (code[tip] == ']') brackets--;
					if (code[tip] == '[') brackets++;
					if (brackets == 0) {
						jump_table[ip] = tip;
						break; // balanced
					}
				}
			}
			break;
		case ']': {
				int brackets = 1;
				size_t tip = ip;
				while (tip > 0) {
					tip--;
					if (code[tip] == ']') brackets++;
					if (code[tip] == '[') brackets--;
					if (brackets == 0) {
						jump_table[ip] = tip;
						break; // balanced
					}
				}
			}
			break;
		}
	}
	
	for (ip = 0; ip < code_size; ip++) {
		char c = code[ip];
		switch (c) {
		case '+':
			mem[ptr]++;
			break;
		case '-':
			mem[ptr]--;
			break;
		case '>':
			ptr++;
			break;
		case '<':
			ptr--;
			break;
		case '.':
			//cout.put((char)mem[ptr]);
			break;
		case ',':
			assert(false);
			break;
		case '[': {
			if (mem[ptr] == 0) {
				ip = jump_table[ip];
			}
		}
			break;
		case ']': {
			if (mem[ptr] != 0) {
				ip = jump_table[ip];
			}
		}
			break;
		default:
			continue;
			break;
		}

		counter++;
	}

	cout.flush();
 }
