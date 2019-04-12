#ifndef __PARSER__
#define __PARSER__

#include "parser_helper.hh"

const bool PRINT_TO_FILE = true;
const bool DO_REWIND = true;

/**
 * Parses the symbol flow, which is the largest and most essential pipeline in
 *   the model.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention).
 * \param option for print to output file.
 */
int parseLayer(std::ifstream &infile, std::ofstream &outfile, int level, int layer_no, bool printToFile);

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
 * \param the level(indention)
 */
int parseSimpleFill(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

/**
 * Parser the binary block where it defines a line fill pattern
 * \param the input file stream.
 * \param the level(indention)
 */
int parseLineFill(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

/**
 * Parser the binary block where it defines a marker fill pattern
 * \param the input file stream.
 * \param the level(indention)
 */
int parseMarkerFill(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

/**
 * Parser the binary block where it defines a double value
 * \param the input file stream.
 * \param the level(indention)
 */
double parseDouble(std::ifstream &infile, std::ofstream &outfile, std::string tag, int level, bool printToFile);

int parseLinePattern(std::ifstream &infile, std::ofstream &outfile, std::string property, int level, bool printToFile);

int parseSimpleLine(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

int parseCartoLine(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

int parseHashLine(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

int parseMarkerLine(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

int parseLineStyle(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

int parseTailPattern(std::ifstream &infile, int type);

#endif
