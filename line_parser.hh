//
//  line_parser.hh
//
//  Created by Erick Li on 04/16/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#ifndef __LINE_PARSER__
#define __LINE_PARSER__

#include <fstream>
#include <string>
#include <iostream>

/**
 * Parser the line patterns.
 * \param the input file stream.
 * \param the output file stream.
 * \param the type:
 *     0: Normal;
 *     1: Symbol.
 * \param the property, which can be `outline` etc.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseLinePattern(char **cursor, std::string &jstring, int type, std::string property, int level);

/**
 * Parser the Simple Line pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseSimpleLine(char **cursor, std::string &jstring, int level);

/**
 * Parser the Cartographic Line pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseCartoLine(char **cursor, std::string &jstring, int level);

/**
 * Parser the Hash Line pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseHashLine(char **cursor, std::string &jstring, int level);

/**
 * Parser the Marker Line pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseMarkerLine(char **cursor, std::string &jstring, int level);

/**
 * Parser the Line Cap pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseLineCaps(char **cursor, std::string &jstring, int level);

/**
 * Parser the Line Join pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseLineJoins(char **cursor, std::string &jstring, int level);

/**
 * Parser the line style pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseLineStyle(char **cursor, std::string &jstring, int level);

#endif
