#include "marker_parser.hh"

int parseMarkerPattern(char **cursor, std::string &jstring, int level, bool printToFile) {
    std::cout << "-----------------------------" << std::endl;
    std::cout << "START parsing marker" << std::endl;
    if (printToFile) {
        write_to_json(jstring, "markerProperties", "{", level);
        // write_to_json(jstring, "name", "\"" + property + "\",", level + 1);
    }

    // Validate if the header is there.
    if (0 != hexValidation(cursor, "FFE5147992C8D0118BB6080009EE4E41", !DO_REWIND)) {
        std::cout << "ERROR: Fail to validate Char Marker pattern header..." << std::endl;
        return -1;
    }
    moveBytes(cursor, 2);

    moveBytes(cursor, 8);
    parseDouble(cursor, jstring, "markerSize", level, printToFile);
    moveBytes(cursor, 24);
    parseColorPattern(cursor, jstring, "Marker Color", level, printToFile);

    write_to_json(jstring, "mask", "{", level);
    parseMaskTypes(cursor, jstring, level + 1, printToFile);
    parseDouble(cursor, jstring, "size", level + 1, printToFile);
    parseLayer(cursor, jstring, 1, level + 1, 0, printToFile);
    write_to_json(jstring, "", "}", level);

    int marker_type = get16Bit(cursor);

    // this part can be deleted in the future...
    if (58876 == marker_type) {
        moveBytes(cursor, 29);
        marker_type = getChar(cursor);
    }

    moveBytes(cursor, 16);
    switch(marker_type) {
        case 58878:
            parseSimpleMarker(cursor, jstring, level + 1, printToFile);
            break;
        case 58880:
            parseCharacterMarker(cursor, jstring, level + 1, printToFile);
            break;
        default:
            std::cout << "ERROR: Line type " << std::to_string(marker_type) << " not found." << std::endl;
            return -1;
    }

    if (printToFile) {
        write_to_json(jstring, "", "},", level);
    }

    return 0;
}

int parseSimpleMarker(char **cursor, std::string &jstring, int level, bool printToFile) {
    std::cout << "Type: Simple Marker..." << std::endl;
    if (printToFile) {
        write_to_json(jstring, "type", "\"Simple Marker\",", level);
    }

    parseColorPattern(cursor, jstring, "color", level, printToFile);
    parseDouble(cursor, jstring, "size", level, printToFile);
    parseMarkerStyle(cursor, jstring, level, printToFile);
    moveBytes(cursor, 16);
    parseDouble(cursor, jstring, "offsetX", level, printToFile);
    parseDouble(cursor, jstring, "offsetY", level, printToFile);

    bool showOutline = bool(getChar(cursor));
    if (printToFile) {
        if (showOutline) {
            write_to_json(jstring, "showOutline", "true,", level);
        } else {
            write_to_json(jstring, "showOutline", "false,", level);
        }
    }
    parseDouble(cursor, jstring, "outlineWidth", level, printToFile);
    parseColorPattern(cursor, jstring, "outline", level, printToFile);

    moveBytes(cursor, 32);
    return 0;
}

int parseCharacterMarker(char **cursor, std::string &jstring, int level, bool printToFile) {
    std::cout << "Type: Character Marker..." << std::endl;
    if (printToFile) {
        write_to_json(jstring, "type", "\"Character Marker\",", level);
    }

    parseColorPattern(cursor, jstring, "Marker Color", level, !PRINT_TO_FILE);
    parseInt(cursor, jstring, "unicode", level, printToFile);
    parseDouble(cursor, jstring, "markerAngle", level, printToFile);
    parseDouble(cursor, jstring, "markerSize", level, printToFile);
    parseDouble(cursor, jstring, "markerOffsetX", level, printToFile);
    parseDouble(cursor, jstring, "markerOffsetY", level, printToFile);
    moveBytes(cursor, 16);

    int title = getChar(cursor);

    if (title == 13) {
        moveBytes(cursor, 13);
    } else {
        goRewind(cursor, 1);
    }

    parseString(cursor, jstring, "font", level, printToFile);

    return 0;
}

int parseMarkerTypes(char **cursor, std::string &jstring, int level, bool printToFile) {
    int marker_type_code = getChar(cursor);
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
        write_to_json(jstring, "style", "\"" + marker_type_name + "\",", level);
    }

    moveBytes(cursor, 3);

    return marker_type_code;
}

int parseMaskTypes(char **cursor, std::string &jstring, int level, bool printToFile) {
    int mask_type_code = getChar(cursor);
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
        write_to_json(jstring, "style", "\"" + mask_type_name + "\",", level);
    }

    moveBytes(cursor, 3);

    return mask_type_code;
}


int parseMarkerStyle(char **cursor, std::string &jstring, int level, bool printToFile) {
    int marker_style_code = getChar(cursor);
    std::string marker_style_name = "";

    switch(marker_style_code) {
        case 0:
            marker_style_name = "Circle";
            break;
        case 1:
            marker_style_name = "Square";
            break;
        case 2:
            marker_style_name = "Cross";
            break;
        case 3:
            marker_style_name = "X";
            break;
        case 4:
            marker_style_name = "Diamond";
            break;
        default:
            std::cout << "ERROR: Marker style code " << marker_style_code << " not found."  << std::endl;
            return -1;
    }

    std::cout << "The marker style is " << marker_style_name << std::endl;
    if (printToFile) {
        write_to_json(jstring, "style", "\"" + marker_style_name + "\",", level);
    }

    moveBytes(cursor, 3);

    return marker_style_code;
}
