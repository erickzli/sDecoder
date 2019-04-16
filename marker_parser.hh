#ifndef __MARKER_PARSER__
#define __MARKER_PARSER__

#include <fstream>
#include <string>
#include <iostream>

#include "parser.hh"

int parseCharacterMarker(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

int parseMaskTypes(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

int parseMarkerTypes(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

#endif
