#include "parser.hh"


void movePointer(ifstream &myfile, int bytesMoved) {
    myfile.seekg(bytesMoved, myfile.cur);
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
