#include "parser_helper.hh"

int hexValidation(std::ifstream &infile, std::string hexStr, bool rewind) {
    int size = hexStr.length();
    // If the length of the hex string is not even, something is wrong...
    if (0 != size % 2)
        return -1;
    int remain = size; // Check how many bytes left
    int currIdx = 0; // Keep the current index of the string

    while (remain > 0) {
        int num1 = stoi(hexStr.substr(currIdx, 2), 0, 16);
        int num2 = getChar(infile);
        currIdx += 2;
        remain -= 2;

        if (num1 != num2) {
            if (rewind)
                goRewind(infile, currIdx);
            return 1;
        }
    }

    if (rewind)
        goRewind(infile, currIdx / 2);

    return 0;
}

void goRewind(std::ifstream &infile, int bytesRewinded) {
    infile.seekg(-bytesRewinded, std::ios::cur);
}

void moveBytes(std::ifstream &infile, int bytesMoved) {
    infile.seekg(bytesMoved, std::ios::cur);
}

int getChar(std::ifstream &infile) {
    unsigned char reader = 'a';
    infile.read((char *)&reader, sizeof(unsigned char));
    return (int)reader;
}

int getInt(std::ifstream &infile) {
    int ret = 0;
    infile.read((char *)&ret, sizeof(int));
    return ret;
}

double getDouble(std::ifstream &infile) {
    double ret = 0.0;
    infile.read((char *)&ret, sizeof(double));

    return ret;
}
