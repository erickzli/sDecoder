#include <iostream>
#include "parser.hh"

using namespace std;

int main(int argc, char **argv) {
    ifstream myfile;
    myfile.open("BFiles/test_double", ios::binary | ios::in);

    if (myfile.is_open()) {
        // Check if the version of style file is parseable.
        int s = validateHexBlock(myfile, "04E6147992C8D0", false);
        if (s == 1) {
            cout << "ERROR: Check style file version." << endl;
            return 1;
        } else {
            cout << "LOG: File version is validated." << endl;
        }
    }

    return 0;
}
