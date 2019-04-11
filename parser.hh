#ifndef __PARSER__
#define __PARSER__

#include "parser_helper.hh"

/**
 * Parser the binary block where it defines a color pattern
 * @param the input file stream.
 */
int parse_color(std::ifstream &infile, std::ofstream &outfile);


/**
 * Parser the binary block where it defines a line pattern
 * @param the input file stream.
 */
int parse_line(std::ifstream &infile, std::ofstream &outfile);

/**
 * Parser the binary block where it defines a marker pattern
 * @param the input file stream.
 */
int parse_marker(std::ifstream &infile, std::ofstream &outfile);

/**
 * Parser the binary block where it defines a double value
 * @param the input file stream.
 */
int parse_double(std::ifstream &infile, std::ofstream &outfile);


#endif
