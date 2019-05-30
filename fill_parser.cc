//
//  fill_parser.cc
//
//  Created by Erick Li on 05/29/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#include "fill_parser.hh"
#include "parser.hh"

int parseFillPattern(char **cursor, std::string &jstring, int level) {
    int num_of_layers = 1;
    try {
        if (getChar(cursor) != 4) {
            num_of_layers = 1;
            bytesRewinder(cursor, 1);
        } else {
            // validate the header of the file.
            int s = hexValidation(cursor, "E6147992C8D011", DO_REWIND);
            if (s == 1) {
                LOG("ERROR: Header cannot be parsed.");
                throw std::string("Header.\n");
            } else {
                LOG("File is validated.");
            }
            // Move infile pointer 26 bytes to skip the header metadata.
            bytesHopper(cursor, 25);
            // Parse the first color pattern. Usage so far is unknown...
            parseColorPattern(cursor, jstring, "Unknown", 1);
            // Parse out the number of layers...
            num_of_layers = parseLayerNumber(cursor, jstring, 1);
            // Test if the number of layers behaves weird...
            if (-1 == num_of_layers) {
                LOG("ERROR: Number of Layers is abnormal.");
                throw std::string("Number of layers.\n");
            }
        }

        write_to_json(jstring, "fillLayer", "[", 1);

        // Start parsing each layer.
        for (int i = 0; i < num_of_layers; i++) {
            LOG("++++ START parsing layer NO. " + std::to_string(i + 1));
            write_to_json(jstring, "", "{", 2);
            write_to_json(jstring, "number", std::to_string(i + 1) + ",", 3);
            parseLayer(cursor, jstring, 0, 3);

            // Inter-layer pattern...
            if (i < num_of_layers - 1) {
                bytesRewinder(cursor, 1);
                int b = 0;
                do {
                    b = getChar(cursor);
                } while (b != 3 && b != 6 && b != 8 && b != 9);
                bytesRewinder(cursor, 1);
            }
            write_to_json(jstring, "", "},", 2);
        }

        write_to_json(jstring, "", "],", 1);
    } catch (std::string err) {
        throw err;
    }

    int stnl = get64Bit(cursor);
    if (0x0D != stnl) {
        LOG("ERROR: sentinel");
        throw std::string("0x0D sentinel");
    }

    write_to_json(jstring, "fillLayerActiveness", "{", 1);
    LOG("Checking fill layer activeness...");

    for (int i = 0; i < num_of_layers; i++) {
        int activeness = get32Bit(cursor);
        LOG("Fill layer " + std::to_string(i + 1) + ": " + std::to_string(activeness));
        write_to_json(jstring, "layer" + std::to_string(i + 1), std::to_string(activeness) + ",", 2);
    }

    write_to_json(jstring, "", "},", 1);

    return 0;
}

int parseLayer(char **cursor, std::string &jstring, int type, int level) {
    // Type 0: A normal layer
    if (type == 0) {
        LOG("START parsing a fill layer...");
    // A Symbol
    } else {
        LOG("START parsing a symbol...");
    }

    // Get the filling type (3 for simple fill; 6 for line fill; 8 for marker fill)
    int filling_type = get16Bit(cursor);
    // name of the corresponding filling type.
    std::string filling_type_name = "";

    // For each filling type, enter into the corresponding field.
    if (0xE603 == filling_type) {
        parseSimpleFill(cursor, jstring, type, level);
    } else if (0xE606 == filling_type) {
        parseLineFill(cursor, jstring, level);
    } else if (0xE608 == filling_type) {
        parseMarkerFill(cursor, jstring, level);
    } else if (0xE609 == filling_type) {
        LOG("ERROR: Gradient Fill is currently not supported");
        throw std::string("Currently unsupported filling type.");
    } else {
        LOG("ERROR: Filling type " + std::to_string(filling_type) + " not supported");
        throw std::string("Filling type.");
    }

    return 0;
}


int parseSimpleFill(char **cursor, std::string &jstring, int type, int level) {
    LOG("Filling type: Simple Fill");

    write_to_json(jstring, "fillingType", "\"Simple Fill\",", level);

    try {
        // Validate if the header is there.
        if (0 != hexValidation(cursor, "147992C8D0118BB6080009EE4E41", !DO_REWIND)) {
            LOG("ERROR: Fail to validate Simple Fill pattern header...");
            throw std::string("Validation.");
        }
        bytesHopper(cursor, 2);

        // Type of the process (0 for normal process; 1 for the symbol process)
        if (type == 0) {
            parseLinePattern(cursor, jstring, 0, "Outline", level);
        } else {
            parseLinePattern(cursor, jstring, 1, "Filling Line", level);
        }

        parseColorPattern(cursor, jstring, "Filling Color", level);
    } catch (std::string err) {
        throw err;
    }

    return 0;
}


int parseLineFill(char **cursor, std::string &jstring, int level) {
    LOG("Filling type: Line Fill");

    write_to_json(jstring, "fillingType", "\"Line Fill\",", level);

    try {
        // Validate if the header is there.
        if (0 != hexValidation(cursor, "147992C8D0118BB6080009EE4E41", !DO_REWIND)) {
            LOG("ERROR: Fail to validate Line Fill pattern header...");
            throw std::string("Validation.");
        }
        bytesHopper(cursor, 18);

        // Parse the line pattern for the filling lines.
        parseLinePattern(cursor, jstring, 0, "Filling Line", level);
        // parse the line pattern for the outline.
        parseLinePattern(cursor, jstring, 0, "Outline", level);
    } catch (std::string err) {
        throw err;
    }

    // Parse the line fill angle. (the angle of the line inclination)
    parseDouble(cursor, jstring, "angle", level);
    // Parse the line fill offset. (the horizontal displacement of the lines)
    parseDouble(cursor, jstring, "offset", level);
    // Parse the line fill separation (the distance between each line)
    parseDouble(cursor, jstring, "separation", level);

    return 0;
}

int parseMarkerFill(char **cursor, std::string &jstring, int level) {
    LOG("Filling type: Marker Fill");

    write_to_json(jstring, "fillingType", "\"Marker Fill\",", level);

    try {
        // Validate if the header is there.
        if (0 != hexValidation(cursor, "147992C8D0118BB6080009EE4E41", !DO_REWIND)) {
            LOG("ERROR: Fail to validate Marker Fill pattern header...");
            throw std::string("validation");
        }
        bytesHopper(cursor, 2);

        // Parse the marker types (Grid(uniform) or random distribution)
        parseMarkerTypes(cursor, jstring, level);
        // Parse the X and Y-axial offset (horizontal displacement of the markers)
        parseDouble(cursor, jstring, "fillPropertiesOffsetX", level);
        parseDouble(cursor, jstring, "fillPropertiesOffsetY", level);
        // Parse the X and Y-axial separation of the markers
        parseDouble(cursor, jstring, "fillPropertiesSeparationX", level);
        parseDouble(cursor, jstring, "fillPropertiesSeparationY", level);
        bytesHopper(cursor, 16);

        // Parse the Marker in detail
        parseMarkerPattern(cursor, jstring, level);

        // Parse the outline.
        parseLinePattern(cursor, jstring, 0, "Outline", level);
    } catch (std::string err) {
        throw err;
    }

    return 0;
}
