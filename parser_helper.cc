#include "parser_helper.hh"

int hexValidation(char **cursor, std::string hexStr, bool rewind) {
    int size = hexStr.length();
    // If the length of the hex string is not even, something is wrong...
    if (0 != size % 2)
        return -1;
    int remain = size; // Check how many bytes left
    int currIdx = 0; // Keep the current index of the string

    while (remain > 0) {
        int num1 = stoi(hexStr.substr(currIdx, 2), 0, 16);
        int num2 = getChar(cursor);

        // std::cout << "num1: " << num1 << std::endl;
        // std::cout << "num2: " << num2 << std::endl;
        currIdx += 2;
        remain -= 2;

        if (num1 != num2) {
            if (rewind)
                goRewind(cursor, currIdx);
            return 1;
        }
    }

    if (rewind)
        goRewind(cursor, currIdx / 2);

    return 0;
}

void goRewind(char **cursor, int bytesRewinded) {
    *cursor -= bytesRewinded;
}

void moveBytes(char **cursor, int bytesMoved) {
    *cursor += bytesMoved;
}

int getChar(char **cursor) {
    unsigned char ret = **cursor;
    moveBytes(cursor, 1);

    return (int)ret;
}

int getInt(char **cursor) {
    uint32_t *ret = (uint32_t *)*cursor;
    moveBytes(cursor, 4);

    return *ret;
}

double getDouble(char **cursor) {
    double *ret = (double *)*cursor;
    moveBytes(cursor, 8);

    return *ret;
}
