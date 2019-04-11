#include "parser.hh"

int parse_color(std::ifstream &infile, std::ofstream &outfile) {
    std::cout << "START parsing color..." << std::endl;
    int color_space = getChar(infile);

    // CMYK color space...
    if (97 == color_space) {
        std::cout << "Color Space: CMYK." << std::endl;

        movePointer(infile, 19);

    }


}

int parse_line(std::ifstream &infile, std::ofstream &outfile) {

}

int parse_marker(std::ifstream &infile, std::ofstream &outfile) {

}

int parse_double(std::ifstream &infile, std::ofstream &outfile) {

}
