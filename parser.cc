#include "parser.hh"

std::string grandParser(char **input) {
    std::string jstring = "";

    int s = hexValidation(input, "04E6147992C8D011", true);
    if (s == 1) {
        std::cout << "ERROR: Check style file version." << std::endl;
        return "";
    } else {
        std::cout << "File version is validated." << std::endl;
    }

    write_to_json(jstring, "", "{", 0);
    // Move infile pointer 26 bytes to skip the header metadata.
    moveBytes(input, 26);
    // Parse the first color pattern. Usage so far is unknown...
    parseColorPattern(input, jstring, "Unknown", 1, PRINT_TO_FILE);
    // Parse out the number of layers...
    int num_of_layers = parseLayerNumber(input, jstring, 1, PRINT_TO_FILE);
    // Test if the number of layers behaves weird...
    if (-1 == num_of_layers)
        exit(1);

    // Start parsing each layer.
    for (int i = 0; i < num_of_layers; i++) {
        parseLayer(input, jstring, 0, 1, i + 1, PRINT_TO_FILE);
        // Inter-layer pattern...
        if (i < num_of_layers - 1) {
            int b = 0;
            do {
                b = getChar(input);
            } while (b != 3 && b != 6 && b != 8 && b != 9);
            goRewind(input, 1);
        }
    }

    write_to_json(jstring, "", "}", 0);
    std::cout << "DONE :-)" << std::endl;

    return jstring;
}

int parseLayer(char **cursor, std::string &jstring, int type, int level, int layer_no, bool printToFile) {
    std::cout << "-------------------------------" << std::endl;
    std::cout << "START parsing a symbol/layer..." << std::endl;

    if (type == 0) {
        write_to_json(jstring, "layer", "{", level);
        write_to_json(jstring, "number", std::to_string(layer_no) + ",", level + 1);
    } else {
        write_to_json(jstring, "fillSymbol", "{", level);
    }

    int filling_type = getChar(cursor);
    std::string filling_type_name = "";

    if (3 == filling_type) {
        parseSimpleFill(cursor, jstring, type, level + 1, PRINT_TO_FILE);
    } else if (6 == filling_type) {
        parseLineFill(cursor, jstring, level + 1, PRINT_TO_FILE);
    } else if (8 == filling_type) {
        parseMarkerFill(cursor, jstring, level + 1, PRINT_TO_FILE);
    } else {
        std::cout << "ERROR: Filling type " << filling_type << " not support" << std::endl;
        return -1;
    }

    // Inter-layer pattern is defined in main()
    // This part is for Symbol...
    if (type == 1) {
        int b = 0;
        do {
            b = getChar(cursor);
        } while (b != 20);

        goRewind(cursor, 3);
    }

    write_to_json(jstring, "", "},", level);

    return 0;
}

int parseColorPattern(char **cursor, std::string &jstring, std::string color_type, int level, bool printToFile) {
    std::cout << "----------------------" << std::endl;
    std::cout << "START parsing color..." << std::endl;
    int color_space = getChar(cursor);

    if (printToFile) {
        write_to_json(jstring, "color", "{", level);
        write_to_json(jstring, "name", "\"" + color_type + "\",", level + 1);
    }

    // CMYK color space...
    if (151 == color_space) {
        std::cout << "Color Space: CMYK." << std::endl;

        moveBytes(cursor, 19);
        int c = getChar(cursor);
        int m = getChar(cursor);
        int y = getChar(cursor);
        int k = getChar(cursor);

        std::cout << "C: " << c << std::endl;
        std::cout << "M: " << m << std::endl;
        std::cout << "Y: " << y << std::endl;
        std::cout << "K: " << k << std::endl;

        if (printToFile) {
            write_to_json(jstring, "space", "\"CMYK\",", level + 1);
            write_to_json(jstring, "C", std::to_string(c) + ",", level + 1);
            write_to_json(jstring, "M", std::to_string(m) + ",", level + 1);
            write_to_json(jstring, "Y", std::to_string(y) + ",", level + 1);
            write_to_json(jstring, "K", std::to_string(k), level + 1);
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
        moveBytes(cursor, 20);
        double first = getDouble(cursor);
        double second = getDouble(cursor);
        double third = getDouble(cursor);

        std::cout << "1st field: " << first << std::endl;
        std::cout << "2nd field: " << second << std::endl;
        std::cout << "3rd field: " << third << std::endl;

        if (printToFile) {
            if (146 == color_space) {
                write_to_json(jstring, "space", "\"HSV\",", level + 1);
            } else if (150 == color_space) {
                write_to_json(jstring, "space", "\"RGB\",", level + 1);
            }
            write_to_json(jstring, "firstField", std::to_string(first) + ",", level + 1);
            write_to_json(jstring, "secondField", std::to_string(second) + ",", level + 1);
            write_to_json(jstring, "thirdField", std::to_string(third), level + 1);
        }
    }

    if (printToFile) {
        write_to_json(jstring, "", "},", level);
    }

    moveBytes(cursor, 2);

    return 0;
}

int parseLayerNumber(char **cursor, std::string &jstring, int level, bool printToFile) {
    std::cout << "-----------------------------" << std::endl;
    std::cout << "START parsing layer number" << std::endl;
    int num_of_layers = getChar(cursor);
    std::cout << "The number of layer is: " << num_of_layers << std::endl;

    // Check whether the layer number is normal...
    if (0 > num_of_layers || 5 < num_of_layers) {
        std::cout << "WARNING: Layer number is abnormal." << std::endl;
        return -1;
    }

    if (printToFile) {
        write_to_json(jstring, "numberOfLayers", std::to_string(num_of_layers) + ",", level);
    }

    moveBytes(cursor, 3);

    return num_of_layers;
}


int parseSimpleFill(char **cursor, std::string &jstring, int type, int level, bool printToFile) {
    std::cout << "---------------------------" << std::endl;
    std::cout << "Filling type: Simple Fill" << std::endl;
    if (printToFile) {
        write_to_json(jstring, "fillingType", "\"Simple Fill\",", level);
    }

    // Validate if the header is there.
    if (0 != hexValidation(cursor, "E6147992C8D0118BB6080009EE4E41", !DO_REWIND)) {
        std::cout << "ERROR: Fail to validate Simple Fill pattern header..." << std::endl;
        return -1;
    }
    moveBytes(cursor, 2);

    if (type == 0) {
        parseLinePattern(cursor, jstring, 0, "Outline", level, PRINT_TO_FILE);
    } else {
        parseLinePattern(cursor, jstring, 1, "Filling Line", level, PRINT_TO_FILE);
    }

    parseColorPattern(cursor, jstring, "Filling Color", level, PRINT_TO_FILE);

    return 0;
}


int parseLineFill(char **cursor, std::string &jstring, int level, bool printToFile) {
    std::cout << "---------------------------" << std::endl;
    std::cout << "Filling type: Line Fill" << std::endl;
    if (printToFile) {
        write_to_json(jstring, "fillingType", "\"Line Fill\",", level);
    }

    // Validate if the header is there.
    if (0 != hexValidation(cursor, "E6147992C8D0118BB6080009EE4E41", !DO_REWIND)) {
        std::cout << "ERROR: Fail to validate Line Fill pattern header..." << std::endl;
        return -1;
    }
    moveBytes(cursor, 18);

    parseLinePattern(cursor, jstring, 0, "Filling Line", level, printToFile);
    parseLinePattern(cursor, jstring, 0, "Outline", level, printToFile);
    parseDouble(cursor, jstring, "angle", level, printToFile);
    parseDouble(cursor, jstring, "offset", level, printToFile);
    parseDouble(cursor, jstring, "separation", level, printToFile);

    return 0;
}

int parseMarkerFill(char **cursor, std::string &jstring, int level, bool printToFile) {
    std::cout << "---------------------------" << std::endl;
    std::cout << "Filling type: Marker Fill" << std::endl;
    if (printToFile) {
        write_to_json(jstring, "fillingType", "\"Marker Fill\",", level);
    }

    // Validate if the header is there.
    if (0 != hexValidation(cursor, "E6147992C8D0118BB6080009EE4E41", !DO_REWIND)) {
        std::cout << "ERROR: Fail to validate Marker Fill pattern header..." << std::endl;
        return -1;
    }
    moveBytes(cursor, 2);

    parseMarkerTypes(cursor, jstring, level, printToFile);
    parseDouble(cursor, jstring, "fillPropertiesOffsetX", level, printToFile);
    parseDouble(cursor, jstring, "fillPropertiesOffsetY", level, printToFile);
    parseDouble(cursor, jstring, "fillPropertiesSeparationX", level, printToFile);
    parseDouble(cursor, jstring, "fillPropertiesSeparationY", level, printToFile);
    moveBytes(cursor, 16);

    parseMarkerPattern(cursor, jstring, level, printToFile);

    parseLinePattern(cursor, jstring, 0, "Outline", level, printToFile);

    return 0;
}

double parseDouble(char **cursor, std::string &jstring, std::string tag, int level, bool printToFile) {
    double val = getDouble(cursor);
    std::cout << tag + " is: " + std::to_string(val) << std::endl;
    if (printToFile) {
        write_to_json(jstring, tag, std::to_string(val) + ",", level);
    }

    return val;
}

int parseInt(char **cursor, std::string &jstring, std::string tag, int level, bool printToFile) {
    int val = getInt(cursor);
    std::cout << tag + " is: " + std::to_string(val) << std::endl;
    if (printToFile) {
        write_to_json(jstring, tag, std::to_string(val) + ",", level);
    }

    return val;
}

int parseString(char **cursor, std::string &jstring, std::string tag, int level, bool printToFile) {
    std::cout << "-------------------------" << std::endl;
    std::cout << "START parsing string..." << std::endl;

    bool going = true;
    std::string str = "";

    while (going) {
        char reader = **cursor;
        moveBytes(cursor, 2);
        /**
         * Since the first representation contains a space after each character,
         *  I do `zigzagging` for identifying valid chars.
         */
            // `reader == 32` includes the case of the font name with a space (unicode: 32).
        if (('a' <= reader && reader <= 'z') || ('A' <= reader && reader <= 'Z') || reader == 32) {
            str += reader;
        } else {
            going = false;
        }
    }

    std::cout << "The string gotten is: " << str << std::endl;

    int id = getChar(cursor);
    if (id == 0) {
        moveBytes(cursor, 54);
    } else if (id == 3) {
        moveBytes(cursor, 24);
    } else {
        std::cout << "ERROR: Failed to validate string format..." << std::endl;
    }

    going = true;
    std::string str2 = "";

    while (going) {
        char reader = **cursor;
        moveBytes(cursor, 1);

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
            write_to_json(jstring, "fontName", "\"" + str + "\",", level);
        }
    } else {
        std::cout << "ERROR: Failed to validate string format..." << std::endl;
    }

    moveBytes(cursor, 29);

    return 0;
}

// 0: others, 1: char marker.
int parseTemplate(char **cursor, std::string &jstring, int type, int level, bool printToFile) {
    std::cout << "-------------------------" << std::endl;
    std::cout << "START parsing template..." << std::endl;

    // Validate if the header is there.
    if (0 != hexValidation(cursor, "713A0941E1CCD011BFAA0080C7E24280", !DO_REWIND)) {
        std::cout << "ERROR: Fail to validate Line Fill pattern header..." << std::endl;
        return -1;
    }
    moveBytes(cursor, 2);

    if (printToFile) {
        write_to_json(jstring, "template", "{", level);
    }
    parseDouble(cursor, jstring, "interval", level + 1, printToFile);
    int num_of_patterns = getChar(cursor);
    std::cout << "There is(are) " + std::to_string(num_of_patterns) + " patterns." << std::endl;
    moveBytes(cursor, 3);

    for (int i = 0; i < num_of_patterns; i++) {
        if (printToFile) {
            write_to_json(jstring, "linePatternFeature", "{", level + 1);
        }
        parseDouble(cursor, jstring, "patternLength", level + 2, printToFile);
        parseDouble(cursor, jstring, "gapLength", level + 2, printToFile);
        if (printToFile) {
            write_to_json(jstring, "", "},", level + 1);
        }
    }

    if (printToFile) {
        write_to_json(jstring, "", "}", level);
    }

    if (type == 0) {
        moveBytes(cursor, 41);
    } else {
        moveBytes(cursor, 33);
    }

    return 0;
}
