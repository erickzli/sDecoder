#include "parser.hh"
#include "json_writer.hh"

int parse_layer(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile) {
    std::cout << "-------------------------------" << std::endl;
    std::cout << "START parsing a symbol/layer..." << std::endl;

    int filling_type = getChar(infile);
    std::string filling_type_name = "";

    if (3 == filling_type) {
        filling_type_name = "Simple";
    } else if (6 == filling_type) {
        filling_type_name = "Line";
    } else if (8 == filling_type) {
        filling_type_name = "Marker";
    } else {
        std::cout << "ERROR: Filling type not support or the file has problems." << std::endl;
        return -1;
    }

    if (3 == filling_type) {
        parse_simple_fill(infile, outfile, level, PRINT_TO_FILE);
    } else if (6 == filling_type) {
        parse_line_fill(infile, outfile, level, PRINT_TO_FILE);
    } else if (8 == filling_type) {
        parse_marker_fill(infile, outfile, level, PRINT_TO_FILE);
    }

    return 0;
}

int parse_color(std::ifstream &infile, std::ofstream &outfile, std::string color_type,
                int level, bool printToFile) {
    std::cout << "------------------------" << std::endl;
    std::cout << "START parsing color..." << std::endl;
    int color_space = getChar(infile);

    // CMYK color space...
    if (151 == color_space) {
        std::cout << "Color Space: CMYK." << std::endl;

        movePointer(infile, 19);
        int c = getChar(infile);
        int m = getChar(infile);
        int y = getChar(infile);
        int k = getChar(infile);

        std::cout << "C: " << c << std::endl;
        std::cout << "M: " << m << std::endl;
        std::cout << "Y: " << y << std::endl;
        std::cout << "K: " << k << std::endl;

        if (printToFile) {
            write_to_json(outfile, color_type, "{", level);
            write_to_json(outfile, "Color Space", "\"CMYK\",", level + 1);
            write_to_json(outfile, "C", std::to_string(c) + ",", level + 1);
            write_to_json(outfile, "M", std::to_string(m) + ",", level + 1);
            write_to_json(outfile, "Y", std::to_string(y) + ",", level + 1);
            write_to_json(outfile, "K", std::to_string(k), level + 1);
            write_to_json(outfile, "", "},", level);
        }
    } else {
        // HSV color space...
        if (146 == color_space) {
            std::cout << "Color Space: HSV." << std::endl;
        // RGB color space...
    } else if (150 == color_space) {
            std::cout << "Color Space: RGB." << std::endl;
        } else {
            std::cout << "ERROR: Color Space not found." << std::endl;
            return -1;
        }
        movePointer(infile, 20);
        double first = getDouble(infile);
        double second = getDouble(infile);
        double third = getDouble(infile);

        std::cout << "1st field: " << first << std::endl;
        std::cout << "2nd field: " << second << std::endl;
        std::cout << "3rd field: " << third << std::endl;

        if (printToFile) {
            write_to_json(outfile, color_type, "{", level);
            if (146 == color_space) {
                write_to_json(outfile, "Color Space", "\"HSV\",", level + 1);
            } else if (150 == color_space) {
                write_to_json(outfile, "Color Space", "\"RGB\",", level + 1);
            }
            write_to_json(outfile, "First Field", std::to_string(first) + ",", level + 1);
            write_to_json(outfile, "Second Field", std::to_string(second) + ",", level + 1);
            write_to_json(outfile, "Third Field", std::to_string(third), level + 1);
            write_to_json(outfile, "", "},", level);
        }
    }

    movePointer(infile, 2);

    return 0;
}

int parse_layer_number(std::ifstream &infile, std::ofstream &outfile,
                       int level, bool printToFile) {
    std::cout << "-----------------------------" << std::endl;
    std::cout << "START parsing layer number" << std::endl;
    int num_of_layers = getChar(infile);
    std::cout << "The number of layer is: " << num_of_layers << std::endl;

    // Check whether the layer number is normal...
    if (0 > num_of_layers || 5 < num_of_layers) {
        std::cout << "WARNING: Layer number is abnormal." << std::endl;
        return -1;
    }

    if (printToFile) {
        write_to_json(outfile, "numberOfLayers", std::to_string(num_of_layers) + ",", level);
    }

    movePointer(infile, 3);

    return num_of_layers;
}


int parse_simple_fill(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile) {
    std::cout << "Filling type: " << filling_type_name << std::endl;
    if (printToFile) {
        write_to_json(outfile, "fillingType", "\"Simple Fill\",", level);
    }

    return 0;
}


int parse_line_fill(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile) {
    return 0;
}

int parse_marker_fill(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile) {
    return 0;
}

int parse_double(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile) {
    return 0;
}
