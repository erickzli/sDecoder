#include <iostream>
#include "parser.hh"

using namespace std;

int main(int argc, char **argv) {
    ifstream myfile;
    myfile.open("BFiles/test_double", ios::binary | ios::in);

    if (myfile.is_open()) {
        int checkee = validateHexBlock(myfile, "64636261", true);
        cout << checkee << endl;
        cout << getChar(myfile) << endl;
        cout << getChar(myfile) << endl;
        cout << getChar(myfile) << endl;
        cout << getChar(myfile) << endl;
        cout << getInt(myfile) << endl;
        cout << getDouble(myfile) << endl;
        cout << getDouble(myfile) << endl;
        myfile.close();
    }

    return 0;
}
