#include "parser.hh"
#include "json_writer.hh"

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
        std::cout << "ERROR: Filling type not support or the file has problems." << std::endl;
        return -1;
    }

    if (type == 0) {
        // Parse the tail pattern of the layer...
        parseTailPattern(infile, 3);
    } else {
        moveBytes(infile, 17);
    }

    write_to_json(outfile, "", "},", level);

    return 0;
}

int parseColorPattern(std::ifstream &infile, std::ofstream &outfile, std::string color_type, int level, bool printToFile) {
    std::cout << "------------------------" << std::endl;
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
        std::cout << "ERROR: Fail to validate Simple Fill pattern header..." << std::endl;
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

int parseLinePattern(std::ifstream &infile, std::ofstream &outfile, int type, std::string property, int level, bool printToFile) {
    std::cout << "-----------------------------" << std::endl;
    if (type == 0) {
        std::cout << "START parsing line" << std::endl;
        if (printToFile) {
            write_to_json(outfile, "lineProperties", "{", level);
            write_to_json(outfile, "name", "\"" + property + "\",", level + 1);
        }
    } else {
        std::cout << "START parsing line symbol" << std::endl;
        if (printToFile) {
            write_to_json(outfile, "lineSymbol", "{", level);
        }
    }

    int line_type = getChar(infile);

    if (250 == line_type) {
        moveBytes(infile, 29);
        line_type = getChar(infile);
    }

    moveBytes(infile, 17);
    switch(line_type) {
        case 249:
            parseSimpleLine(infile, outfile, level + 1, printToFile);
            break;
        case 251:
            parseCartoLine(infile, outfile, level + 1, printToFile);
            break;
        case 252:
            parseHashLine(infile, outfile, level + 1, printToFile);
            break;
        case 253:
            parseMarkerLine(infile, outfile, level + 1, printToFile);
            break;
        default:
            std::cout << "ERROR: Line type " << std::to_string(line_type) << " not found." << std::endl;
            return -1;
    }

    if (printToFile) {
        write_to_json(outfile, "", "},", level);
    }

    // Ignore the tail pattern.
    if (property == "Outline") {
        parseTailPattern(infile, 1);
    } else if (property == "Filling Line") {
        parseTailPattern(infile, 2);
    } else {
        std::cout << "ERROR: Invalid line property..." << std::endl;
        return -1;
    }

    return 0;
}

int parseSimpleLine(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile) {
    std::cout << "Type: Simple Line..." << std::endl;
    if (printToFile) {
        write_to_json(outfile, "type", "\"Simple Line\",", level);
    }
    parseColorPattern(infile, outfile, "Simple Line Color", level, printToFile);
    parseDouble(infile, outfile, "width", level, printToFile);
    parseLineStyle(infile, outfile, level, printToFile);

    return 0;
}

int parseCartoLine(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile) {
    std::cout << "Type: Cartographic Line..." << std::endl;
    if (printToFile) {
        write_to_json(outfile, "type", "\"Cartographic Line\",", level);
    }

    parseLineCaps(infile, outfile, level, printToFile);
    parseLineJoins(infile, outfile, level, printToFile);
    parseDouble(infile, outfile, "cartographicLineWidth", level, printToFile);
    moveBytes(infile, 1);
    parseDouble(infile, outfile, "propertiesOffset", level, printToFile);
    parseColorPattern(infile, outfile, "Cartographic Line Color", level, printToFile);
    parseTemplate(infile, outfile, level, printToFile);

    return 0;
}

int parseHashLine(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile) {
    std::cout << "Type: Hash Line..." << std::endl;
    if (printToFile) {
        write_to_json(outfile, "type", "\"Hash Line\",", level);
    }

    parseDouble(infile, outfile, "hashLineAngle", level, printToFile);
    parseLineCaps(infile, outfile, level, printToFile);
    parseLineJoins(infile, outfile, level, printToFile);
    parseDouble(infile, outfile, "cartographicLineWidth", level, printToFile);
    moveBytes(infile, 1);
    parseDouble(infile, outfile, "propertiesOffset", level, printToFile);
    parseLinePattern(infile, outfile, 1, "Outline", level, printToFile);
    parseColorPattern(infile, outfile, "Cartographic Line Color", level, printToFile);
    parseTemplate(infile, outfile, level, printToFile);

    return 0;
}

int parseMarkerLine(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile) {
    std::cout << "Type: Hash Line..." << std::endl;
    if (printToFile) {
        write_to_json(outfile, "type", "\"Marker Line\",", level);
    }

    moveBytes(infile, 1);
    parseDouble(infile, outfile, "propertiesOffset", level, printToFile);
    parseCharacterMarker(infile, outfile, level, printToFile); // TODO...
    parseTemplate(infile, outfile, level, printToFile);
    parseLineCaps(infile, outfile, level, printToFile);
    parseLineJoins(infile, outfile, level, printToFile);
    parseDouble(infile, outfile, "cartographicLineWidth", level, printToFile);

    return 0;
}

int parseCharacterMarker(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile) {
    std::cout << "Type: Character Marker..." << std::endl;
    if (printToFile) {
        write_to_json(outfile, "type", "\"Character Marker\",", level);
    }

    moveBytes(infile, 18);
    moveBytes(infile, 8);
    parseDouble(infile, outfile, "markerSize", level, printToFile);
    moveBytes(infile, 24);
    parseColorPattern(infile, outfile, "Marker Color", level, printToFile);
    parseMaskTypes(infile, outfile, level, printToFile);
    parseDouble(infile, outfile, "Mask Size", level, printToFile);
    parseLayer(infile, outfile, 1, level, 0, printToFile); // TODO

    // Validate if the header is there.
    if (0 != hexValidation(infile, "E6147992C8D0118BB6080009EE4E41", !DO_REWIND)) {
        std::cout << "ERROR: Fail to validate Simple Fill pattern header..." << std::endl;
        return -1;
    }
    moveBytes(infile, 2);

    parseColorPattern(infile, outfile, "Marker Color", level, !PRINT_TO_FILE);
    parseInt(infile, outfile, "unicode", level, printToFile);
    parseDouble(infile, outfile, "markerAngle", level, printToFile);
    parseDouble(infile, outfile, "markerSize", level, printToFile);
    parseDouble(infile, outfile, "markerOffsetX", level, printToFile);
    parseDouble(infile, outfile, "markerOffsetY", level, printToFile);
    moveBytes(infile, 16);

    if (getChar(infile) == 13) {
        moveBytes(infile, 13);
    } else {
        goRewind(infile, 1);
    }

    parseString(infile, outfile, "font", level, printToFile);


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

int parseLineCaps(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile) {
    int line_caps_code = getChar(infile);
    std::string line_caps_name = "";

    switch(line_caps_code) {
        case 0:
            line_caps_name = "Butt";
            break;
        case 1:
            line_caps_name = "Round";
            break;
        case 2:
            line_caps_name = "Square";
            break;
        default:
            std::cout << "ERROR: Line caps code " << line_caps_code << " not found."  << std::endl;
            return -1;
    }

    std::cout << "The line cap is " << line_caps_name << std::endl;
    if (printToFile) {
        write_to_json(outfile, "lineCaps", "\"" + line_caps_name + "\",", level);
    }

    moveBytes(infile, 3);

    return line_caps_code;
}

int parseLineJoins(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile) {
    int line_joins_code = getChar(infile);
    std::string line_joins_name = "";

    switch(line_joins_code) {
        case 0:
            line_joins_name = "Miter";
            break;
        case 1:
            line_joins_name = "Round";
            break;
        case 2:
            line_joins_name = "Bevel";
            break;
        default:
            std::cout << "ERROR: Line joins code " << line_joins_code << " not found."  << std::endl;
            return -1;
    }

    std::cout << "The line join is " << line_joins_name << std::endl;
    if (printToFile) {
        write_to_json(outfile, "lineJoins", "\"" + line_joins_name + "\",", level);
    }

    moveBytes(infile, 3);

    return line_joins_code;
}

int parseMaskTypes(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile) {
    int mask_type_code = getChar(infile);
    std::string mask_type_name = "";

    if (mask_type_code == 0) {
        mask_type_name = "None";
    } else if (mask_type_code == 1) {
        mask_type_name = "Halo";
    } else {
        std::cout << "ERROR: Mask type code " << mask_type_code << " not found." << std::endl;
        return -1;
    }

    std::cout << "The mask type is " << mask_type_name << std::endl;
    if (printToFile) {
        write_to_json(outfile, "style", "\"" + mask_type_name + "\",", level);
    }

    moveBytes(infile, 3);

    return mask_type_code;
}

int parseMarkerTypes(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile) {
    int marker_type_code = getChar(infile);
    std::string marker_type_name = "";

    if (marker_type_code == 0) {
        marker_type_name = "Grid";
    } else if (marker_type_code == 1) {
        marker_type_name = "Random";
    } else {
        std::cout << "ERROR: Marker type code " << marker_type_code << " not found." << std::endl;
        return -1;
    }

    std::cout << "The marker type is " << marker_type_name << std::endl;
    if (printToFile) {
        write_to_json(outfile, "style", "\"" + marker_type_name + "\",", level);
    }

    moveBytes(infile, 3);

    return marker_type_code;
}

int parseLineStyle(std::ifstream &infile, std::ofstream &outfile, int level, bool printToFile) {
    int line_style_code = getChar(infile);
    std::string line_style_name = "";

    switch(line_style_code) {
        case 0:
            line_style_name = "Solid";
            break;
        case 1:
            line_style_name = "Dashed";
            break;
        case 2:
            line_style_name = "Dotted";
            break;
        case 3:
            line_style_name = "Dash-Dot";
            break;
        case 4:
            line_style_name = "Dash-Dot-Dot";
            break;
        case 5:
            line_style_name = "Null";
            break;
        default:
            std::cout << "ERROR: Line style code " << line_style_code << " not found."  << std::endl;
            return -1;
    }

    std::cout << "The line style is " << line_style_name << std::endl;
    if (printToFile) {
        write_to_json(outfile, "style", "\"" + line_style_name + "\",", level);
    }

    moveBytes(infile, 3);

    return line_style_code;
}


int parseTailPattern(std::ifstream &infile, int type) {
    // OUTLINE
    if (1 == type) {
        moveBytes(infile, 22);
    // LINE FILL
    } else if (2 == type) {
        moveBytes(infile, 8);
    // FILL TAIL
    } else if (3 == type) {
        moveBytes(infile, 26);
    } else {
        std::cout << "ERROR: Not support this tail code..." << std::endl;
    }

    return 0;
}
