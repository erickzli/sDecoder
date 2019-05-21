//
//  marker_parser.cc
//
//  Created by Erick Li on 04/16/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#include "marker_parser.hh"
#include "parser.hh"

int parseMarkerPattern(char **cursor, std::string &jstring, int level) {
    LOG("-----------------------------");
    LOG("START parsing marker");
    write_to_json(jstring, "markerProperties", "{", level);

    int num_of_marker_layers = 1;
    int marker_type_precheck = get16Bit(cursor);

    try {
        if (0xE5FF == marker_type_precheck) { // 58879
            // Validate if the header is there.
            if (0 != hexValidation(cursor, "147992C8D0118BB6080009EE4E41", !DO_REWIND)) {
                LOG("ERROR: Fail to validate marker pattern header...");
                throw std::string("Marker type validation.");
            }
            bytesHopper(cursor, 10);
            parseDouble(cursor, jstring, "markerSize", level + 1);
            bytesHopper(cursor, 24);
            parseColorPattern(cursor, jstring, "Marker Color", level + 1);

            write_to_json(jstring, "mask", "{", level + 1);
            parseMaskTypes(cursor, jstring, level + 2);
            parseDouble(cursor, jstring, "size", level + 2);
            num_of_marker_layers = parseLayer(cursor, jstring, 1, level + 2, 0);
            write_to_json(jstring, "", "},", level + 1);

            write_to_json(jstring, "numberOfMarkerLayers", std::to_string(num_of_marker_layers) + ",", level + 1);
        } else {
            LOG("WARNING: Marker type abnormal.");
            bytesRewinder(cursor, 2); // Move back to the front
        }

        // Parse different marker layers.
        for (int i = 0; i < num_of_marker_layers; i++) {
            // Indicate the number of the marker layer.
            write_to_json(jstring, "markerLayer" + std::to_string(i + 1), "{", level + 1);
            write_to_json(jstring, "number", std::to_string(i + 1), level + 2);
            int marker_type = get16Bit(cursor);
            // this part can be deleted in the future...
            if (0xE5FC == marker_type) {
                bytesHopper(cursor, 29);
                marker_type = getChar(cursor);
            }

            bytesHopper(cursor, 16);

            if (num_of_marker_layers - 1 == i) {
                switch(marker_type) {
                    case 0xE5FE: // 58878
                        parseSimpleMarker(cursor, jstring, level + 2, true);
                        break;
                    case 0xE600: // 58880
                        parseCharacterMarker(cursor, jstring, level + 2, true);
                        break;
                    case 0x9431: // 37937
                        parseArrowMarker(cursor, jstring, level + 2, true);
                        break;
                    default:
                        LOG("ERROR: Marker type " + std::to_string(marker_type) + " not found.");
                        throw std::string("Marker type.");
                }
            } else {
                switch(marker_type) {
                    case 0xE5FE:
                        parseSimpleMarker(cursor, jstring, level + 2, false);
                        break;
                    case 0xE600:
                        parseCharacterMarker(cursor, jstring, level + 2, false);
                        break;
                    case 0x9431:
                        parseArrowMarker(cursor, jstring, level + 2, false);
                        break;
                    default:
                        LOG("ERROR: Marker type " + std::to_string(marker_type) + " not found.");
                        throw std::string("Marker type.");
                }
            }
            write_to_json(jstring, "", "},", level + 1);
        }
    } catch (std::string err) {
        throw err;
    }

    write_to_json(jstring, "", "},", level);

    return 0;
}

int parseSimpleMarker(char **cursor, std::string &jstring, int level, bool withTail) {
    LOG("Type: Simple Marker...");
    write_to_json(jstring, "type", "\"Simple Marker\",", level);

    try {
        parseColorPattern(cursor, jstring, "color", level);
        parseDouble(cursor, jstring, "size", level);
        parseMarkerStyle(cursor, jstring, level);
        bytesHopper(cursor, 16);
        parseDouble(cursor, jstring, "offsetX", level);
        parseDouble(cursor, jstring, "offsetY", level);

        bool showOutline = bool(getChar(cursor));
        if (showOutline) {
            write_to_json(jstring, "showOutline", "true,", level);
        } else {
            write_to_json(jstring, "showOutline", "false,", level);
        }
        parseDouble(cursor, jstring, "outlineWidth", level);
        parseColorPattern(cursor, jstring, "outline", level);
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

int parseCharacterMarker(char **cursor, std::string &jstring, int level, bool withTail) {
    LOG("Type: Character Marker...");
    write_to_json(jstring, "type", "\"Character Marker\",", level);

    try {
        parseColorPattern(cursor, jstring, "Marker Color", level);
        parseInt(cursor, jstring, "unicode", level);
        parseDouble(cursor, jstring, "markerAngle", level);
        parseDouble(cursor, jstring, "markerSize", level);
        parseDouble(cursor, jstring, "markerOffsetX", level);
        parseDouble(cursor, jstring, "markerOffsetY", level);
        bytesHopper(cursor, 16);

        int title = getChar(cursor);

        if (title == 13) {
            bytesHopper(cursor, 13);
        } else {
            bytesRewinder(cursor, 1);
        }

        parseString(cursor, jstring, "font", level);
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

int parseArrowMarker(char **cursor, std::string &jstring, int level, bool withTail) {
    LOG("Type: Arrow Marker...");
    write_to_json(jstring, "type", "\"Arrow Marker\",", level);

    try {
        parseColorPattern(cursor, jstring, "Marker Color", level);
        parseDouble(cursor, jstring, "size", level);
        parseDouble(cursor, jstring, "width", level);
        parseDouble(cursor, jstring, "angle", level);

        bytesHopper(cursor, 12);

        parseDouble(cursor, jstring, "XOffset", level);
        parseDouble(cursor, jstring, "YOffset", level);

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

int parseMarkerTypes(char **cursor, std::string &jstring, int level) {
    int marker_type_code = getChar(cursor);
    std::string marker_type_name = "";

    if (marker_type_code == 0) {
        marker_type_name = "Grid";
    } else if (marker_type_code == 1) {
        marker_type_name = "Random";
    } else {
        LOG("ERROR: Marker type code " + std::to_string(marker_type_code) + " not found.");
        throw std::string("Marker type.");
    }

    LOG("The marker type is " + marker_type_name);
    write_to_json(jstring, "style", "\"" + marker_type_name + "\",", level);

    bytesHopper(cursor, 3);

    return marker_type_code;
}

int parseMaskTypes(char **cursor, std::string &jstring, int level) {
    int mask_type_code = getChar(cursor);
    std::string mask_type_name = "";

    if (mask_type_code == 0) {
        mask_type_name = "None";
    } else if (mask_type_code == 1) {
        mask_type_name = "Halo";
    } else {
        LOG("ERROR: Mask type code " + std::to_string(mask_type_code) + " not found.");
        throw std::string("Mask type.");
    }

    LOG("The mask type is " + mask_type_name);
    write_to_json(jstring, "style", "\"" + mask_type_name + "\",", level);

    bytesHopper(cursor, 3);

    return mask_type_code;
}


int parseMarkerStyle(char **cursor, std::string &jstring, int level) {
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
            LOG("ERROR: Marker style code " + std::to_string(marker_style_code) + " not found." );
            throw std::string("Marker style.");
    }

    LOG("The marker style is " + marker_style_name);
    write_to_json(jstring, "style", "\"" + marker_style_name + "\",", level);

    bytesHopper(cursor, 3);

    return marker_style_code;
}
