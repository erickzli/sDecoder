#ifndef __LINE_PARSER__
#define __LINE_PARSER__

#include <fstream>
#include <string>
#include <iostream>

#include "parser.hh"

// 0: normal; 1: symbol
int parseLinePattern(std::ifstream &infile, std::ofstream &outfile, int type, std::string property, int level, bool printToFile);

int parseSimpleLine(std::ifstream &infile, std::ofstream &outfile, int type, int level, bool printToFile);

int parseCartoLine(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

int parseHashLine(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

int parseMarkerLine(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

int parseLineCaps(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

int parseLineJoins(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

int parseLineStyle(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile);

#endif
