//
//  parser_helper.cc
//
//  Created by Erick Li on 04/10/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

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
                bytesRewinder(cursor, currIdx);
            return 1;
        }
    }

    if (rewind)
        bytesRewinder(cursor, currIdx / 2);

    return 0;
}

void bytesRewinder(char **cursor, int bytesRewinded) {
    // Move the cursor backwwards
    *cursor -= bytesRewinded;
}

void bytesHopper(char **cursor, int bytesHopped) {
    // Move the cursor forwards
    *cursor += bytesHopped;
}

int getChar(char **cursor) {
    unsigned char ret = **cursor;
    // After getting the current character, move to the next character.
    bytesHopper(cursor, 1);

    return (int)ret;
}

int get16Bit(char **cursor) {
    uint16_t *ret = (uint16_t *)*cursor;
    // After getting the current character, move to the second next character.
    bytesHopper(cursor, 2);

    return *ret;
}

int getInt(char **cursor) {
    uint32_t *ret = (uint32_t *)*cursor;
    // After getting the current character, move to the next integer.
    bytesHopper(cursor, 4);

    return *ret;
}

double getDouble(char **cursor) {
    double *ret = (double *)*cursor;
    // After getting the current character, move to the next double.
    bytesHopper(cursor, 8);

    return *ret;
}

void LOG(std::string mseg) {
    std::cout << mseg << std::endl;
}
