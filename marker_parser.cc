#include "marker_parser.hh"

int parseCharacterMarker(char **cursor, std::string &jstring, int level, bool printToFile) {
    std::cout << "Type: Character Marker..." << std::endl;
    if (printToFile) {
        write_to_json(jstring, "type", "\"Character Marker\",", level);
    }

    moveBytes(cursor, 18);
    moveBytes(cursor, 8);
    parseDouble(cursor, jstring, "markerSize", level, printToFile);
    moveBytes(cursor, 24);
    parseColorPattern(cursor, jstring, "Marker Color", level, printToFile);
    parseMaskTypes(cursor, jstring, level, printToFile);
    parseDouble(cursor, jstring, "maskSize", level, printToFile);
    parseLayer(cursor, jstring, 1, level, 0, printToFile); // TODO

    // Validate if the header is there.
    if (0 != hexValidation(cursor, "E6147992C8D0118BB6080009EE4E41", !DO_REWIND)) {
        std::cout << "ERROR: Fail to validate Char Marker pattern header..." << std::endl;
        return -1;
    }
    moveBytes(cursor, 2);

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
