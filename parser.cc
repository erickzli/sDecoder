#include "parser.hh"


int validateHexBlock(ifstream &myfile, string hexStr, bool rewind) {
    int size = hexStr.length();
    // If the length of the hex string is not even, something is wrong...
    if (0 != size % 2)
        return -1;
    int remain = size; // Check how many bytes left
    int currIdx = 0; // Keep the current index of the string

    while (remain > 0) {
        int num1 = stoi(hexStr.substr(currIdx, 2), 0, 16);
        int num2 = getChar(myfile);
        currIdx += 2;
        remain -= 2;
        cout << "num1: " << num1 << endl;
        cout << "num2: " << num2 << endl;
        cout << "curridx: " << currIdx << endl;
        if (num1 != num2) {
            if (rewind)
                goRewind(myfile, currIdx);
            return 1;
        }
    }

    if (rewind)
        goRewind(myfile, currIdx / 2);

    return 0;
}

void goRewind(ifstream &myfile, int bytesRewinded) {
    myfile.seekg(-bytesRewinded, ios::cur);
}

void movePointer(ifstream &myfile, int bytesMoved) {
    myfile.seekg(bytesMoved, ios::cur);
}

int getChar(ifstream &myfile) {
    char reader = 'a';
    myfile.read((char *)&reader, sizeof(char));
    return (int)reader;
}

int getInt(ifstream &myfile) {
    int ret = 0;
    myfile.read((char *)&ret, sizeof(int));
    return ret;
}

double getDouble(ifstream &myfile) {
    double ret = 0.0;
    myfile.read((char *)&ret, sizeof(double));

    return ret;
}
