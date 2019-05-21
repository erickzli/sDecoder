//
//  marker_parser.hh
//
//  Created by Erick Li on 04/16/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#ifndef __MARKER_PARSER__
#define __MARKER_PARSER__

#include <fstream>
#include <string>
#include <iostream>


/**
 * Parser the marker pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseMarkerPattern(char **cursor, std::string &jstring, int level);

/**
 * Parser the simple marker pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseSimpleMarker(char **cursor, std::string &jstring, int level, bool withTail);

/**
 * Parser the character marker pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseCharacterMarker(char **cursor, std::string &jstring, int level, bool withTail);

/**
 * Parser the arrow marker pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseArrowMarker(char **cursor, std::string &jstring, int level, bool withTail);

/**
 * Parser the type of markers (Grid or random)
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseMarkerTypes(char **cursor, std::string &jstring, int level);

/**
 * Parser the type of masks (None or Halo)
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseMaskTypes(char **cursor, std::string &jstring, int level);

int parseMarkerStyle(char **cursor, std::string &jstring, int level);

#endif
