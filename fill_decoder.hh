//
//  fill_decoder.hh
//
//  Created by Erick Li on 05/29/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#ifndef __FILL_DECODER__
#define __FILL_DECODER__

#include <fstream>
#include <string>
#include <iostream>

/**
 * The main decoding function for the fill pattern.
 * \param the input file stream.
 * \param the output JSON string.
 * \param the current indentation for the JSON instance.
 * \param a pointer of pointer pointing to the end of the file
 */
int decodeFillPattern(char **cursor, std::string &jstring, int level, char **tail);


/**
 * decodes the symbol flow, which is the largest and most essential pipeline in
 *   the model.
 * \param the input file stream.
 * \param the output file stream.
 * \param type of decoder:
 *     0: Normal decoder;
 *     1: Mask symbol decoder.
 * \param the level(indention).
 */
int decodeLayer(char **cursor, std::string &jstring, int type, int level);

/**
 * decoder the binary block where it defines a simple fill pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param type:
 *     0: Normal process;
 *     1: Symbol process.
 * \param the level(indention)
 */
int decodeSimpleFill(char **cursor, std::string &jstring, int type, int level);

/**
 * decoder the binary block where it defines a line fill pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 */
int decodeLineFill(char **cursor, std::string &jstring, int level);

/**
 * decoder the binary block where it defines a marker fill pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 */
int decodeMarkerFill(char **cursor, std::string &jstring, int level);

#endif
