#include <cassert>
#include <iostream>
#include <fstream>
#include <chrono>

#include "shared.h"

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
	instruction* instructions = new instruction[code_size];
	for (size_t ip = 0; ip < code_size; ip++) {
		switch (code[ip]) {
		case '+':
			instructions[ip].opcode = eOpcode::Add;
			break;
		case '-':
			instructions[ip].opcode = eOpcode::Minus;
			break;
		case '>':
			instructions[ip].opcode = eOpcode::Right;
			break;
		case '<':
			instructions[ip].opcode = eOpcode::Left;
			break;
		case '.':
			instructions[ip].opcode = eOpcode::Print;
			break;
		case ',':
			instructions[ip].opcode = eOpcode::Read;
			break;
		case '[': {
			instructions[ip].opcode = eOpcode::Loop;
			int brackets = 1;
			size_t tip = ip;
			while (tip < code_size) {
				tip++;
				if (code[tip] == ']') brackets--;
				if (code[tip] == '[') brackets++;
				if (brackets == 0) {
					instructions[ip].value = tip - ip;
					break; // balanced
				}
			}
		}
				  break;
		case ']': {
			instructions[ip].opcode = eOpcode::Return;
			int brackets = 1;
			size_t tip = ip;
			while (tip > 0) {
				tip--;
				if (code[tip] == ']') brackets++;
				if (code[tip] == '[') brackets--;
				if (brackets == 0) {
					instructions[ip].value = ip - tip;
					break; // balanced
				}
			}
		}
			break;
		default:
			instructions[ip].opcode = eOpcode::Invalid;
			break;
		}
	}

	int8_t mem[mem_size] = { 0 }; // memory
	int8_t* ptr = mem; // memory pointer

	size_t counter = 0; // how many instructions we have gone through
	
	for (size_t ip = 0; ip < code_size; ip++) {
		instruction inst = instructions[ip];
		switch (inst.opcode) {
		case eOpcode::Add:
			(*ptr)++;
			break;
		case eOpcode::Minus:
			(*ptr)--;
			break;
		case eOpcode::Right:
			ptr++;
			break;
		case eOpcode::Left:
			ptr--;
			break;
		case eOpcode::Print:
			//cout.put((char)(*ptr));
			break;
		case eOpcode::Read:
			assert(false);
			break;
		case eOpcode::Loop: {
			if ((*ptr) == 0) {
				ip += inst.value;
			}
		}
			break;
		case eOpcode::Return: {
			if ((*ptr) != 0) {
				ip -= inst.value;
			}
		}
			break;
		case eOpcode::Invalid:
			break;
		default:
			assert(false);
			break;
		}

		counter++;
	}

	cout.flush();
 }
