#include "line_parser.hh"

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
        moveBytes(infile, 22);
    } else if (property == "Filling Line") {
        moveBytes(infile, 8);
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
