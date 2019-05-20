//
//  marker_parser.cc
//
//  Created by Erick Li on 04/16/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#include "marker_parser.hh"
#include "parser.hh"

int parseMarkerPattern(char **cursor, std::string &jstring, int level, bool printToFile) {
    std::cout << "-----------------------------" << std::endl;
    std::cout << "START parsing marker" << std::endl;
    if (printToFile) {
        write_to_json(jstring, "markerProperties", "{", level);
        // write_to_json(jstring, "name", "\"" + property + "\",", level + 1);
    }

    int num_of_marker_layers = 1;

    int marker_type_precheck = get16Bit(cursor);

    try {
        if (58879 == marker_type_precheck) { // FFE5
            // Validate if the header is there.
            if (0 != hexValidation(cursor, "147992C8D0118BB6080009EE4E41", !DO_REWIND)) {
                std::cout << "ERROR: Fail to validate marker pattern header..." << std::endl;
                throw std::string("validation.");
            }
            bytesHopper(cursor, 10);
            parseDouble(cursor, jstring, "markerSize", level + 1, printToFile);
            bytesHopper(cursor, 24);
            parseColorPattern(cursor, jstring, "Marker Color", level + 1, printToFile);

            write_to_json(jstring, "mask", "{", level + 1);
            parseMaskTypes(cursor, jstring, level + 2, printToFile);
            parseDouble(cursor, jstring, "size", level + 2, printToFile);
            num_of_marker_layers = parseLayer(cursor, jstring, 1, level + 2, 0, printToFile);
            write_to_json(jstring, "", "},", level + 1);

            write_to_json(jstring, "numberOfMarkerLayers", std::to_string(num_of_marker_layers), level + 1);
        } else {
            bytesRewinder(cursor, 2); // Move back to the front
        }

        // Parse different marker layers.
        for (int i = 0; i < num_of_marker_layers; i++) {
            // Indicate the number of the marker layer.
            write_to_json(jstring, "markerLayer" + std::to_string(i + 1), "{", level + 1);
            write_to_json(jstring, "number", std::to_string(i + 1), level + 2);
            int marker_type = get16Bit(cursor);
            // this part can be deleted in the future...
            if (58876 == marker_type) {
                bytesHopper(cursor, 29);
                marker_type = getChar(cursor);
            }

            bytesHopper(cursor, 16);

            if (num_of_marker_layers - 1 == i) {
                switch(marker_type) {
                    case 58878:
                        parseSimpleMarker(cursor, jstring, level + 2, printToFile, true);
                        break;
                    case 58880:
                        parseCharacterMarker(cursor, jstring, level + 2, printToFile, true);
                        break;
                    case 37937:
                        parseArrowMarker(cursor, jstring, level + 2, printToFile, true);
                        break;
                    default:
                        std::cout << "ERROR: Line type " << std::to_string(marker_type) << " not found." << std::endl;
                        throw std::string("Line type.");
                }
            } else {
                switch(marker_type) {
                    case 58878:
                        parseSimpleMarker(cursor, jstring, level + 2, printToFile, false);
                        break;
                    case 58880:
                        parseCharacterMarker(cursor, jstring, level + 2, printToFile, false);
                        break;
                    case 37937:
                        parseArrowMarker(cursor, jstring, level + 2, printToFile, false);
                        break;
                    default:
                        std::cout << "ERROR: Line type " << std::to_string(marker_type) << " not found." << std::endl;
                        throw std::string("Line type.");
                }
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

int parseSimpleMarker(char **cursor, std::string &jstring, int level, bool printToFile, bool withTail) {
    std::cout << "Type: Simple Marker..." << std::endl;
    if (printToFile) {
        write_to_json(jstring, "type", "\"Simple Marker\",", level);
    }

    try {
        parseColorPattern(cursor, jstring, "color", level, printToFile);
        parseDouble(cursor, jstring, "size", level, printToFile);
        parseMarkerStyle(cursor, jstring, level, printToFile);
        bytesHopper(cursor, 16);
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
    } catch (std::string err) {
        throw err;
    }

    if (withTail) {
        bytesHopper(cursor, 32);
    } else {
        bytesRewinder(cursor, 1);
    }

    return 0;
}

int parseCharacterMarker(char **cursor, std::string &jstring, int level, bool printToFile, bool withTail) {
    std::cout << "Type: Character Marker..." << std::endl;
    if (printToFile) {
        write_to_json(jstring, "type", "\"Character Marker\",", level);
    }

    try {
        parseColorPattern(cursor, jstring, "Marker Color", level, !PRINT_TO_FILE);
        parseInt(cursor, jstring, "unicode", level, printToFile);
        parseDouble(cursor, jstring, "markerAngle", level, printToFile);
        parseDouble(cursor, jstring, "markerSize", level, printToFile);
        parseDouble(cursor, jstring, "markerOffsetX", level, printToFile);
        parseDouble(cursor, jstring, "markerOffsetY", level, printToFile);
        bytesHopper(cursor, 16);

        int title = getChar(cursor);

        if (title == 13) {
            bytesHopper(cursor, 13);
        } else {
            bytesRewinder(cursor, 1);
        }

        parseString(cursor, jstring, "font", level, printToFile);
    } catch (std::string err) {
        throw err;
    }

    if (withTail) {
        // Parse the TAIL pattern of carto line.
        while (getChar(cursor) < 20) {}
        bytesRewinder(cursor, 1);
    } else {
        bytesRewinder(cursor, 1);
    }

    return 0;
}

int parseArrowMarker(char **cursor, std::string &jstring, int level, bool printToFile, bool withTail) {
    std::cout << "Type: Arrow Marker..." << std::endl;
    if (printToFile) {
        write_to_json(jstring, "type", "\"Arrow Marker\",", level);
    }

    try {
        parseColorPattern(cursor, jstring, "Marker Color", level, printToFile);
        parseDouble(cursor, jstring, "size", level, printToFile);
        parseDouble(cursor, jstring, "width", level, printToFile);
        parseDouble(cursor, jstring, "angle", level, printToFile);

        bytesHopper(cursor, 12);

        parseDouble(cursor, jstring, "XOffset", level, printToFile);
        parseDouble(cursor, jstring, "YOffset", level, printToFile);

        if (withTail) {
            bytesHopper(cursor, 32);
        } else {
            bytesRewinder(cursor, 1);
        }
    } catch (std::string err) {
        throw err;
    }

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
        throw std::string("Marker type.");
    }

    std::cout << "The marker type is " << marker_type_name << std::endl;
    if (printToFile) {
        write_to_json(jstring, "style", "\"" + marker_type_name + "\",", level);
    }

    bytesHopper(cursor, 3);

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
        throw std::string("Mask type.");
    }

    std::cout << "The mask type is " << mask_type_name << std::endl;
    if (printToFile) {
        write_to_json(jstring, "style", "\"" + mask_type_name + "\",", level);
    }

    bytesHopper(cursor, 3);

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
            throw std::string("Marker style.");
    }

    std::cout << "The marker style is " << marker_style_name << std::endl;
    if (printToFile) {
        write_to_json(jstring, "style", "\"" + marker_style_name + "\",", level);
    }

    bytesHopper(cursor, 3);

    return marker_style_code;
}
