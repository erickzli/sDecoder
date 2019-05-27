//
//  main.cc
//
//  Created by Erick Li on 04/10/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#include <iostream>
#include <list>
#include <string>

#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>

#include "parser.hh"
#include "json_writer.hh"

/**
 * Print out the instruction for formatting.
 */
void printFormat() {
    std::clog << "usage: ./main [-f input_filename] [-p print_log (true/false)]" << std::endl;
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
        if (!strcmp(argv[i], "-f")) {
            filename = std::string(argv[i + 1]);
        // Check the option of logging.
    } else if (!strcmp(argv[i], "-p")) {
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

    if (filename == ".") {
        std::clog << "This is test mode..." << std::endl;
        sleep(1); // Sleep 1 second for user experience

        int passed = 0;
        int failed = 0;

        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir ("Test")) == NULL) {
            /* could not open directory */
            std::clog << "TEST MODE: You need to create a directory called exactly \"Test\"..." << std::endl;
            return -1;
        }

        if (!enableLog) {
            std::clog << "TEST MODE: Logging is automatically on under test mode..." << std::endl;
        }

        std::list<std::string> test_files;
        int num_of_files = 0;

        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL) {
            std::string fname(ent->d_name);
            // Ignore current directory and parent directory..
            if (fname[0] != '.') {
                test_files.push_back(fname);
                num_of_files += 1;
            }
        }

        if (0 == num_of_files) {
            std::clog << "TEST MODE: No files in the Test folder..." << std::endl;
            return -1;
        }

        std::list<std::string>::iterator trav;
        test_files.sort();
        std::clog << "TEST MODE: Test summary" << std::endl;
        std::clog << "------------------------" << std::endl;

        // Traverse each files, and find which one passed and which one failed
        for (trav = test_files.begin(); trav != test_files.end(); trav++) {
            FILE *infile;
            int ch;
            size_t idx = 0;
            const char *filepath = ("Test/" + *trav).c_str();

            // Open file
            infile = fopen(filepath, "r");
            // The case that the file is not existed or cannot be opened.
            if (infile == NULL) {
                std::clog << "\033[1;31m" + *trav + "\033[0m";
                std::clog << " -> ERROR: Cannot open the file" << std::endl;
                continue;
            }

            // Get the size of the file to reserve enough memory.
            fseek(infile, 0L, SEEK_END);
            size_t file_size = ftell(infile);
            rewind(infile);

            // Malloc enough memory for the binary file.
            char *input = (char *)malloc(file_size + 1);
            char *save = input;

            // Read the file into the memory
            while ((ch = fgetc(infile)) != EOF) {
                input[idx++] = (char) ch;
            }
            input[idx] = '\0';

            // Underlying buffer.
            std::streambuf* orig_buf;
            // Get underlying buffer
            orig_buf = std::clog.rdbuf();
            // Set NULL
            std::clog.rdbuf(NULL);

            // The most important part of the code.
            std::string jstring = grandParser(&input);

            // However, we still want the results being output, don't we?
            std::clog.rdbuf(orig_buf);

            // Close the file...
            fclose(infile);
            free(save);

            // failed
            if ('e' == jstring[1]) {
                std::clog << "\033[1;31m" + *trav + "\033[0m -> " << jstring;
                failed += 1;
            } else { // passed
                std::clog << "\033[1;32m" + *trav + "\033[0m\n";
                passed += 1;
            }
        }

        std::clog << "----------------------" << std::endl;
        std::clog << "Passed: " << passed << std::endl;
        std::clog << "Failed: " << failed << std::endl;

        closedir (dir);

        return 0;
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
        std::clog << "ERROR: Cannot find file \"" << filename << "\"." << std::endl;
        return -1;
    }

    // Get the size of the file to reserve enough memory.
    fseek(infile, 0L, SEEK_END);
    size_t file_size = ftell(infile);
    rewind(infile);

    // Malloc enough memory for the binary file.
    char *input = (char *)malloc(file_size + 1);
    char *save = input;

    // Read the file into the memory
    while ((ch = fgetc(infile)) != EOF) {
        input[idx++] = (char) ch;
    }
    input[idx] = '\0';

    // Underlying buffer.
    std::streambuf* orig_buf;

    // If logging is disabled, then we need to close clog for logging.
    if (!enableLog) {
        // Get underlying buffer
        orig_buf = std::clog.rdbuf();
        // Set NULL
        std::clog.rdbuf(NULL);
    }

    // The most important part of the code.
    std::string jstring = grandParser(&input);

    // However, we still want the results being output, don't we?
    if (!enableLog) {
        std::clog.rdbuf(orig_buf);
    }

    std::clog << std::endl << "~~~~~~ Beginning of the jstring ~~~~~~" << std::endl << std::endl;
    // clog the JSON string gotten through the grand parser.
    std::clog << jstring << std::endl;
    std::clog << "~~~~~~~~~~~ End of jstring ~~~~~~~~~~~" << std::endl;

    // Close the file...
    fclose(infile);
    free(save);

    return 0;
}
