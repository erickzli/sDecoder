//
//  marker_decoder.cc
//
//  Created by Erick Li on 04/16/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#include "marker_decoder.hh"
#include "decoder.hh"

int decodeMarkerPattern(char **cursor, std::string &jstring, int level) {
    LOG("-----------------------------");
    LOG("START decoding marker");
    write_to_json(jstring, "markerProperties", "{", level);

    int marker_type_precheck = get16Bit(cursor);
    int num_of_marker_layers = 1;

    if (0xE5FF == marker_type_precheck) { // 58879
        try {
            // Validate if the header is there.
            if (0 != hexValidation(cursor, "147992C8D0118BB6080009EE4E41", !DO_REWIND)) {
                LOG("ERROR: Fail to validate marker pattern header...");
                throw std::string("Marker type validation.");
            }
            bytesHopper(cursor, 10);
            decodeDouble(cursor, jstring, level + 1, "markerSize");
            bytesHopper(cursor, 24);
            decodeColorPattern(cursor, jstring, level + 1, "Marker Color");

            write_to_json(jstring, "mask", "{", level + 1);
            decodeMaskTypes(cursor, jstring, level + 2);
            decodeDouble(cursor, jstring, level + 2, "size");
            write_to_json(jstring, "fillSymbol", "{", level + 2);
            decodeLayer(cursor, jstring, level + 3, 1);
            write_to_json(jstring, "", "},", level + 2);
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
        LOG("Before rewinding, marker code: " + std::to_string(marker_type_precheck));
        bytesRewinder(cursor, 2); // Move back to the front
    }

    LOG("Number of Marker Layers: " + std::to_string(num_of_marker_layers));

    write_to_json(jstring, "markerLayer", "[", level + 1);
    // decode different marker layers.
    for (int i = 0; i < num_of_marker_layers; i++) {
        LOG(" >>>> START decoding marker layer NO. " + std::to_string(i + 1));
        // Indicate the number of the marker layer.
        write_to_json(jstring, "", "{", level + 2);
        write_to_json(jstring, "number", std::to_string(i + 1) + ",", level + 3);
        int marker_type = get16Bit(cursor);
        // this part can be deleted in the future...
        if (0xE5FC == marker_type) {
            bytesHopper(cursor, 29);
            marker_type = getChar(cursor);
        }

        bytesHopper(cursor, 16);

        try {
            switch(marker_type) {
                case 0xE5FE: // 58878
                    decodeSimpleMarker(cursor, jstring, level + 3);
                    break;
                case 0xE600: // 58880
                    decodeCharacterMarker(cursor, jstring, level + 3);
                    break;
                case 0x9431: // 37937
                    decodeArrowMarker(cursor, jstring, level + 3);
                    break;
                default:
                    LOG("ERROR: Marker type " + std::to_string(marker_type) + " not found.");
                    throw std::string("Marker type.");
            }
        } catch (std::string err) {
            throw err;
        }

        write_to_json(jstring, "", "},", level + 2);
    }

    write_to_json(jstring, "", "],", level + 1);


    int check_active_int = get32Bit(cursor);
    bytesRewinder(cursor, 4);
    double check_active_double = getDouble(cursor);
    bytesRewinder(cursor, 8);

    write_to_json(jstring, "markerLayerActiveness", "{", level + 1);
    if ((check_active_int == 0 || check_active_int == 1) && check_active_double < 0.5) {
        LOG("Checking marker activeness...");
        for (size_t i = 0; i < num_of_marker_layers; i++) {
            int activeness = get32Bit(cursor); // 0: deactivated; 1: activated
            LOG("Marker layer" + std::to_string(i + 1) + " Activeness: " + std::to_string(activeness));
            write_to_json(jstring, "layer" + std::to_string(i + 1), std::to_string(activeness) + ",", level + 2);
        }

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
    } else {
        for (size_t i = 0; i < num_of_marker_layers; i++) {
            LOG("Marker layer" + std::to_string(i + 1) + " Activeness: 2");
            write_to_json(jstring, "layer" + std::to_string(i + 1), "1,", level + 2);
        }
    }

    write_to_json(jstring, "", "},", level + 1);

    write_to_json(jstring, "", "},", level);

    return 0;
}

int decodeSimpleMarker(char **cursor, std::string &jstring, int level) {
    LOG("Type: Simple Marker...");
    write_to_json(jstring, "type", "\"Simple Marker\",", level);

    try {
        decodeColorPattern(cursor, jstring, level, "color");
        decodeDouble(cursor, jstring, level, "size");
        decodeMarkerStyle(cursor, jstring, level);
        bytesHopper(cursor, 16);
        decodeDouble(cursor, jstring, level, "offsetX");
        decodeDouble(cursor, jstring, level, "offsetY");

        bool showOutline = bool(getChar(cursor));
        LOG("Show outline: " + std::to_string(showOutline));
        if (showOutline) {
            write_to_json(jstring, "showOutline", "true,", level);
        } else {
            write_to_json(jstring, "showOutline", "false,", level);
        }
        decodeDouble(cursor, jstring, level, "outlineWidth");
        decodeColorPattern(cursor, jstring, level, "outline");
    } catch (std::string err) {
        throw err;
    }

    bytesHopper(cursor, 2);

    return 0;
}

int decodeCharacterMarker(char **cursor, std::string &jstring, int level) {
    LOG("Type: Character Marker...");
    write_to_json(jstring, "type", "\"Character Marker\",", level);

    try {
        decodeColorPattern(cursor, jstring, level, "Marker Color");
    } catch (std::string err) {
        throw err;
    }

    decodeInt(cursor, jstring, level, "unicode");
    decodeDouble(cursor, jstring, level, "markerAngle");
    decodeDouble(cursor, jstring, level, "markerSize");
    decodeDouble(cursor, jstring, level, "markerOffsetX");
    decodeDouble(cursor, jstring, level, "markerOffsetY");
    bytesHopper(cursor, 16);

    int stnl = getChar(cursor);

    if (stnl == 0x0D) {
        bytesHopper(cursor, 13);
    } else {
        bytesRewinder(cursor, 1);
    }

    try {
        decodeString(cursor, jstring, level, "font");
    } catch (std::string err) {
        throw err;
    }

    return 0;
}

int decodeArrowMarker(char **cursor, std::string &jstring, int level) {
    LOG("Type: Arrow Marker...");
    write_to_json(jstring, "type", "\"Arrow Marker\",", level);

    try {
        decodeColorPattern(cursor, jstring, level, "Marker Color");
    } catch (std::string err) {
        throw err;
    }

    decodeDouble(cursor, jstring, level, "size");
    decodeDouble(cursor, jstring, level, "width");
    decodeDouble(cursor, jstring, level, "angle");

    bytesHopper(cursor, 12);

    decodeDouble(cursor, jstring, level, "XOffset");
    decodeDouble(cursor, jstring, level, "YOffset");

    bytesHopper(cursor, 2);

    return 0;
}

int decodeMarkerTypes(char **cursor, std::string &jstring, int level) {
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

int decodeMaskTypes(char **cursor, std::string &jstring, int level) {
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


int decodeMarkerStyle(char **cursor, std::string &jstring, int level) {
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
