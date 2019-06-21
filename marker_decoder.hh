//
//  marker_decoder.hh
//
//  Created by Erick Li on 04/16/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#ifndef __MARKER_DECODER__
#define __MARKER_DECODER__

#include <fstream>
#include <string>
#include <iostream>


/**
 * decoder the marker pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 */
int decodeMarkerPattern(char **cursor, std::string &jstring, int level);

/**
 * decoder the simple marker pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 */
int decodeSimpleMarker(char **cursor, std::string &jstring, int level);

/**
 * decoder the character marker pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 */
int decodeCharacterMarker(char **cursor, std::string &jstring, int level);

/**
 * decoder the arrow marker pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 */
int decodeArrowMarker(char **cursor, std::string &jstring, int level);

/**
 * decoder the type of markers (Grid or random)
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 */
int decodeMarkerTypes(char **cursor, std::string &jstring, int level);

/**
 * decoder the type of masks (None or Halo)
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 */
int decodeMaskTypes(char **cursor, std::string &jstring, int level);

/**
 * decoder the type of marker
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 */
int decodeMarkerStyle(char **cursor, std::string &jstring, int level);

#endif
