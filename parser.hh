//
//  parser.hh
//
//  Created by Erick Li on 04/11/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#ifndef __PARSER__
#define __PARSER__

#include "parser_helper.hh"
#include "line_parser.hh"
#include "marker_parser.hh"
#include "json_writer.hh"

const bool PRINT_TO_FILE = true;
const bool DO_REWIND = true;


std::string grandParser(char **input);

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
int parseLayer(char **cursor, std::string &jstring, int type, int level, int layer_no);

/**
 * Parses the binary block where it defines a color pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param color type, such as, "outline color", etc.
 * \param the level(indention).
 */
int parseColorPattern(char **cursor, std::string &jstring, std::string color_type,
                int level);

/**
 * Parses out layer number.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention).
 * \return:
 *    val > 0: the number of layers
 *    val == -1: something wrong...
 */
int parseLayerNumber(char **cursor, std::string &jstring, int level);


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

/**
 * Parser the binary block where it defines a double value
 * \param the input file stream.
 * \param the output file stream.
 * \param the name of the double value.
 * \param the level(indention)
 */
double parseDouble(char **cursor, std::string &jstring, std::string tag, int level);

/**
 * Parser the binary block where it defines an integer value
 * \param the input file stream.
 * \param the output file stream.
 * \param the name of the double value.
 * \param the level(indention)
 */
int parseInt(char **cursor, std::string &jstring, std::string tag, int level);

/**
 * Parser the binary block where it defines a string (font name)
 * \param the input file stream.
 * \param the output file stream.
 * \param the name of the double value.
 * \param the level(indention)
 */
int parseString(char **cursor, std::string &jstring, std::string tag, int level);

/**
 * Parser the TEMPLATE pattern.
 *     TEMPLATE is for line feature which defines dot repeats.
 *     interval length -> line length -> gap length.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 */
int parseTemplate(char **cursor, std::string &jstring, int type, int level);

#endif
