#ifndef __HEX_PARSER__
#define __HEX_PARSER__

#include <fstream>

using namespace std;

/**
 * Move the file pointer bytesMoved bytes.
 * @param the input file stream.
 * @param the amount of bytes the pointer moves.
 */
void movePointer(ifstream &myfile, int bytesMoved);

/**
 * Get the character integer value (8-bit) from the current position.
 * @param the input file stream.
 * @return the integer-type value of the character gotten.
 */
int getChar(ifstream &myfile);

/**
 * Get the integer value (32-bit) from the current position.
 * @param the input file stream.
 * @return the integer.
 */
int getInt(ifstream &myfile);

/**
 * Get the double value from the current position.
 * @param the input file stream.
 * @return the double value.
 */
double getDouble(ifstream &myfile);


#endif
