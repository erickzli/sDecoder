#ifndef __PARSER__
#define __PARSER__

#include "parser_helper.hh"
#include "line_parser.hh"
#include "marker_parser.hh"
#include "json_writer.hh"

const bool PRINT_TO_FILE = true;
const bool DO_REWIND = true;

/**
 * Parses the symbol flow, which is the largest and most essential pipeline in
 *   the model.
 * \param the input file stream.
 * \param the output file stream.
 * \param type of parser:
 *     0: Normal parser;
 *     1: Symbol parser.
 * \param the level(indention).
 * \param option for print to output file.
 */
int parseLayer(std::ifstream &infile, std::ofstream &outfile, int type, int level, int layer_no, bool printToFile);

/**
 * Parses the binary block where it defines a color pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param color type, such as, "outline color", etc.
 * \param the level(indention).
 * \param option for print to output file.
 */
int parseColorPattern(std::ifstream &infile, std::ofstream &outfile, std::string color_type,
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
int parseLayerNumber(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);


/**
 * Parser the binary block where it defines a simple fill pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param type:
 *     0: Normal process;
 *     1: Symbol process.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseSimpleFill(std::ifstream &infile, std::ofstream &outfile, int type, int level, bool printToFile);

/**
 * Parser the binary block where it defines a line fill pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseLineFill(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

/**
 * Parser the binary block where it defines a marker fill pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseMarkerFill(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

/**
 * Parser the binary block where it defines a double value
 * \param the input file stream.
 * \param the output file stream.
 * \param the name of the double value.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
double parseDouble(std::ifstream &infile, std::ofstream &outfile, std::string tag, int level, bool printToFile);

/**
 * Parser the binary block where it defines an integer value
 * \param the input file stream.
 * \param the output file stream.
 * \param the name of the double value.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseInt(std::ifstream &infile, std::ofstream &outfile, std::string tag, int level, bool printToFile);

/**
 * Parser the binary block where it defines a string (font name)
 * \param the input file stream.
 * \param the output file stream.
 * \param the name of the double value.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseString(std::ifstream &infile, std::ofstream &outfile, std::string tag, int level, bool printToFile);

/**
 * Parser the TEMPLATE (check the diagram)
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseTemplate(std::ifstream &infile, std::ofstream &outfile, int type, int level, bool printToFile);

#endif
