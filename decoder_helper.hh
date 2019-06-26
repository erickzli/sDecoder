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
 * Validate if the data block starting from the position of the cursor is the
      same as stated in "hexStr", until reaching the length of hexStr divided
      by two (because one hexidecimal digit uses 4 bits, and a byte in the data
      block occupies TWO "4 bits")
 * \param (See json_writer.hh)
 * \param a string used for comparison, the string should be written as a
            hexidecimal string. For example, by passing "E35467" into the
            function, 0xE35467 will be used for comparison.
 * \param option if the cursor should go back to the position before the
            function was called:
          true: go back to the original position;
          false: stay at the position at the end of the compared block.
 * \return:
      0 if validated;
      1 if not validated;
      -1 if the hexStr is not a legal hexidecimal string (see WARNING)
 * WARNING: hexStr MUST be a legal hexidecimal string, which should follow the
     rules:
       1. The length of hexStr must be a positive even integer;
       2. Each character of hexStr must fall into 0-9, or A-F.
     Not following the rules may occur undefined behaviors
 */
int hexValidation(char **cursor, std::string hexStr, bool rewind);

/**
 * Move the cursor towards the head of the data block.
 * \param (See json_writer.hh)
 * \param Number of bytes to be rewinded
 * WARNING:
 *   Undefined behaviors, when:
 *     1. bytesRewinded is negative;
 *     2. Rewind beyond the head of the data block
 */
void bytesRewinder(char **cursor, int bytesRewinded);

/**
 * Move the cursor towards the tail of the data block.
 * \param (See json_writer.hh)
 * \param Number of bytes to be hopped
 * WARNING:
 *   Undefined behaviors, when:
 *     1. bytesHopped is negative;
 *     2. Hop beyond the tail of the data block
 */
void bytesHopper(char **cursor, int bytesHopped);

/**
 * Get the character in integer form from the current cursor position.
 * \param (See json_writer.hh)
 * \return the integer
 */
int getChar(char **cursor);

/**
 * Get the value of a 16-bit long block in integer form from the current cursor
 *   position.
 * \param (See json_writer.hh)
 * \return the integer
 */
int get16Bit(char **cursor);

/**
 * Get the value of a 32-bit long block in integer form from the current cursor
 *   position.
 * \param (See json_writer.hh)
 * \return the integer
 */
int get32Bit(char **cursor);

/**
 * Get the value of an integer from the current cursor position.
 * \param (See json_writer.hh)
 * \return the integer
 */
int get64Bit(char **cursor);

/**
 * Get the value of a double-precision floating-point number from the current
 *    cursor position.
 * \param (See json_writer.hh)
 * \return the double-precision floating-point number
 */
double getDouble(char **cursor);

/**
 * Print out the message stated in msg.
 * \param the logging message.
 */
void LOG(std::string msg);

/**
 * Print out indicated bytes of data in hexidecimal form, the cursor will be
 *   automatically rewinded.
 * \param (See json_writer.hh)
 * \param number of bytes to be printed
 * NOTE: The length of the output string should be twice as the bytes stated in
 *   the param numOfBytes, since two digits of a hexidecimal number are for one
 *   byte.
 */
void printHex(char **cursor, int numOfBytes);

/**
 * Convert a string into camelCase format.
 * \param the input string
 * \return the camelCased output.
 */
std::string _toCamelCaseSimple(std::string str);

/**
 * Convert CIELAB color to RGB or HSV
 * \param L
 * \param a
 * \param b
 * \param type: 0: to RGB; 1: to HSV.
 * \return a list with 3 components for RGB or HSV
 * NOTE:
 *   1. The conversion CIELAB -> CIEXYZ is from English Wikipedia page "CIELAB
 *     color space";
 *   2. The conversion CIEXYZ -> RGB is from
 *     https://www.cs.rit.edu/~ncs/color/t_convert.html#XYZ%20to%20CIE%20L*a*b*%20(CIELAB)%20&%20CIELAB%20to%20XYZ
 * WARNING: This function should be updated in the future when the real rule
 *   used by ArcGIS is found.
 */
std::list<double> CIELAB_to_RGB_HSV(double L, double a, double b, int type);

/**
 * Convert RGB color to HSV
 * \param R
 * \param G
 * \param B
 * \return a list with 3 components for HSV
 * NOTE: https://www.rapidtables.com/convert/color/rgb-to-hsv.html
 */
std::list<double> _RGB_to_HSV(double r, double g, double b);

/**
 * A private function used by conversion from Lab to RGB/HSV
 * \param factor t
 * \return result
 */
double _f(double t);

/**
 * A private function ensuring the range of RGB is between 0-255
 * \param one component from RGB
 * \return the component from 0-255
 */
double _format_rgb(double code);

#endif
