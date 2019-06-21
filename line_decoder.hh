//
//  line_decoder.hh
//
//  Created by Erick Li on 04/16/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#ifndef __LINE_DECODER__
#define __LINE_DECODER__

#include <fstream>
#include <string>
#include <iostream>

/**
 * decoder the line patterns.
 * \param the input file stream.
 * \param the output file stream.
 * \param the type:
 *     0: Normal;
 *     1: Symbol.
 * \param the property, which can be `outline` etc.
 * \param the level(indention)
 */
int decodeLinePattern(char **cursor, std::string &jstring, int type, std::string property, int level);

/**
 * decoder the Simple Line pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 */
int decodeSimpleLine(char **cursor, std::string &jstring, int level);

/**
 * decoder the Cartographic Line pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 */
int decodeCartoLine(char **cursor, std::string &jstring, int level);

/**
 * decoder the Hash Line pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 */
int decodeHashLine(char **cursor, std::string &jstring, int level);

/**
 * decoder the Marker Line pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 */
int decodeMarkerLine(char **cursor, std::string &jstring, int level);

/**
 * decoder the Line Cap pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 */
int decodeLineCaps(char **cursor, std::string &jstring, int level);

/**
 * decoder the Line Join pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 */
int decodeLineJoins(char **cursor, std::string &jstring, int level);

/**
 * decoder the line style pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 */
int decodeLineStyle(char **cursor, std::string &jstring, int level);

/**
 * decoder the TEMPLATE pattern.
 *     TEMPLATE is for line feature which defines dot repeats.
 *     interval length -> line length -> gap length.
 * \param the input file stream.
 * \param the output file stream.
 * \param type type of template feed (0: hash/carto line; 1: marker line)
 * \param the level(indention)
 */
int decodeTemplate(char **cursor, std::string &jstring, int type, int level);

#endif
