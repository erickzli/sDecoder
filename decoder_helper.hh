//
//  decoder_helper.hh
//
//  Created by Erick Li on 04/10/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#ifndef __DECODER_HELPER_H__
#define __DECODER_HELPER_H__

#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <list>
#include <algorithm>

#include <stdint.h>

const double SIGMA = 0.008856;

/**
 * Validate if the upcoming hex value is the same as hexStr.
 * @param the input file stream.
 * @param the hex string used for validation.
 * @param true if the pointer should rewind to the position before this
 *        function is called.
 * @return 0 if validated; 1 if not validated; -1 check errno.
 */
int hexValidation(char **cursor, std::string hexStr, bool rewind);

/**
 * Rewind file pointer by bytesRewinded.
 * @param the input file stream.
 * @param Bytes need to be rewinded (Number in POSITIVE)
 */
void bytesRewinder(char **cursor, int bytesRewinded);

/**
 * Move the file pointer bytesHopped bytes.
 * @param the input file stream.
 * @param the amount of bytes the pointer moves.
 */
void bytesHopper(char **cursor, int bytesHopped);

/**
 * Get the character integer value (8-bit) from the current position.
 * @param the input file stream.
 * @return the integer-type value of the character gotten.
 */
int getChar(char **cursor);

/**
 * Get the character integer value (16-bit) from the current position.
 * @param the input file stream.
 * @return the integer-type value of the character gotten.
 */
int get16Bit(char **cursor);

/**
 * Get the integer value (32-bit) from the current position.
 * @param the input file stream.
 * @return the integer.
 */
int get32Bit(char **cursor);

/**
 * Get the integer value (64-bit) from the current position.
 * @param the input file stream.
 * @return the integer.
 */
int get64Bit(char **cursor);

/**
 * Get the double value from the current position.
 * @param the input file stream.
 * @return the double value.
 */
double getDouble(char **cursor);

/**
 * cout the mseg.
 * @param the logging message.
 */
void LOG(std::string mseg);

/**
 * Print "num" bytes of the binary file from the current position. The cursor
 *   will be automatically rewinded.
 * @param the input file stream.
 * @param number of bytes to be printed.
 */
void printHex(char **cursor, int num);

/**
 * Convert a string to camelCase format.
 * @param the input string
 * @return the camelCased output.
 */
std::string toSimpleCamelCase(std::string str);

/**
 * Convert a CIELAB color code to RGB
 * @param L
 * @param a
 * @param b
 * @param type: 0: to RGB; 1: to HSV.
 * @return the corresponding list of RGB color code.
 * NOTE:
 *   1. The conversion CIELAB -> CIEXYZ is from English Wikipedia page "CIELAB
 *     color space";
 *   2. The conversion CIEXYZ -> RGB is from
 *     https://www.cs.rit.edu/~ncs/color/t_convert.html#XYZ%20to%20CIE%20L*a*b*%20(CIELAB)%20&%20CIELAB%20to%20XYZ
 */
std::list<double> CIELAB_to_RGB_HSV(double L, double a, double b, int type);

// https://www.rapidtables.com/convert/color/rgb-to-hsv.html
std::list<double> RGB_to_HSV(double r, double g, double b);

double private_f(double t);
double format_rgb(double code);

#endif
