//
//  fill_parser.cc
//
//  Created by Erick Li on 05/29/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#include "fill_parser.hh"
#include "parser.hh"


int parseSimpleFill(char **cursor, std::string &jstring, int type, int level) {
    LOG("---------------------------");
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
    LOG("---------------------------");
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
    LOG("---------------------------");
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
