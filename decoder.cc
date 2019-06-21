//
//  decoder.cc
//
//  Created by Erick Li on 04/11/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#include "decoder.hh"

std::string grandDecoder(char *head, char *tail, bool enableLogging) {
    // Initialize a jstring (for JSON string)
    std::string jstring = "";
    char **cursor = &head;

    // Underlying buffer.
    std::streambuf* orig_buf;

    // If logging is disabled, then we need to close clog for logging.
    if (!enableLogging) {
        // Get underlying buffer
        orig_buf = std::clog.rdbuf();
        // Set NULL
        std::clog.rdbuf(NULL);
    }

    // bytesHopper(cursor, num_of_bytes - 1);
    // **cursor = 0x99; // Indicate the end of the block
    // bytesRewinder(cursor, num_of_bytes - 1);

    write_to_json(jstring, "", "{", 0);

    try {
        int grand_stnl = get16Bit(cursor);
        if (0xE603 == grand_stnl || (0xE606 <= grand_stnl && grand_stnl <= 0xE609)) {
            grand_stnl = 0x0000; // this is a special code for fill symbol without header...
        }
        bytesRewinder(cursor, 2);

        switch (grand_stnl) {
            case 0xE604:
                // Fill Symbol
                write_to_json(jstring, "symbolType", "\"fill\",", 1);
                LOG("START decoding fill symbol...");
                decodeFillPattern(cursor, jstring, 1, &tail);
                break;
            case 0x0000:
                // Fill Symbol without header
                write_to_json(jstring, "symbolType", "\"fill\",", 1);
                LOG("START decoding fill symbol...");
                decodeFillPattern(cursor, jstring, 1, &tail);
                break;
            case 0xE5FA:
                // line symbol
                write_to_json(jstring, "symbolType", "\"line\",", 1);
                LOG("START decoding line symbol...");
                decodeLinePattern(cursor, jstring, 1, "", 1);
                break;
            case 0xE5FF:
                // marker symbol
                write_to_json(jstring, "symbolType", "\"marker\",", 1);
                LOG("START decoding marker symbol...");
                decodeMarkerPattern(cursor, jstring, 1);
                break;
            default:
                LOG("ERROR: Cannot recognize header...");
                throw std::string("Symbol header.");
        }
    } catch (std::string err) {
        LOG("ERROR occurred. Stopped...");
        return std::string("\"error\": \"" + err + "\"\n");
    }

    write_to_json(jstring, "", "}", 0);
    LOG("FINISHED decoding :-)");

    // However, we still want the results being output, don't we?
    if (!enableLogging) {
        std::clog.rdbuf(orig_buf);
    }

    return json_comma_remover(jstring);
}


void decodeColorPattern(char **cursor, std::string &jstring, std::string color_type, int level) {
    LOG("START decoding color...");

    // Get the color space (0x92 for HSV; 0x96 for RGB; 0x97 for CMYK)
    int color_space = get16Bit(cursor);

    write_to_json(jstring, "color_" + toSimpleCamelCase(color_type), "{", level);
    write_to_json(jstring, "name", "\"" + color_type + "\",", level + 1);

    // CMYK color space...
    if (0xC497 == color_space) {
        LOG("Color Space: CMYK.");

        // Skip the metadata of the color space.
        bytesHopper(cursor, 18);
        int c = getChar(cursor);
        int m = getChar(cursor);
        int y = getChar(cursor);
        int k = getChar(cursor);

        LOG("C: " + std::to_string(c));
        LOG("M: " + std::to_string(m));
        LOG("Y: " + std::to_string(y));
        LOG("K: " + std::to_string(k));

        write_to_json(jstring, "space", "\"CMYK\",", level + 1);
        write_to_json(jstring, "colorCode", "[", level + 1);
        write_to_json(jstring, "", std::to_string(c) + ",", level + 2);
        write_to_json(jstring, "", std::to_string(m) + ",", level + 2);
        write_to_json(jstring, "", std::to_string(y) + ",", level + 2);
        write_to_json(jstring, "", std::to_string(k) + ",", level + 2);
        write_to_json(jstring, "", "],", level + 1);
    } else {
        std::list<double> mycolor;
        bytesHopper(cursor, 19);

        if (0xC492 == color_space) {
            write_to_json(jstring, "space", "\"HSV\",", level + 1);
        } else if (0xC496 == color_space) {
            write_to_json(jstring, "space", "\"RGB\",", level + 1);
        }

        // There are three fields for both HSV and RGB.
        // Type and the definition are unknown so far.
        // HSV and RGB share the same coding philsophy.
        write_to_json(jstring, "rawColorCode", "[", level + 1);
        double first = decodeDouble(cursor, jstring, "", level + 2);
        double second = decodeDouble(cursor, jstring, "", level + 2);
        double third = decodeDouble(cursor, jstring, "", level + 2);
        write_to_json(jstring, "", "],", level + 1);

        // HSV color space...
        if (0xC492 == color_space) {
            LOG("Color Space: HSV.");
            mycolor = CIELAB_to_RGB_HSV(first, second, third, USE_HSV);
        // RGB color space...
        } else if (0xC496 == color_space) {
            LOG("Color Space: RGB.");
            mycolor = CIELAB_to_RGB_HSV(first, second, third, USE_RGB);
        } else {
            // If the color space code is not 92, 96, or 97, then an error mesg will be printed out.
            LOG("ERROR: Color Space " + std::to_string(color_space) + " not found.");
            throw std::string("Color Space.");
        }

        double fs = mycolor.front();
        mycolor.pop_front();
        double sd = mycolor.front();
        mycolor.pop_front();
        double td = mycolor.front();
        mycolor.pop_front();

        LOG("Converted color");
        LOG("1: " + std::to_string(fs));
        LOG("2: " + std::to_string(sd));
        LOG("3: " + std::to_string(td));

        write_to_json(jstring, "colorCode", "[", level + 1);
        write_to_json(jstring, "", std::to_string(fs) + ",", level + 2);
        write_to_json(jstring, "", std::to_string(sd) + ",", level + 2);
        write_to_json(jstring, "", std::to_string(td) + ",", level + 2);
        write_to_json(jstring, "", "],", level + 1);
    }

    write_to_json(jstring, "", "},", level);
    bytesHopper(cursor, 2);
    LOG("FINISHED decoding color :-)");
}

int decodeLayerNumber(char **cursor, std::string &jstring, int level) {
    LOG("START decoding layer number...");

    // Number of layers.
    int num_of_layers = getChar(cursor);

    if (0 >= num_of_layers) {
        LOG("ERROR: Number of layers is negative");
        throw std::string("number of layers.");
    }
    LOG("The number of layer is: " + std::to_string(num_of_layers));

    // Check whether the layer number is normal...
    if (10 < num_of_layers) {
        LOG("WARNING: Layer number is abnormal.");
    }

    write_to_json(jstring, "numberOfLayers", std::to_string(num_of_layers) + ",", level);
    bytesHopper(cursor, 3);

    return num_of_layers;
}

double decodeDouble(char **cursor, std::string &jstring, std::string tag, int level) {
    double val = getDouble(cursor);

    // LOG("this");
    // std::cout << std::setprecision(10) << val << std::endl;
    std::stringstream ss;
    ss << std::setprecision(50) << val;
    std::string dstr;
    ss >> dstr;

    // Put the name and the value of the double value into the JSON string.
    LOG(tag + " is: " + std::to_string(val));
    write_to_json(jstring, tag, dstr + ",", level);

    return val;
}

int decodeInt(char **cursor, std::string &jstring, std::string tag, int level) {
    int val = get32Bit(cursor);

    // Put the name and value of the integer value into the JSON string.
    LOG(tag + " is: " + std::to_string(val));
    write_to_json(jstring, tag, std::to_string(val) + ",", level);

    return val;
}

int decodeString(char **cursor, std::string &jstring, std::string tag, int level) {
    LOG("START decoding string...");

    // bool going = true; // When "going" is true, the while loop will keep going.
    // std::string str = "";
    int code = 1;
    write_to_json(jstring, "fontName", "[", level);

    while (0 != (code = get16Bit(cursor))) {
        write_to_json(jstring, "", std::to_string(code) + ",", level + 1);
    }

    write_to_json(jstring, "", "]", level);

    // The length of the null pattern can be varied.
    int id = getChar(cursor);
    if (id == 0) {
        bytesHopper(cursor, 54);
    } else if (id == 3) {
        bytesHopper(cursor, 24);
    } else {
        LOG("ERROR: Failed to validate string format...");
        throw std::string("validation.");
    }

    // printHex(cursor, 30);

    int secondCode = getChar(cursor);
    while (1 != secondCode && 0 != secondCode) {
        secondCode = getChar(cursor);
    }

    bytesRewinder(cursor, 1);

    LOG("FINISHED decoding string. :-)");

    return 0;
}
