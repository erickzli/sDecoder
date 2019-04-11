#ifndef __PARSER__
#define __PARSER__

#include "parser_helper.hh"

const bool PRINT_TO_FILE = true;

/**
 * Parses the symbol flow, which is the largest and most essential pipeline in
 *   the model.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention).
 * \param option for print to output file.
 */
int parse_layer(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

/**
 * Parses the binary block where it defines a color pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param color type, such as, "outline color", etc.
 * \param the level(indention).
 * \param option for print to output file.
 */
int parse_color(std::ifstream &infile, std::ofstream &outfile, std::string color_type,
                int level, bool printToFile);

/**
 * Parses out layer number.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention).
 * \param option for print to output file.
 * \return:
 *    val > 0: the number of layers
 *    val == -1: something wrong...
 */
int parse_layer_number(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);


/**
 * Parser the binary block where it defines a simple fill pattern
 * \param the input file stream.
 * \param the level(indention)
 */
int parse_simple_fill(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

/**
 * Parser the binary block where it defines a line fill pattern
 * \param the input file stream.
 * \param the level(indention)
 */
int parse_line_fill(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

/**
 * Parser the binary block where it defines a marker fill pattern
 * \param the input file stream.
 * \param the level(indention)
 */
int parse_marker_fill(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

/**
 * Parser the binary block where it defines a double value
 * \param the input file stream.
 * \param the level(indention)
 */
int parse_double(std::ifstream &infile, std::ofstream &outfile, int level);


#endif
