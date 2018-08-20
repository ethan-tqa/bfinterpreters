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
	for (size_t i = 0; i < 1; i++)
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
	instruction instructions[10000];
	size_t instructions_count = 0;

	for (size_t ip = 0; ip < code_size;) {
		size_t tip = ip;
		char c = code[ip];
		eOpcode op = eOpcode::Invalid;
		switch (c) {
		case '+':
			op = eOpcode::Add;
			break;
		case '-':
			op = eOpcode::Minus;
			break;
		case '>':
			op = eOpcode::Right;
			break;
		case '<':
			op = eOpcode::Left;
			break;
		case '.':
			op = eOpcode::Print;
			break;
		case ',':
			op = eOpcode::Read;
			break;
		case '[':
			op = eOpcode::Loop;
			break;
		case ']':
			op = eOpcode::Return;
			break;
		default:
			op = eOpcode::Invalid;
			ip++;
			continue;
			break;
		}

		instructions[instructions_count].opcode = op;

		if (op != eOpcode::Loop && op != eOpcode::Return && op != eOpcode::Print) {
			while (tip < code_size && code[tip] == c) {
				tip++;
			}
			instructions[instructions_count].value = tip - ip;
			ip = tip;
		}
		else {
			ip++;
		}

		instructions_count++;
	}

	for (size_t ip = 0; ip < instructions_count; ip++)
	{
		int brackets = 1;
		size_t tip = ip;
		switch (instructions[ip].opcode)
		{
		case eOpcode::Loop:
			while (tip < instructions_count) {
				tip++;
				if (instructions[tip].opcode == eOpcode::Return) brackets--;
				if (instructions[tip].opcode == eOpcode::Loop) brackets++;
				if (brackets == 0) {
					instructions[ip].value = tip - ip;
					break; // balanced
				}
			}
			break;
		case eOpcode::Return:
			while (tip > 0) {
				tip--;
				if (instructions[tip].opcode == eOpcode::Return) brackets++;
				if (instructions[tip].opcode == eOpcode::Loop) brackets--;
				if (brackets == 0) {
					instructions[ip].value = ip - tip + 1;
					break; // balanced
				}
			}
			break;
		default:
			break;
		}
	}

	int8_t mem[mem_size] = { 0 }; // memory
	size_t ptr = 0; // memory pointer

	size_t counter = 0; // how many instructions we have gone through
	
	for (size_t ip = 0; ip < instructions_count; ip++) {
		instruction inst = instructions[ip];
		switch (inst.opcode) {
		case eOpcode::Add:
			mem[ptr] += inst.value;
			break;
		case eOpcode::Minus:
			mem[ptr] -= inst.value;
			break;
		case eOpcode::Right:
			ptr += inst.value;
			break;
		case eOpcode::Left:
			ptr -= inst.value;
			break;
		case eOpcode::Print:
			cout.put((char)mem[ptr]);
			break;
		case eOpcode::Read:
			assert(false);
			break;
		case eOpcode::Loop: {
			if (mem[ptr] == 0) {
				ip += inst.value;
			}
		}
			break;
		case eOpcode::Return: {
			if (mem[ptr] != 0) {
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
