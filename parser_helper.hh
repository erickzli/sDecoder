#ifndef __PARSER_HELPER__
#define __PARSER_HELPER__

#include <fstream>
#include <string>
#include <iostream>

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
 * Rewind file pointer by rewindBytes.
 * @param the input file stream.
 * @param Bytes need to be rewinded (Number in POSITIVE)
 */
void goRewind(char **cursor, int rewindBytes);

/**
 * Move the file pointer bytesMoved bytes.
 * @param the input file stream.
 * @param the amount of bytes the pointer moves.
 */
void moveBytes(char **cursor, int bytesMoved);

/**
 * Get the character integer value (8-bit) from the current position.
 * @param the input file stream.
 * @return the integer-type value of the character gotten.
 */
int getChar(char **cursor);

int get16Bit(char **cursor);

/**
 * Get the integer value (32-bit) from the current position.
 * @param the input file stream.
 * @return the integer.
 */
int getInt(char **cursor);

/**
 * Get the double value from the current position.
 * @param the input file stream.
 * @return the double value.
 */
double getDouble(char **cursor);


#endif
