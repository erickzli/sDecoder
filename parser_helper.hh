//
//  parser_helper.hh
//
//  Created by Erick Li on 04/10/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#ifndef __PARSER_HELPER__
#define __PARSER_HELPER__

#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>

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
 * Print "num" bytes of the binary file from the current position. The cursor will be
 *   automatically rewinded.
 * @param the input file stream.
 * @param number of bytes to be printed.
 */
void printHex(char **cursor, int num);


#endif
