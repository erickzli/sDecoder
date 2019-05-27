//
//  line_parser.cc
//
//  Created by Erick Li on 04/16/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#include "line_parser.hh"
#include "parser.hh"

int parseLinePattern(char **cursor, std::string &jstring, int type, std::string property, int level) {
    LOG("-----------------------------");
    if (type == 0) {
        LOG("START parsing line");
        write_to_json(jstring, "lineProperties_" + toSimpleCamelCase(property), "{", level);
        write_to_json(jstring, "name", "\"" + property + "\",", level + 1);
    } else {
        LOG("START parsing line symbol");
        write_to_json(jstring, "lineSymbol", "{", level);
    }

    // Get the line type
    int line_type = getChar(cursor);
    bytesRewinder(cursor, 1);
    // Set the default number of line layers to 1
    int num_of_line_layers = 1;

    // When the line type code is 0xFA, we need to find the info like layer number from it.
    if (0xFA == line_type) {
        bytesHopper(cursor, 26);
        num_of_line_layers = get32Bit(cursor);
    }

    write_to_json(jstring, "numberOfLineLayers", std::to_string(num_of_line_layers) + ",", level + 1);
    LOG("Number of line layers: " + std::to_string(num_of_line_layers));

    write_to_json(jstring, "lineLayer", "[", level + 1);

    // Go through each line layer.
    for (int i = 0; i < num_of_line_layers; i++) {
        LOG(" --- START parsing line layer NO. " + std::to_string(i + 1));
        line_type = getChar(cursor);
        bytesHopper(cursor, 17);

        write_to_json(jstring, "", "{", level + 2);
        write_to_json(jstring, "number", std::to_string(i + 1) + ",", level + 3);

        try {
            switch(line_type) {
                case 0xF9:
                    parseSimpleLine(cursor, jstring, level + 3);
                    break;
                case 0xFB:
                    parseCartoLine(cursor, jstring, level + 3);
                    break;
                case 0xFC:
                    parseHashLine(cursor, jstring, level + 3);
                    break;
                case 0xFD:
                    parseMarkerLine(cursor, jstring, level + 3);
                    break;
                default:
                    LOG("ERROR: Line type " + std::to_string(line_type) + " not found.");
                    throw std::string("Line type.");
            }
        } catch (std::string err) {
            throw err;
        }

        write_to_json(jstring, "", "},", level + 2);
    }

    write_to_json(jstring, "", "],", level + 1);

    // Dealing: only has activeness but without 02 00 00 00 00 00
    int preprecheck = get32Bit(cursor);
    bytesRewinder(cursor, 4);
    if (preprecheck != 1) {
        bytesHopper(cursor, num_of_line_layers * 8);
        int one = get32Bit(cursor);
        int two = get16Bit(cursor);
        bytesRewinder(cursor, num_of_line_layers * 8 + 6);
        if (one != 2 || two != 0) {
            write_to_json(jstring, "", "},", level);
            return 0;
        }
    }

    // Parse the activeness of layers.
    int check_active_int = get32Bit(cursor);
    bytesRewinder(cursor, 4);
    double check_active_double = getDouble(cursor);
    bytesRewinder(cursor, 8);
    if ((check_active_int == 0 || check_active_int == 1) && check_active_double < 0.5) {
        LOG("Checking line layer activeness...");
        write_to_json(jstring, "lineLayerActiveness", "{", level + 1);
        for (size_t i = 0; i < num_of_line_layers; i++) {
            int activeness = get32Bit(cursor); // 0: deactivated; 1: activated
            LOG("Line layer" + std::to_string(i + 1) + " Activeness: " + std::to_string(activeness));
            write_to_json(jstring, "layer" + std::to_string(i + 1), std::to_string(activeness) + ",", level + 2);
        }
        write_to_json(jstring, "", "}", level + 1);

        for (size_t i = 0; i < num_of_line_layers; i++) {
            bytesHopper(cursor, 4); // Hop for (01 00 00 00) or (00 00 00 00)
        }
        for (size_t i = 0; i < num_of_line_layers; i++) {
            if (0x02 == getChar(cursor)) {
                bytesHopper(cursor, 5); // Hop for (02 00 00 00 00 00)
            } else {
                bytesRewinder(cursor, 1);
            }
        }
    }

    write_to_json(jstring, "", "},", level);

    return 0;
}

int parseSimpleLine(char **cursor, std::string &jstring, int level) {
    LOG("Type: Simple Line...");
    write_to_json(jstring, "type", "\"Simple Line\",", level);

    try {
        // Parse the color for the line.
        parseColorPattern(cursor, jstring, "Simple Line Color", level);
        // Parse the width of the line.
        parseDouble(cursor, jstring, "width", level);
        // Parse the line style of the line. (Solid, dashed, dotted, dash-dot, dash-dot-dot, null)
        parseLineStyle(cursor, jstring, level);
    } catch (std::string err) {
        throw err;
    }

    // Parse the TAIL pattern of simple line.
    //while (0x0D != getChar(cursor)) {}
    bytesHopper(cursor, 8);

    return 0;
}

int parseCartoLine(char **cursor, std::string &jstring, int level) {
    LOG("Type: Cartographic Line...");
    write_to_json(jstring, "type", "\"Cartographic Line\",", level);

    try {
        // Parse line caps (butt, round, square)
        parseLineCaps(cursor, jstring, level);
        // Parse line joins (miter, round, bevel)
        parseLineJoins(cursor, jstring, level);
        // Parse the cartographic line width
        parseDouble(cursor, jstring, "cartographicLineWidth", level);
        bytesHopper(cursor, 1);
        // Parse the offset in the line properties section.
        parseDouble(cursor, jstring, "propertiesOffset", level);
        // Parse the color of the cartographic line.
        parseColorPattern(cursor, jstring, "Cartographic Line Color", level);
        // Parse the TEMPLATE
        parseTemplate(cursor, jstring, 0, level);
    } catch (std::string err) {
        throw err;
    }

    return 0;
}

int parseHashLine(char **cursor, std::string &jstring, int level) {
    LOG("Type: Hash Line...");
    write_to_json(jstring, "type", "\"Hash Line\",", level);

    try {
        parseDouble(cursor, jstring, "hashLineAngle", level);
        parseLineCaps(cursor, jstring, level);
        parseLineJoins(cursor, jstring, level);
        parseDouble(cursor, jstring, "cartographicLineWidth", level);
        bytesHopper(cursor, 1);
        parseDouble(cursor, jstring, "propertiesOffset", level);
        parseLinePattern(cursor, jstring, 1, "Outline", level);
        parseColorPattern(cursor, jstring, "Cartographic Line Color", level);
        parseTemplate(cursor, jstring, 0, level);
    } catch (std::string err) {
        throw err;
    }

    return 0;
}

int parseMarkerLine(char **cursor, std::string &jstring, int level) {
    LOG("Type: Marker Line...");
    write_to_json(jstring, "type", "\"Marker Line\",", level);

    bytesHopper(cursor, 1);
    parseDouble(cursor, jstring, "propertiesOffset", level);

    try {
        parseMarkerPattern(cursor, jstring, level);
        parseTemplate(cursor, jstring, 1, level);
        parseLineCaps(cursor, jstring, level);
        parseLineJoins(cursor, jstring, level);
    } catch (std::string err) {
        throw err;
    }

    //while (0x0D != getChar(cursor)) {}
    bytesHopper(cursor, 8);

    return 0;
}

int parseLineCaps(char **cursor, std::string &jstring, int level) {
    int line_caps_code = getChar(cursor);
    std::string line_caps_name = "";

    // Select different line caps.
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
            LOG("ERROR: Line caps code " + std::to_string(line_caps_code) + " not found." );
            throw std::string("Line caps code.");
    }

    LOG("The line cap is " + line_caps_name);
    write_to_json(jstring, "lineCaps", "\"" + line_caps_name + "\",", level);

    bytesHopper(cursor, 3);

    return line_caps_code;
}

int parseLineJoins(char **cursor, std::string &jstring, int level) {
    int line_joins_code = getChar(cursor);
    std::string line_joins_name = "";

    // Select different line joins.
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
            LOG("ERROR: Line joins code " + std::to_string(line_joins_code) + " not found." );
            throw std::string("Line join code.");
    }

    LOG("The line join is " + line_joins_name);
    write_to_json(jstring, "lineJoins", "\"" + line_joins_name + "\",", level);

    bytesHopper(cursor, 3);

    return line_joins_code;
}


int parseLineStyle(char **cursor, std::string &jstring, int level) {
    int line_style_code = getChar(cursor);
    std::string line_style_name = "";

    // Select different line styles.
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
            LOG("ERROR: Line style code " + std::to_string(line_style_code) + " not found." );
            throw std::string("Line style code.");
    }

    LOG("The line style is " + line_style_name);
    write_to_json(jstring, "style", "\"" + line_style_name + "\",", level);

    bytesHopper(cursor, 3);

    return line_style_code;
}
