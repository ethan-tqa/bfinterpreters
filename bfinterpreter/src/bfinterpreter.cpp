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

	char * content = new char[size + 1];
	content[size] = 0; // teminate NULL
	fs.read(content, size);
	fs.close();

	cout << "File content: " << content;
}
