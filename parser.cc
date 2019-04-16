#include "parser.hh"

int parseLayer(std::ifstream &infile, std::ofstream &outfile, int type, int level, int layer_no, bool printToFile) {
    std::cout << "-------------------------------" << std::endl;
    std::cout << "START parsing a symbol/layer..." << std::endl;

    if (type == 0) {
        write_to_json(outfile, "layer", "{", level);
        write_to_json(outfile, "number", std::to_string(layer_no) + ",", level + 1);
    } else {
        write_to_json(outfile, "fillSymbol", "{", level);
    }


    int filling_type = getChar(infile);
    std::string filling_type_name = "";

    if (3 == filling_type) {
        parseSimpleFill(infile, outfile, type, level + 1, PRINT_TO_FILE);
    } else if (6 == filling_type) {
        parseLineFill(infile, outfile, level + 1, PRINT_TO_FILE);
    } else if (8 == filling_type) {
        parseMarkerFill(infile, outfile, level + 1, PRINT_TO_FILE);
    } else {
        std::cout << "ERROR: Filling type " << filling_type << " not support" << std::endl;
        return -1;
    }

    if (type == 0) {
        // Parse the tail pattern of the layer...
        moveBytes(infile, 26);
    } else {
        moveBytes(infile, 17);
    }

    write_to_json(outfile, "", "},", level);

    return 0;
}

int parseColorPattern(std::ifstream &infile, std::ofstream &outfile, std::string color_type, int level, bool printToFile) {
    std::cout << "----------------------" << std::endl;
    std::cout << "START parsing color..." << std::endl;
    int color_space = getChar(infile);

    if (printToFile) {
        write_to_json(outfile, "color", "{", level);
        write_to_json(outfile, "name", "\"" + color_type + "\",", level + 1);
    }

    // CMYK color space...
    if (151 == color_space) {
        std::cout << "Color Space: CMYK." << std::endl;

        moveBytes(infile, 19);
        int c = getChar(infile);
        int m = getChar(infile);
        int y = getChar(infile);
        int k = getChar(infile);

        std::cout << "C: " << c << std::endl;
        std::cout << "M: " << m << std::endl;
        std::cout << "Y: " << y << std::endl;
        std::cout << "K: " << k << std::endl;

        if (printToFile) {
            write_to_json(outfile, "space", "\"CMYK\",", level + 1);
            write_to_json(outfile, "C", std::to_string(c) + ",", level + 1);
            write_to_json(outfile, "M", std::to_string(m) + ",", level + 1);
            write_to_json(outfile, "Y", std::to_string(y) + ",", level + 1);
            write_to_json(outfile, "K", std::to_string(k), level + 1);
        }
    } else {
        // HSV color space...
        if (146 == color_space) {
            std::cout << "Color Space: HSV." << std::endl;
        // RGB color space...
    } else if (150 == color_space) {
            std::cout << "Color Space: RGB." << std::endl;
        } else {
            std::cout << "ERROR: Color Space " << std::to_string(color_space) << " not found." << std::endl;
            return -1;
        }
        moveBytes(infile, 20);
        double first = getDouble(infile);
        double second = getDouble(infile);
        double third = getDouble(infile);

        std::cout << "1st field: " << first << std::endl;
        std::cout << "2nd field: " << second << std::endl;
        std::cout << "3rd field: " << third << std::endl;

        if (printToFile) {
            if (146 == color_space) {
                write_to_json(outfile, "space", "\"HSV\",", level + 1);
            } else if (150 == color_space) {
                write_to_json(outfile, "space", "\"RGB\",", level + 1);
            }
            write_to_json(outfile, "firstField", std::to_string(first) + ",", level + 1);
            write_to_json(outfile, "secondField", std::to_string(second) + ",", level + 1);
            write_to_json(outfile, "thirdField", std::to_string(third), level + 1);
        }
    }

    if (printToFile) {
        write_to_json(outfile, "", "},", level);
    }

    moveBytes(infile, 2);

    return 0;
}

int parseLayerNumber(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile) {
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

    moveBytes(infile, 3);

    return num_of_layers;
}


int parseSimpleFill(std::ifstream &infile, std::ofstream &outfile, int type, int level, bool printToFile) {
    std::cout << "---------------------------" << std::endl;
    std::cout << "Filling type: Simple Fill" << std::endl;
    if (printToFile) {
        write_to_json(outfile, "fillingType", "\"Simple Fill\",", level);
    }

    // Validate if the header is there.
    if (0 != hexValidation(infile, "E6147992C8D0118BB6080009EE4E41", !DO_REWIND)) {
        std::cout << "ERROR: Fail to validate Simple Fill pattern header..." << std::endl;
        return -1;
    }
    moveBytes(infile, 2);

    if (type == 0) {
        parseLinePattern(infile, outfile, 0, "Outline", level, PRINT_TO_FILE);
    } else {
        parseLinePattern(infile, outfile, 0, "Filling Line", level, PRINT_TO_FILE);
    }

    parseColorPattern(infile, outfile, "Filling Color", level, PRINT_TO_FILE);

    return 0;
}


int parseLineFill(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile) {
    std::cout << "---------------------------" << std::endl;
    std::cout << "Filling type: Line Fill" << std::endl;
    if (printToFile) {
        write_to_json(outfile, "fillingType", "\"Line Fill\",", level);
    }

    // Validate if the header is there.
    if (0 != hexValidation(infile, "E6147992C8D0118BB6080009EE4E41", !DO_REWIND)) {
        std::cout << "ERROR: Fail to validate Line Fill pattern header..." << std::endl;
        return -1;
    }
    moveBytes(infile, 18);

    parseLinePattern(infile, outfile, 0, "Filling Line", level, printToFile);
    parseLinePattern(infile, outfile, 0, "Outline", level, printToFile);
    parseDouble(infile, outfile, "angle", level, printToFile);
    parseDouble(infile, outfile, "Offset", level, printToFile);
    parseDouble(infile, outfile, "separation", level, printToFile);

    return 0;
}

int parseMarkerFill(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile) {
    std::cout << "---------------------------" << std::endl;
    std::cout << "Filling type: Marker Fill" << std::endl;
    if (printToFile) {
        write_to_json(outfile, "fillingType", "\"Marker Fill\",", level);
    }

    // Validate if the header is there.
    if (0 != hexValidation(infile, "E6147992C8D0118BB6080009EE4E41", !DO_REWIND)) {
        std::cout << "ERROR: Fail to validate Marker Fill pattern header..." << std::endl;
        return -1;
    }
    moveBytes(infile, 2);

    parseMarkerTypes(infile, outfile, level, printToFile);
    parseDouble(infile, outfile, "fillPropertiesOffsetX", level, printToFile);
    parseDouble(infile, outfile, "fillPropertiesOffsetY", level, printToFile);
    parseDouble(infile, outfile, "fillPropertiesSeparationX", level, printToFile);
    parseDouble(infile, outfile, "fillPropertiesSeparationY", level, printToFile);
    moveBytes(infile, 16);

    parseCharacterMarker(infile, outfile, level, printToFile); // TODO...

    parseLinePattern(infile, outfile, 0, "Outline", level, printToFile);

    return 0;
}

double parseDouble(std::ifstream &infile, std::ofstream &outfile, std::string tag, int level, bool printToFile) {
    double val = getDouble(infile);
    std::cout << tag + " is: " + std::to_string(val) << std::endl;
    if (printToFile) {
        write_to_json(outfile, tag, std::to_string(val) + ",", level);
    }

    return val;
}

int parseInt(std::ifstream &infile, std::ofstream &outfile, std::string tag, int level, bool printToFile) {
    int val = getInt(infile);
    std::cout << tag + " is: " + std::to_string(val) << std::endl;
    if (printToFile) {
        write_to_json(outfile, tag, std::to_string(val) + ",", level);
    }

    return val;
}

int parseString(std::ifstream &infile, std::ofstream &outfile, std::string tag, int level, bool printToFile) {
    std::cout << "-------------------------" << std::endl;
    std::cout << "START parsing string..." << std::endl;

    bool charsTerm = true;
    bool going = true;
    std::string str = "";

    while (going) {
        unsigned char reader = 'a';
        infile.read((char *)&reader, sizeof(unsigned char));

        /**
         * Since the first representation contains a space after each character,
         *  I do `zigzagging` for identifying valid chars.
         */
        if (charsTerm) {
            // `reader == 32` includes the case of the font name with a space (unicode: 32).
            if (('a' <= reader && reader <= 'z') || ('A' <= reader && reader <= 'Z') || reader == 32) {
                str += reader;
            } else {
                going = false;
            }
        }
        charsTerm = !charsTerm;
    }

    std::cout << "The string gotten is: " << str << std::endl;

    int id = getChar(infile);
    if (id == 0) {
        moveBytes(infile, 55);
    } else if (id == 3) {
        moveBytes(infile, 25);
    } else {
        std::cout << "ERROR: Failed to validate string format..." << std::endl;
    }

    going = true;
    std::string str2 = "";

    while (going) {
        unsigned char reader = 'a';
        infile.read((char *)&reader, sizeof(unsigned char));

        // `reader == 32` includes the case of the font name with a space (unicode: 32).
        if (('a' <= reader && reader <= 'z') || ('A' <= reader && reader <= 'Z') || reader == 32) {
            str2 += reader;
        } else {
            going = false;
        }
    }

    std::cout << "The 2nd string gotten is: " << str2 << std::endl;

    if (str == str2) {
        if (printToFile) {
            write_to_json(outfile, "fontName", "\"" + str + "\",", level);
        }
    } else {
        std::cout << "ERROR: Failed to validate string format..." << std::endl;
    }

    moveBytes(infile, 29);

    return 0;
}

int parseTemplate(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile) {
    std::cout << "-------------------------" << std::endl;
    std::cout << "START parsing template..." << std::endl;

    // Validate if the header is there.
    if (0 != hexValidation(infile, "713A0941E1CCD011BFAA0080C7E24280", !DO_REWIND)) {
        std::cout << "ERROR: Fail to validate Line Fill pattern header..." << std::endl;
        return -1;
    }
    moveBytes(infile, 2);

    if (printToFile) {
        write_to_json(outfile, "template", "{", level);
    }
    parseDouble(infile, outfile, "interval", level + 1, printToFile);
    int num_of_patterns = getChar(infile);
    std::cout << "There is(are) " + std::to_string(num_of_patterns) + " patterns." << std::endl;
    moveBytes(infile, 3);

    for (int i = 0; i < num_of_patterns; i++) {
        if (printToFile) {
            write_to_json(outfile, "linePatternFeature", "{", level + 1);
        }
        parseDouble(infile, outfile, "patternLength", level + 2, printToFile);
        parseDouble(infile, outfile, "gapLength", level + 2, printToFile);
        if (printToFile) {
            write_to_json(outfile, "", "},", level + 1);
        }
    }

    if (printToFile) {
        write_to_json(outfile, "", "}", level);
    }

    moveBytes(infile, 33);

    return 0;
}
