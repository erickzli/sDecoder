#include "marker_parser.hh"

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
