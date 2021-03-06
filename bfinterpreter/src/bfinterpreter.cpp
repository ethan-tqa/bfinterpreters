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
	run(code, code_size);
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

	for (ip = 0; ip < code_size; ip++)
	{
		char c = code[ip];
		switch (c)
		{
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
			cout.put((char)mem[ptr]);
			break;
		case ',':
			assert(false);
			break;
		case '[':
			if (mem[ptr] == 0) {
				int brackets = 1;
				while (ip < code_size) {
					ip++;
					if (code[ip] == ']') brackets--;
					if (code[ip] == '[') brackets++;
					if (brackets == 0)
						break; // balanced
				}
			}
			break;
		case ']':
			if (mem[ptr] != 0) {
				int brackets = 1;
				while (ip > 0) {
					ip--;
					if (code[ip] == ']') brackets++;
					if (code[ip] == '[') brackets--;
					if (brackets == 0) 
						break; // balanced
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
