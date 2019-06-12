//
//  fill_parser.hh
//
//  Created by Erick Li on 05/29/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#ifndef __FILL_PARSER__
#define __FILL_PARSER__

#include <fstream>
#include <string>
#include <iostream>


int parseFillPattern(char **cursor, std::string &jstring, int level, char **tail);


/**
 * Parses the symbol flow, which is the largest and most essential pipeline in
 *   the model.
 * \param the input file stream.
 * \param the output file stream.
 * \param type of parser:
 *     0: Normal parser;
 *     1: Mask symbol parser.
 * \param the level(indention).
 */
int parseLayer(char **cursor, std::string &jstring, int type, int level);

/**
 * Parser the binary block where it defines a simple fill pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param type:
 *     0: Normal process;
 *     1: Symbol process.
 * \param the level(indention)
 */
int parseSimpleFill(char **cursor, std::string &jstring, int type, int level);

/**
 * Parser the binary block where it defines a line fill pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 */
int parseLineFill(char **cursor, std::string &jstring, int level);

/**
 * Parser the binary block where it defines a marker fill pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 */
int parseMarkerFill(char **cursor, std::string &jstring, int level);

#endif
