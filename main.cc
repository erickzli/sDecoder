#include <iostream>
#include "parser.hh"
#include "json_writer.hh"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << "Input Format:" << std::endl;
        std::cout << "./main [input_filename] [output_json_filename]" << std::endl;
        exit(1);
    }
    std::string infile_name = argv[1];
    std::string outfile_name = argv[2];

    std::ifstream infile;
    std::ofstream outfile;

    infile.open("BFiles/" + infile_name, std::ios::binary | std::ios::in);

    if (infile.is_open()) {
        // Check if the version of style file is parseable.
        int s = validateHexBlock(infile, "04E6147992C8D0", false);
        if (s == 1) {
            std::cout << "ERROR: Check style file version." << std::endl;
            outfile.write("ERROR\n", 7);
            outfile.close();
            return 1;
        } else {
            std::cout << "File version is validated." << std::endl;
        }

        outfile.open("Outfiles/" + outfile_name + ".json", std::ios::out);
        write_to_json(outfile, "", "{", 0, 2);

        // Move infile pointer 28 bytes to skip the header metadata.
        movePointer(infile, 28);

        parse_color(infile, outfile);
    }

    return 0;
}
