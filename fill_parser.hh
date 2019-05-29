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
