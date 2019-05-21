//
//  line_parser.cc
//
//  Created by Erick Li on 04/16/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#include "line_parser.hh"
#include "parser.hh"

int parseLinePattern(char **cursor, std::string &jstring, int type, std::string property, int level, bool printToFile) {
    LOG("-----------------------------");
    if (type == 0) {
        LOG("START parsing line");
        if (printToFile) {
            write_to_json(jstring, "lineProperties", "{", level);
            write_to_json(jstring, "name", "\"" + property + "\",", level + 1);
        }
    } else {
        LOG("START parsing line symbol");
        if (printToFile) {
            write_to_json(jstring, "lineSymbol", "{", level);
        }
    }


    // Get the line type
    int line_type = getChar(cursor);
    bytesRewinder(cursor, 1);
    // Set the default number of line layers to 1
    int num_of_line_layers = 1;

    // When the line type code is 0xFA, we need to find the info like layer number from it.
    if (0xFA == line_type) {
        bytesHopper(cursor, 26);
        num_of_line_layers = getInt(cursor);
    }

    write_to_json(jstring, "numberOfLineLayers", std::to_string(num_of_line_layers) + ",", level + 1);

    try {
        // Go through each line layer.
        for (int i = 0; i < num_of_line_layers; i++) {
            LOG(" --- START parsing line layer NO. " + std::to_string(i + 1));
            line_type = getChar(cursor);
            bytesHopper(cursor, 17);

            write_to_json(jstring, "lineLayer" + std::to_string(i + 1), "{", level + 1);

            switch(line_type) {
                case 0xF9:
                    parseSimpleLine(cursor, jstring, level + 2, printToFile);
                    break;
                case 0xFB:
                    parseCartoLine(cursor, jstring, level + 2, printToFile);
                    break;
                case 0xFC:
                    parseHashLine(cursor, jstring, level + 2, printToFile);
                    break;
                case 0xFD:
                    parseMarkerLine(cursor, jstring, level + 2, printToFile);
                    break;
                default:
                    LOG("ERROR: Line type " + std::to_string(line_type) + " not found.");
                    throw std::string("Line type.");
            }

            write_to_json(jstring, "", "},", level + 1);
        }
    } catch (std::string err) {
        throw err;
    }

    if (printToFile) {
        write_to_json(jstring, "", "},", level);
    }

    return 0;
}

int parseSimpleLine(char **cursor, std::string &jstring, int level, bool printToFile) {
    LOG("Type: Simple Line...");
    if (printToFile) {
        write_to_json(jstring, "type", "\"Simple Line\",", level);
    }

    try {
        // Parse the color for the line.
        parseColorPattern(cursor, jstring, "Simple Line Color", level, printToFile);
        // Parse the width of the line.
        parseDouble(cursor, jstring, "width", level, printToFile);
        // Parse the line style of the line. (Solid, dashed, dotted, dash-dot, dash-dot-dot, null)
        parseLineStyle(cursor, jstring, level, printToFile);
    } catch (std::string err) {
        throw err;
    }

    // Parse the TAIL pattern of simple line.
    while (getChar(cursor) < 20) {}
    bytesRewinder(cursor, 1);

    return 0;
}

int parseCartoLine(char **cursor, std::string &jstring, int level, bool printToFile) {
    LOG("Type: Cartographic Line...");
    if (printToFile) {
        write_to_json(jstring, "type", "\"Cartographic Line\",", level);
    }

    try {
        // Parse line caps (butt, round, square)
        parseLineCaps(cursor, jstring, level, printToFile);
        // Parse line joins (miter, round, bevel)
        parseLineJoins(cursor, jstring, level, printToFile);
        // Parse the cartographic line width
        parseDouble(cursor, jstring, "cartographicLineWidth", level, printToFile);
        bytesHopper(cursor, 1);
        // Parse the offset in the line properties section.
        parseDouble(cursor, jstring, "propertiesOffset", level, printToFile);
        // Parse the color of the cartographic line.
        parseColorPattern(cursor, jstring, "Cartographic Line Color", level, printToFile);
        // Parse the TEMPLATE
        parseTemplate(cursor, jstring, 0, level, printToFile);
    } catch (std::string err) {
        throw err;
    }

    // Parse the TAIL pattern of carto line.
    bool exit_cond = false;
    while (!exit_cond) {
        int sentinel = getChar(cursor);
        if (1 == sentinel) {
            bytesHopper(cursor, 7);
        } else if (2 == sentinel) {
            bytesHopper(cursor, 5);
        } else {
            bytesRewinder(cursor, 1);
            exit_cond = true;
        }
    }

    return 0;
}

int parseHashLine(char **cursor, std::string &jstring, int level, bool printToFile) {
    LOG("Type: Hash Line...");
    if (printToFile) {
        write_to_json(jstring, "type", "\"Hash Line\",", level);
    }

    try {
        parseDouble(cursor, jstring, "hashLineAngle", level, printToFile);
        parseLineCaps(cursor, jstring, level, printToFile);
        parseLineJoins(cursor, jstring, level, printToFile);
        parseDouble(cursor, jstring, "cartographicLineWidth", level, printToFile);
        bytesHopper(cursor, 1);
        parseDouble(cursor, jstring, "propertiesOffset", level, printToFile);
        parseLinePattern(cursor, jstring, 1, "Outline", level, printToFile);
        parseColorPattern(cursor, jstring, "Cartographic Line Color", level, printToFile);
        parseTemplate(cursor, jstring, 0, level, printToFile);
    } catch (std::string err) {
        throw err;
    }

    // Parse the TAIL pattern of carto line.
    bool exit_cond = false;
    while (!exit_cond) {
        int sentinel = getChar(cursor);
        if (1 == sentinel) {
            bytesHopper(cursor, 7);
        } else if (2 == sentinel) {
            bytesHopper(cursor, 5);
        } else {
            bytesRewinder(cursor, 1);
            exit_cond = true;
        }
    }

    return 0;
}

int parseMarkerLine(char **cursor, std::string &jstring, int level, bool printToFile) {
    LOG("Type: Hash Line...");
    if (printToFile) {
        write_to_json(jstring, "type", "\"Marker Line\",", level);
    }

    try {
        bytesHopper(cursor, 1);
        parseDouble(cursor, jstring, "propertiesOffset", level, printToFile);
        parseMarkerPattern(cursor, jstring, level, printToFile);
        parseTemplate(cursor, jstring, 1, level, printToFile);
        parseLineCaps(cursor, jstring, level, printToFile);
        parseLineJoins(cursor, jstring, level, printToFile);
        parseDouble(cursor, jstring, "cartographicLineWidth", level, printToFile);

        bytesHopper(cursor, 14);
    } catch (std::string err) {
        throw err;
    }

    return 0;
}

int parseLineCaps(char **cursor, std::string &jstring, int level, bool printToFile) {
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
    if (printToFile) {
        write_to_json(jstring, "lineCaps", "\"" + line_caps_name + "\",", level);
    }

    bytesHopper(cursor, 3);

    return line_caps_code;
}

int parseLineJoins(char **cursor, std::string &jstring, int level, bool printToFile) {
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
    if (printToFile) {
        write_to_json(jstring, "lineJoins", "\"" + line_joins_name + "\",", level);
    }

    bytesHopper(cursor, 3);

    return line_joins_code;
}


int parseLineStyle(char **cursor, std::string &jstring, int level, bool printToFile) {
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
    if (printToFile) {
        write_to_json(jstring, "style", "\"" + line_style_name + "\",", level);
    }

    bytesHopper(cursor, 3);

    return line_style_code;
}
