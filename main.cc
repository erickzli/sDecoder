#include <iostream>
#include "parser.hh"
#include "json_writer.hh"

void printFormat() {
    std::cout << "usage: ./main [-F input_filename] [-P print_log (true/false)]" << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        printFormat();
        exit(1);
    }

    if (argc % 2 != 1) {
        printFormat();
        exit(1);
    }

    std::string filename;
    bool enableLog = true;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-F")) {
            filename = std::string(argv[i + 1]);
        } else if (!strcmp(argv[i], "-P")) {
            if (!strcmp(argv[i + 1], "false")) {
                enableLog = false;
            }
        } else {
            printFormat();
            exit(1);
        }
        i += 1;
    }

    FILE *infile;
    int ch;
    size_t idx = 0;
    std::string fn = "BFiles/" + filename;
    const char *filepath = fn.c_str();

    infile = fopen(filepath, "r");
    if (infile == NULL) {
        std::cout << "ERROR: Cannot find file \"" << filename << "\"." << std::endl;
        exit(1);
    }

    fseek(infile, 0L, SEEK_END);
    size_t file_size = ftell(infile);
    rewind(infile);

    char *input = (char *)malloc(file_size + 1);

    while ((ch = fgetc(infile)) != EOF) {
        input[idx++] = (char) ch;
    }

    input[idx] = '\0';

    std::streambuf* orig_buf;

    if (!enableLog) {
        // Get underlying buffer
        orig_buf = std::cout.rdbuf();
        // Set NULL
        std::cout.rdbuf(NULL);
    }

    std::string jstring = grandParser(&input);

    if (!enableLog) {
        std::cout.rdbuf(orig_buf);
    }

    std::cout << jstring << std::endl;

    fclose(infile);

    return 0;
}
