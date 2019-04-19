#include "line_parser.hh"

int parseLinePattern(char **cursor, std::string &jstring, int type, std::string property, int level, bool printToFile) {
    std::cout << "-----------------------------" << std::endl;
    if (type == 0) {
        std::cout << "START parsing line" << std::endl;
        if (printToFile) {
            write_to_json(jstring, "lineProperties", "{", level);
            write_to_json(jstring, "name", "\"" + property + "\",", level + 1);
        }
    } else {
        std::cout << "START parsing line symbol" << std::endl;
        if (printToFile) {
            write_to_json(jstring, "lineSymbol", "{", level);
        }
    }

    // Get the line type
    int line_type = getChar(cursor);

    // When the line type code is 0xFA, then extra hops are needed.
    if (250 == line_type) {
        bytesHopper(cursor, 29);
        line_type = getChar(cursor);
    }

    bytesHopper(cursor, 17);

    switch(line_type) {
        case 249:
            parseSimpleLine(cursor, jstring, level + 1, printToFile);
            break;
        case 251:
            parseCartoLine(cursor, jstring, level + 1, printToFile);
            break;
        case 252:
            parseHashLine(cursor, jstring, level + 1, printToFile);
            break;
        case 253:
            parseMarkerLine(cursor, jstring, level + 1, printToFile);
            break;
        default:
            std::cout << "ERROR: Line type " << std::to_string(line_type) << " not found." << std::endl;
            return -1;
    }

    if (printToFile) {
        write_to_json(jstring, "", "},", level);
    }

    return 0;
}

int parseSimpleLine(char **cursor, std::string &jstring, int level, bool printToFile) {
    std::cout << "Type: Simple Line..." << std::endl;
    if (printToFile) {
        write_to_json(jstring, "type", "\"Simple Line\",", level);
    }

    // Parse the color for the line.
    parseColorPattern(cursor, jstring, "Simple Line Color", level, printToFile);
    // Parse the width of the line.
    parseDouble(cursor, jstring, "width", level, printToFile);
    // Parse the line style of the line. (Solid, dashed, dotted, dash-dot, dash-dot-dot, null)
    parseLineStyle(cursor, jstring, level, printToFile);

    // Parse the TAIL pattern of simple line.
    while (getChar(cursor) < 20) {}
    bytesRewinder(cursor, 1);

    return 0;
}

int parseCartoLine(char **cursor, std::string &jstring, int level, bool printToFile) {
    std::cout << "Type: Cartographic Line..." << std::endl;
    if (printToFile) {
        write_to_json(jstring, "type", "\"Cartographic Line\",", level);
    }

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

    bytesHopper(cursor, 14);

    return 0;
}

int parseHashLine(char **cursor, std::string &jstring, int level, bool printToFile) {
    std::cout << "Type: Hash Line..." << std::endl;
    if (printToFile) {
        write_to_json(jstring, "type", "\"Hash Line\",", level);
    }

    parseDouble(cursor, jstring, "hashLineAngle", level, printToFile);
    parseLineCaps(cursor, jstring, level, printToFile);
    parseLineJoins(cursor, jstring, level, printToFile);
    parseDouble(cursor, jstring, "cartographicLineWidth", level, printToFile);
    bytesHopper(cursor, 1);
    parseDouble(cursor, jstring, "propertiesOffset", level, printToFile);
    parseLinePattern(cursor, jstring, 1, "Outline", level, printToFile);
    parseColorPattern(cursor, jstring, "Cartographic Line Color", level, printToFile);
    parseTemplate(cursor, jstring, 0, level, printToFile);

    bytesHopper(cursor, 14);

    return 0;
}

int parseMarkerLine(char **cursor, std::string &jstring, int level, bool printToFile) {
    std::cout << "Type: Hash Line..." << std::endl;
    if (printToFile) {
        write_to_json(jstring, "type", "\"Marker Line\",", level);
    }

    bytesHopper(cursor, 1);
    parseDouble(cursor, jstring, "propertiesOffset", level, printToFile);
    parseMarkerPattern(cursor, jstring, level, printToFile); // TODO...
    parseTemplate(cursor, jstring, 1, level, printToFile);
    parseLineCaps(cursor, jstring, level, printToFile);
    parseLineJoins(cursor, jstring, level, printToFile);
    parseDouble(cursor, jstring, "cartographicLineWidth", level, printToFile);

    bytesHopper(cursor, 14);

    return 0;
}

int parseLineCaps(char **cursor, std::string &jstring, int level, bool printToFile) {
    int line_caps_code = getChar(cursor);
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
        write_to_json(jstring, "lineCaps", "\"" + line_caps_name + "\",", level);
    }

    bytesHopper(cursor, 3);

    return line_caps_code;
}

int parseLineJoins(char **cursor, std::string &jstring, int level, bool printToFile) {
    int line_joins_code = getChar(cursor);
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
        write_to_json(jstring, "lineJoins", "\"" + line_joins_name + "\",", level);
    }

    bytesHopper(cursor, 3);

    return line_joins_code;
}


int parseLineStyle(char **cursor, std::string &jstring, int level, bool printToFile) {
    int line_style_code = getChar(cursor);
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
        write_to_json(jstring, "style", "\"" + line_style_name + "\",", level);
    }

    bytesHopper(cursor, 3);

    return line_style_code;
}
