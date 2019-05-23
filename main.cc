//
//  main.cc
//
//  Created by Erick Li on 04/10/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//


#include <iostream>
#include "parser.hh"
#include "json_writer.hh"

/**
 * Print out the instruction for formatting.
 */
void printFormat() {
    std::cout << "usage: ./main [-F input_filename] [-P print_log (true/false)]" << std::endl;
}

int main(int argc, char *argv[]) {
    // When there are too few arguments.
    if (argc <= 1) {
        printFormat();
        return -1;
    }

    // When the number of arguments is not odd (including ./main)
    if (argc % 2 != 1) {
        printFormat();
        return -1;
    }

    // The name of the input file.
    std::string filename;

    // Option to enable or disable logging for the information of parsing processes.
    bool enableLog = true;

    // Check each argument.
    for (int i = 1; i < argc; i++) {
        // Check filename.
        if (!strcmp(argv[i], "-F")) {
            filename = std::string(argv[i + 1]);
        // Check the option of logging.
        } else if (!strcmp(argv[i], "-P")) {
            if (!strcmp(argv[i + 1], "false")) {
                enableLog = false;
            }
        // If the argument flag is not shown above, then the format has some problems.
        } else {
            printFormat();
            return -1;
        }
        i += 1;
    }

    // File pointer
    FILE *infile;
    // One byte in the file.
    int ch;
    // Index
    size_t idx = 0;
    // Relavant path of the input file.
    const char *filepath = ("BFiles/" + filename).c_str();

    // Open file
    infile = fopen(filepath, "r");
    // The case that the file is not existed or cannot be opened.
    if (infile == NULL) {
        std::cout << "ERROR: Cannot find file \"" << filename << "\"." << std::endl;
        return -1;
    }

    // Get the size of the file to reserve enough memory.
    fseek(infile, 0L, SEEK_END);
    size_t file_size = ftell(infile);
    rewind(infile);

    // Malloc enough memory for the binary file.
    char *input = (char *)malloc(file_size + 1);

    // Read the file into the memory
    while ((ch = fgetc(infile)) != EOF) {
        input[idx++] = (char) ch;
    }
    input[idx] = '\0';

    // Underlying buffer.
    std::streambuf* orig_buf;

    // If logging is disabled, then we need to close cout for logging.
    if (!enableLog) {
        // Get underlying buffer
        orig_buf = std::cout.rdbuf();
        // Set NULL
        std::cout.rdbuf(NULL);
    }

    // The most important part of the code.
    std::string jstring = grandParser(&input);

    // However, we still want the results being output, don't we?
    if (!enableLog) {
        std::cout.rdbuf(orig_buf);
    }

    std::cout << std::endl << "~~~~~~ Beginning of the jstring ~~~~~~" << std::endl << std::endl;
    // Cout the JSON string gotten through the grand parser.
    std::cout << jstring << std::endl;
    std::cout << "~~~~~~~~~~~ End of jstring ~~~~~~~~~~~" << std::endl;

    // Close the file...
    fclose(infile);

    return 0;
}
