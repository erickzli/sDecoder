#include <iostream>
#include "parser.hh"
#include "json_writer.hh"

int main(int argc, char **argv) {
    if (argc <= 1) {
        std::cout << "Input Format:" << std::endl;
        std::cout << "./main [input_filename]" << std::endl;
        exit(1);
    }

    FILE *infile;
    int ch;
    size_t idx = 0;
    std::string fn = "BFiles/" + std::string(argv[1]);
    const char *filename = fn.c_str();

    infile = fopen(filename, "r");

    fseek(infile, 0L, SEEK_END);
    size_t file_size = ftell(infile);
    rewind(infile);

    char *input = (char *)malloc(file_size + 1);

    while ((ch = fgetc(infile)) != EOF) {
        input[idx++] = (char) ch;
    }

    input[idx] = '\0';

    std::string jstring = grandParser(&input);

    std::cout << jstring << std::endl;

    return 0;
}
