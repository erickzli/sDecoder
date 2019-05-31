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

std::list<double> CIELAB_to_RGB_HSV(double L, double a, double b, int type) {
    if (0 != type && 1 != type) {
        LOG("ERROR: Color conversion type not found...");
        throw std::string("Color conversion type");
    }

    const double X_n = 95.0489;
    const double Y_n = 100.0;
    const double Z_n = 108.884;

    double x = X_n * private_f((L + 16.0) / 116.0 + a / 500.0);
    double y = Y_n * private_f((L + 16.0) / 116.0);
    double z = Z_n * private_f((L + 16.0) / 116.0 - b / 200.0);

    double r = format_rgb(3.240479 * x - 1.537150 * y - 0.498535 * z);
    double g = format_rgb(-0.969256 * x + 1.875992 * y + 0.041556 * z);
    double bl = format_rgb(0.055648 * x - 0.204043 * y + 1.057311 * z);

    // return RGB
    if (0 == type) {
        std::list<double> myrgb;
        myrgb.push_back(r);
        myrgb.push_back(g);
        myrgb.push_back(bl);
        return myrgb;
    }

    // otherwise return HSV...
    return RGB_to_HSV(r, g, bl);
}

std::list<double> RGB_to_HSV(double r, double g, double b) {
    double r_pm = r / 255.0;
    double g_pm = g / 255.0;
    double b_pm = b / 255.0;

    double c_max = std::max(r_pm, g_pm);
    c_max = std::max(c_max, b_pm);
    double c_min = std::min(r_pm, g_pm);
    c_min = std::min(c_min, b_pm);

    double delta = c_max - c_min;

    double h = 0.0;
    double s = 0.0;
    double v = 0.0;

    // Hue
    if (0.0 == delta) {
        h = 0.0;
    } else if (c_max == r_pm) {
        h = 60.0 * ((g_pm - b_pm) / delta + 6);
    } else if (c_max == g_pm) {
        h = 60.0 * ((b_pm - r_pm) / delta + 2);
    } else if (c_max == b_pm) {
        h = 60.0 * ((r_pm - g_pm) / delta + 4);
    }

    // Saturation
    if (0.0 == c_max) {
        s = 0.0;
    } else {
        s = delta / c_max;
    }

    // Value
    v = c_max;

    std::list<double> myhsv;
    myhsv.push_back(h);
    myhsv.push_back(s);
    myhsv.push_back(v);
    return myhsv;
}

double private_f(double t) {
    const double SIGMA = 6.0 / 29.0;
    if (t > SIGMA) {
        return pow(t, 3.0);
    } else {
        return 3 * pow(SIGMA, 2.0) * (t - 4.0 / 29.0);
    }
}

double format_rgb(double code) {
    if (code > 255.0) {
        return 255.0;
    } else if (code < 0) {
        return 0.0;
    } else {
        return code;
    }
}
