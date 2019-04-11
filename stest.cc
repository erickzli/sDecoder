#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {
	double num = 3.555555555;

	cout << to_string(num) << endl;
	cout << argc << endl;

	ofstream f;
	f.open("test.txt", ios::out);
	f << "hello" << endl;

	f.close();

	return 0;
}

