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

    if (0xE5FF == marker_type_precheck) { // 58879
        try {
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
            parseLayer(cursor, jstring, 1, level + 2, 0);
            write_to_json(jstring, "", "},", level + 1);
        } catch (std::string err) {
            throw err;
        }

        int b = 0;
        do {
            b = getChar(cursor);
            if (0 < b && b < 10) {
                num_of_marker_layers = b;
            }
        } while (b != 0x14 && b != 0x53); // While b is not 0x14 and 0x53.

        // trace back to the front of the header
        bytesRewinder(cursor, 3);

        write_to_json(jstring, "numberOfMarkerLayers", std::to_string(num_of_marker_layers) + ",", level + 1);
    } else {
        LOG(" ##### WARNING: Marker type abnormal. Marker code: " + std::to_string(marker_type_precheck));
        bytesRewinder(cursor, 2); // Move back to the front
    }

    LOG("Number of Marker Layers: " + std::to_string(num_of_marker_layers));

    // Parse different marker layers.
    for (int i = 0; i < num_of_marker_layers; i++) {
        LOG(" ^^^^START parsing marker layer NO. " + std::to_string(i + 1));
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

        try {
            // When we reach the last layer
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
        } catch (std::string err) {
            throw err;
        }

        write_to_json(jstring, "", "},", level + 1);
    }

    // Parse the activeness of layers.
    int check_active = getChar(cursor);
    bytesRewinder(cursor, 1);
    if (check_active == 0 || check_active == 1) {
        LOG("Checking marker activeness...");
        write_to_json(jstring, "lineActiveness", "{", level + 1);
        for (size_t i = 0; i < num_of_marker_layers; i++) {
            int activeness = getInt(cursor); // 0: deactivated; 1: activated
            LOG("Activeness: " + std::to_string(activeness));
            write_to_json(jstring, "layer" + std::to_string(i + 1), std::to_string(activeness) + ",", level + 2);
        }
        write_to_json(jstring, "", "}", level + 1);

        for (size_t i = 0; i < num_of_marker_layers; i++) {
            bytesHopper(cursor, 4); // Hop for (01 00 00 00) or (00 00 00 00)
        }
        bytesHopper(cursor, 16);
        for (size_t i = 0; i < num_of_marker_layers; i++) {
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

    bytesHopper(cursor, 2);

    return 0;
}

int parseCharacterMarker(char **cursor, std::string &jstring, int level, bool withTail) {
    LOG("Type: Character Marker...");
    write_to_json(jstring, "type", "\"Character Marker\",", level);

    try {
        parseColorPattern(cursor, jstring, "Marker Color", level);
    } catch (std::string err) {
        throw err;
    }

    parseInt(cursor, jstring, "unicode", level);
    parseDouble(cursor, jstring, "markerAngle", level);
    parseDouble(cursor, jstring, "markerSize", level);
    parseDouble(cursor, jstring, "markerOffsetX", level);
    parseDouble(cursor, jstring, "markerOffsetY", level);
    bytesHopper(cursor, 16);

    int stnl = getChar(cursor);

    if (stnl == 0x0D) {
        bytesHopper(cursor, 13);
    } else {
        bytesRewinder(cursor, 1);
    }

    try {
        parseString(cursor, jstring, "font", level);
    } catch (std::string err) {
        throw err;
    }

    return 0;
}

int parseArrowMarker(char **cursor, std::string &jstring, int level, bool withTail) {
    LOG("Type: Arrow Marker...");
    write_to_json(jstring, "type", "\"Arrow Marker\",", level);

    try {
        parseColorPattern(cursor, jstring, "Marker Color", level);
    } catch (std::string err) {
        throw err;
    }

    parseDouble(cursor, jstring, "size", level);
    parseDouble(cursor, jstring, "width", level);
    parseDouble(cursor, jstring, "angle", level);

    bytesHopper(cursor, 12);

    parseDouble(cursor, jstring, "XOffset", level);
    parseDouble(cursor, jstring, "YOffset", level);

    bytesHopper(cursor, 2);

    // if (withTail) {
    //     bytesHopper(cursor, 32);
    // } else {
    //     bytesHopper(cursor, 2);
    // }

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
