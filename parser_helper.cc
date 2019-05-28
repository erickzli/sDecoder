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

        // std::clog << "num1: " << num1 << std::endl;
        // std::clog << "num2: " << num2 << std::endl;
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

int get32Bit(char **cursor) {
    uint32_t *ret = (uint32_t *)*cursor;
    // After getting the current character, move to the next integer.
    bytesHopper(cursor, 4);

    return *ret;
}

int get64Bit(char **cursor) {
    uint64_t *ret = (uint64_t *)*cursor;
    // After getting the current character, move to the next integer.
    bytesHopper(cursor, 8);

    return *ret;
}

double getDouble(char **cursor) {
    double *ret = (double *)*cursor;
    // After getting the current character, move to the next double.
    bytesHopper(cursor, 8);

    return *ret;
}

void LOG(std::string mseg) {
    std::clog << mseg << std::endl;
}

void printHex(char **cursor, int num) {
    if (num <= 0) {
        std::clog << "WARNING: printHex() cannot take param <= 0... ";
        std::clog << "  So...nothing is printed..." << std::endl;
        return;
    }

    std::clog << "TEST: printing " << num << " byte(s)..." << std::endl;
    for (int i = 0; i < num; i++) {
        std::clog << std::hex << std::setfill('0') << std::setw(2) << getChar(cursor) << " ";
        if ((i + 1) % 8 == 0) {
            std::clog << " ";
        }
        if ((i + 1) % 16 == 0) {
            std::clog << std::endl;
        }
    }
    std::clog << std::endl;

    bytesRewinder(cursor, num);
}

std::string toSimpleCamelCase(std::string str) {
    int len = str.length();
    std::string ret = "";
    bool mustUpper = false;
    bool mustLower = true;

    for (int i = 0; i < len; i++) {
        if (str[i] == ' ') {
            mustUpper = true;
        } else if (mustUpper) {
            ret += toupper(str[i]);
            mustUpper = false;
        } else if (mustLower) {
            ret += tolower(str[i]);
            mustLower = false;
        } else {
            ret += str[i];
        }
    }

    return ret;
}
