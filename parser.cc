//
//  parser.cc
//
//  Created by Erick Li on 04/11/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#include "parser.hh"

std::string grandParser(char **input) {
    // Initialize a jstring (for JSON string)
    std::string jstring = "";
    bool skipHead = false;
    int num_of_layers = 0;
    write_to_json(jstring, "", "{", 0);

    try {
        if (getChar(input) != 4) {
            skipHead = true;
            num_of_layers = 1;
            bytesRewinder(input, 1);
        } else {
            // validate the header of the file.
            int s = hexValidation(input, "E6147992C8D011", DO_REWIND);
            if (s == 1) {
                LOG("ERROR: Header cannot be parsed.");
                throw std::string("Header.\n");
            } else {
                LOG("File is validated.");
            }
            // Move infile pointer 26 bytes to skip the header metadata.
            bytesHopper(input, 25);
            // Parse the first color pattern. Usage so far is unknown...
            parseColorPattern(input, jstring, "Unknown", 1);
            // Parse out the number of layers...
            num_of_layers = parseLayerNumber(input, jstring, 1);
            // Test if the number of layers behaves weird...
            if (-1 == num_of_layers) {
                LOG("ERROR: Number of Layers is abnormal.");
                throw std::string("Number of layers.\n");
            }
        }

        write_to_json(jstring, "layer", "[", 1);

        // Start parsing each layer.
        for (int i = 0; i < num_of_layers; i++) {
            LOG("++++ START parsing layer NO. " + std::to_string(i + 1));
            write_to_json(jstring, "", "{", 2);
            write_to_json(jstring, "number", std::to_string(i + 1) + ",", 3);
            parseLayer(input, jstring, 0, 3);

            // Inter-layer pattern...
            if (i < num_of_layers - 1) {
                bytesRewinder(input, 1);
                int b = 0;
                do {
                    b = getChar(input);
                } while (b != 3 && b != 6 && b != 8 && b != 9);
                bytesRewinder(input, 1);
            }
            write_to_json(jstring, "", "},", 2);
        }

        write_to_json(jstring, "", "],", 1);
    } catch (std::string err) {
        LOG("ERROR occurred. Stopped...");
        return std::string("\"error\": \"" + err + "\"\n");
    }

    int stnl = get64Bit(input);
    if (0x0D != stnl) {
        LOG("ERROR: sentinel");
        throw std::string("0x0D sentinel");
    }

    write_to_json(jstring, "fillLayerActiveness", "{", 1);
    LOG("Checking fill layer activeness...");

    for (int i = 0; i < num_of_layers; i++) {
        int activeness = get32Bit(input);
        LOG("Fill layer " + std::to_string(i + 1) + ": " + std::to_string(activeness));
        write_to_json(jstring, "layer" + std::to_string(i + 1), std::to_string(activeness) + ",", 2);
    }

    write_to_json(jstring, "", "},", 1);
    write_to_json(jstring, "", "}", 0);
    LOG("DONE :-)");

    return json_comma_remover(jstring);
}

int parseLayer(char **cursor, std::string &jstring, int type, int level) {
    LOG("-------------------------------");

    // Type 0: A normal layer
    if (type == 0) {
        LOG("START parsing a layer...");
    // A Symbol
    } else {
        LOG("START parsing a symbol...");
    }

    // Get the filling type (3 for simple fill; 6 for line fill; 8 for marker fill)
    int filling_type = get16Bit(cursor);
    // name of the corresponding filling type.
    std::string filling_type_name = "";

    // For each filling type, enter into the corresponding field.
    if (0xE603 == filling_type) {
        parseSimpleFill(cursor, jstring, type, level);
    } else if (0xE606 == filling_type) {
        parseLineFill(cursor, jstring, level);
    } else if (0xE608 == filling_type) {
        parseMarkerFill(cursor, jstring, level);
    } else if (0xE609 == filling_type) {
        LOG("ERROR: Gradient Fill is currently not supported");
        throw std::string("Currently unsupported filling type.");
    } else {
        LOG("ERROR: Filling type " + std::to_string(filling_type) + " not supported");
        throw std::string("Filling type.");
    }

    return 0;
}

int parseSimpleFill(char **cursor, std::string &jstring, int type, int level) {
    LOG("---------------------------");
    LOG("Filling type: Simple Fill");

    write_to_json(jstring, "fillingType", "\"Simple Fill\",", level);

    try {
        // Validate if the header is there.
        if (0 != hexValidation(cursor, "147992C8D0118BB6080009EE4E41", !DO_REWIND)) {
            LOG("ERROR: Fail to validate Simple Fill pattern header...");
            throw std::string("Validation.");
        }
        bytesHopper(cursor, 2);

        // Type of the process (0 for normal process; 1 for the symbol process)
        if (type == 0) {
            parseLinePattern(cursor, jstring, 0, "Outline", level);
        } else {
            parseLinePattern(cursor, jstring, 1, "Filling Line", level);
        }

        parseColorPattern(cursor, jstring, "Filling Color", level);
    } catch (std::string err) {
        throw err;
    }

    return 0;
}


int parseLineFill(char **cursor, std::string &jstring, int level) {
    LOG("---------------------------");
    LOG("Filling type: Line Fill");

    write_to_json(jstring, "fillingType", "\"Line Fill\",", level);

    try {
        // Validate if the header is there.
        if (0 != hexValidation(cursor, "147992C8D0118BB6080009EE4E41", !DO_REWIND)) {
            LOG("ERROR: Fail to validate Line Fill pattern header...");
            throw std::string("Validation.");
        }
        bytesHopper(cursor, 18);

        // Parse the line pattern for the filling lines.
        parseLinePattern(cursor, jstring, 0, "Filling Line", level);
        // parse the line pattern for the outline.
        parseLinePattern(cursor, jstring, 0, "Outline", level);
    } catch (std::string err) {
        throw err;
    }

    // Parse the line fill angle. (the angle of the line inclination)
    parseDouble(cursor, jstring, "angle", level);
    // Parse the line fill offset. (the horizontal displacement of the lines)
    parseDouble(cursor, jstring, "offset", level);
    // Parse the line fill separation (the distance between each line)
    parseDouble(cursor, jstring, "separation", level);



    return 0;
}

int parseMarkerFill(char **cursor, std::string &jstring, int level) {
    LOG("---------------------------");
    LOG("Filling type: Marker Fill");

    write_to_json(jstring, "fillingType", "\"Marker Fill\",", level);

    try {
        // Validate if the header is there.
        if (0 != hexValidation(cursor, "147992C8D0118BB6080009EE4E41", !DO_REWIND)) {
            LOG("ERROR: Fail to validate Marker Fill pattern header...");
            throw std::string("validation");
        }
        bytesHopper(cursor, 2);

        // Parse the marker types (Grid(uniform) or random distribution)
        parseMarkerTypes(cursor, jstring, level);
        // Parse the X and Y-axial offset (horizontal displacement of the markers)
        parseDouble(cursor, jstring, "fillPropertiesOffsetX", level);
        parseDouble(cursor, jstring, "fillPropertiesOffsetY", level);
        // Parse the X and Y-axial separation of the markers
        parseDouble(cursor, jstring, "fillPropertiesSeparationX", level);
        parseDouble(cursor, jstring, "fillPropertiesSeparationY", level);
        bytesHopper(cursor, 16);

        // Parse the Marker in detail
        parseMarkerPattern(cursor, jstring, level);

        // Parse the outline.
        parseLinePattern(cursor, jstring, 0, "Outline", level);
    } catch (std::string err) {
        throw err;
    }

    return 0;
}

int parseColorPattern(char **cursor, std::string &jstring, std::string color_type, int level) {
    LOG("----------------------");
    LOG("START parsing color...");

    // Get the color space (0x92 for HSV; 0x96 for RGB; 0x97 for CMYK)
    int color_space = get16Bit(cursor);

    write_to_json(jstring, "color_" + toSimpleCamelCase(color_type), "{", level);
    write_to_json(jstring, "name", "\"" + color_type + "\",", level + 1);

    // CMYK color space...
    if (0xC497 == color_space) {
        LOG("Color Space: CMYK.");

        // Skip the metadata of the color space.
        bytesHopper(cursor, 18);
        int c = getChar(cursor);
        int m = getChar(cursor);
        int y = getChar(cursor);
        int k = getChar(cursor);

        LOG("C: " + std::to_string(c));
        LOG("M: " + std::to_string(m));
        LOG("Y: " + std::to_string(y));
        LOG("K: " + std::to_string(k));

        write_to_json(jstring, "space", "\"CMYK\",", level + 1);
        write_to_json(jstring, "C", std::to_string(c) + ",", level + 1);
        write_to_json(jstring, "M", std::to_string(m) + ",", level + 1);
        write_to_json(jstring, "Y", std::to_string(y) + ",", level + 1);
        write_to_json(jstring, "K", std::to_string(k), level + 1);
    } else {
        // HSV color space...
        if (0xC492 == color_space) {
            LOG("Color Space: HSV.");
        // RGB color space...
    } else if (0xC496 == color_space) {
            LOG("Color Space: RGB.");
        } else {
            // If the color space code is not 92, 96, or 97, then an error mesg will be printed out.
            LOG("ERROR: Color Space " + std::to_string(color_space) + " not found.");
            throw std::string("Color Space.");
        }
        bytesHopper(cursor, 19);

        // There are three fields for both HSV and RGB.
        // Type and the definition are unknown so far.
        // HSV and RGB share the same coding philsophy.
        double first = getDouble(cursor);
        double second = getDouble(cursor);
        double third = getDouble(cursor);

        LOG("1st field: " + std::to_string(first));
        LOG("2nd field: " + std::to_string(second));
        LOG("3rd field: " + std::to_string(third));

        if (0xC492 == color_space) {
            write_to_json(jstring, "space", "\"HSV\",", level + 1);
        } else if (0xC496 == color_space) {
            write_to_json(jstring, "space", "\"RGB\",", level + 1);
        }
        write_to_json(jstring, "firstField", std::to_string(first) + ",", level + 1);
        write_to_json(jstring, "secondField", std::to_string(second) + ",", level + 1);
        write_to_json(jstring, "thirdField", std::to_string(third), level + 1);
    }

    write_to_json(jstring, "", "},", level);

    bytesHopper(cursor, 2);

    return 0;
}

int parseLayerNumber(char **cursor, std::string &jstring, int level) {
    LOG("-----------------------------");
    LOG("START parsing layer number");

    // Number of layers.
    int num_of_layers = getChar(cursor);
    LOG("The number of layer is: " + std::to_string(num_of_layers));

    // Check whether the layer number is normal...
    if (0 > num_of_layers || 5 < num_of_layers) {
        LOG("WARNING: Layer number is abnormal.");
    }

    write_to_json(jstring, "numberOfLayers", std::to_string(num_of_layers) + ",", level);

    bytesHopper(cursor, 3);

    return num_of_layers;
}




double parseDouble(char **cursor, std::string &jstring, std::string tag, int level) {
    double val = getDouble(cursor);

    // Put the name and the value of the double value into the JSON string.
    LOG(tag + " is: " + std::to_string(val));
    write_to_json(jstring, tag, std::to_string(val) + ",", level);

    return val;
}

int parseInt(char **cursor, std::string &jstring, std::string tag, int level) {
    int val = get32Bit(cursor);

    // Put the name and value of the integer value into the JSON string.
    LOG(tag + " is: " + std::to_string(val));
    write_to_json(jstring, tag, std::to_string(val) + ",", level);

    return val;
}

int parseString(char **cursor, std::string &jstring, std::string tag, int level) {
    LOG("-------------------------");
    LOG("START parsing string...");

    bool going = true; // When "going" is true, the while loop will keep going.
    std::string str = "";

    while (going) {
        char reader = **cursor;
        bytesHopper(cursor, 2);
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

    LOG("The string gotten is: " + str);

    // The length of the null pattern can be varied.
    int id = getChar(cursor);
    if (id == 0) {
        bytesHopper(cursor, 54);
    } else if (id == 3) {
        bytesHopper(cursor, 24);
    } else {
        LOG("ERROR: Failed to validate string format...");
        throw std::string("validation.");
    }

    going = true;
    std::string str2 = "";

    // This round will have no space after each character.
    while (going) {
        char reader = **cursor;
        bytesHopper(cursor, 1);

        // `reader == 32` includes the case of the font name with a space (unicode: 32).
        if (('a' <= reader && reader <= 'z') || ('A' <= reader && reader <= 'Z') || reader == 32) {
            str2 += reader;
        } else {
            going = false;
        }
    }

    LOG("The 2nd string gotten is: " + str2);

    // Validate if the two string is matching.
    if (str == str2) {
        write_to_json(jstring, "fontName", "\"" + str + "\",", level);
    } else {
        LOG("ERROR: Failed to validate string format...");
        throw std::string("validation.");
    }

    bytesRewinder(cursor, 1);

    return 0;
}


int parseTemplate(char **cursor, std::string &jstring, int type, int level) {
    LOG("---------------------------------------");
    LOG("START parsing template...");

    // It is possible that the template is not defined.
    double preq = getDouble(cursor);// WARNING should be 0 in the case i am working on
    bytesRewinder(cursor, 8);

    // If the template is defined, we can go through it.
    if (0.0 != preq) {
        // Validate if the header is there.
        if (0 != hexValidation(cursor, "713A0941E1CCD011BFAA0080C7E24280", !DO_REWIND)) {
            LOG("ERROR: Fail to validate template pattern header...");
            throw std::string("Template validation.");
        }
        bytesHopper(cursor, 2);

        write_to_json(jstring, "template", "{", level);

        // Distance between two patterns.
        parseDouble(cursor, jstring, "interval", level + 1);
        int num_of_patterns = getChar(cursor);
        LOG("There is(are) " + std::to_string(num_of_patterns) + " patterns.");
        bytesHopper(cursor, 3);

        for (int i = 0; i < num_of_patterns; i++) {
            write_to_json(jstring, "linePatternFeature", "{", level + 1);
            parseDouble(cursor, jstring, "patternLength", level + 2);
            parseDouble(cursor, jstring, "gapLength", level + 2);
            write_to_json(jstring, "", "},", level + 1);
        }

        write_to_json(jstring, "", "},", level);
        // bytesHopper(cursor, 16);
    } else {
        // Null bytes for null template.
        bytesHopper(cursor, 16);
    }

    if (type == 1) {
        while (0x0D != getChar(cursor)) {}
        bytesHopper(cursor, 16);
    } else {
        while (0x24 != getChar(cursor)) {}
        if (0x40 != getChar(cursor)) {
            LOG("ERROR: Fail to validate template tail...");
            throw std::string("Template tail validation.");
        }
    }


    return 0;
}
