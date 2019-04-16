#ifndef __MARKER_PARSER__
#define __MARKER_PARSER__

#include <fstream>
#include <string>
#include <iostream>

#include "parser.hh"

/**
 * Parser the character marker pattern
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseCharacterMarker(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

/**
 * Parser the type of markers (Grid or random)
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseMarkerTypes(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

/**
 * Parser the type of masks (None or Halo)
 * \param the input file stream.
 * \param the output file stream.
 * \param the level(indention)
 * \param whether print to JSON file.
 */
int parseMaskTypes(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

#endif
