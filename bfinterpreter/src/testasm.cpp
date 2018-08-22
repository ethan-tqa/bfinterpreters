#include <cassert>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

extern "C" int lol();

int main(int argc, char *argv[]) {
	cout << "result: " << lol();
	cin.get();
}