#include <iostream>
#include "parser.hh"

using namespace std;

int main(int argc, char **argv) {
    ifstream myfile;
    myfile.open("BFiles/test_double", ios::binary | ios::in);

    if (myfile.is_open()) {
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
