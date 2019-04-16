#ifndef __LINE_PARSER__
#define __LINE_PARSER__

#include <fstream>
#include <string>
#include <iostream>

#include "parser.hh"

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
int parseLinePattern(std::ifstream &infile, std::ofstream &outfile, int type, std::string property, int level, bool printToFile);

/**
 * Parser the Simple Line pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseSimpleLine(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

/**
 * Parser the Cartographic Line pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseCartoLine(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

/**
 * Parser the Hash Line pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseHashLine(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

/**
 * Parser the Marker Line pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseMarkerLine(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

/**
 * Parser the Line Cap pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseLineCaps(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

/**
 * Parser the Line Join pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseLineJoins(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

/**
 * Parser the line style pattern.
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseLineStyle(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

#endif
