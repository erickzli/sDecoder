//
//  parser.cc
//
//  Created by Erick Li on 04/11/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#include "parser.hh"

std::string grandParser(char **cursor) {
    // Initialize a jstring (for JSON string)
    std::string jstring = "";
    write_to_json(jstring, "", "{", 0);

    try {
        int grand_stnl = get16Bit(cursor);
        if (0xE603 == grand_stnl || (0xE606 <= grand_stnl && grand_stnl <= 0xE609)) {
            grand_stnl = 0x0000; // this is a special code for fill symbol without header...
        }
        bytesRewinder(cursor, 2);

        switch (grand_stnl) {
            case 0xE604:
                // Fill Symbol
                write_to_json(jstring, "symbolType", "fill,", 1);
                parseFillPattern(cursor, jstring, 1);
                break;
            case 0x0000:
                // Fill Symbol without header
                write_to_json(jstring, "symbolType", "fill,", 1);
                parseFillPattern(cursor, jstring, 1);
                break;
            case 0xE5FA:
                // line symbol
                write_to_json(jstring, "symbolType", "line,", 1);
                parseLinePattern(cursor, jstring, 1, "", 1);
                break;
            case 0xE5FF:
                // marker symbol
                write_to_json(jstring, "symbolType", "marker,", 1);
                parseMarkerPattern(cursor, jstring, 1);
                break;
            default:
                LOG("ERROR: Cannot recognize header...");
                throw std::string("Symbol header.");
        }
    } catch (std::string err) {
        LOG("ERROR occurred. Stopped...");
        return std::string("\"error\": \"" + err + "\"\n");
    }

    write_to_json(jstring, "", "}", 0);
    LOG("DONE :-)");

    return json_comma_remover(jstring);
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
