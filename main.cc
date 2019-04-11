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
        int s = hexValidation(infile, "04E6147992C8D011", true);
        if (s == 1) {
            std::cout << "ERROR: Check style file version." << std::endl;
            outfile.write("ERROR\n", 7);
            outfile.close();
            exit(1);
        } else {
            std::cout << "File version is validated." << std::endl;
        }

        outfile.open("Outfiles/" + outfile_name + ".json", std::ios::out);

        write_to_json(outfile, "", "{", 0);
        // Move infile pointer 26 bytes to skip the header metadata.
        movePointer(infile, 26);
        // Parse the first color pattern. Usage so far is unknown...
        parse_color(infile, outfile, "unknownColor", 1, PRINT_TO_FILE);
        // Parse out the number of layers...
        int num_of_layers = parse_layer_number(infile, outfile, 1, PRINT_TO_FILE);
        // Test if the number of layers behaves weird...
        if (-1 == num_of_layers)
            exit(1);

        // Start parsing each layer.
        for (int i = 0; i < num_of_layers; i++) {
            write_to_json(outfile, "layerNumber", std::to_string(i) + ",", 1);
            parse_layer(infile, outfile, 1, PRINT_TO_FILE);
        }

        write_to_json(outfile, "", "}", 0);
    }

    infile.close();
    outfile.close();

    return 0;
}
