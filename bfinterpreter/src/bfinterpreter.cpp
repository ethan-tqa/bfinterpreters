#include <cassert>
#include <iostream>
#include <fstream>

using namespace std;

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
	long size = fs.tellg();
	fs.seekg(0, fs.beg);

	char * code = new char[size + 1];
	code[size] = 0; // teminate NULL
	fs.read(code, size);
	fs.close();

	uint8_t mem[10] = { 0 }; // memory
	size_t ptr = 0; // memory pointer
	size_t ip = 0; // instruction pointer

	for (ip = 0; ip < size; ip++)
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
				do {
					ip++;
				} while (code[ip] != ']');
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
		}
	}

	cout.flush();
 }
