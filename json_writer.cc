//
//  json_writer.cc
//
//  Created by Erick Li on 04/11/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#include "json_writer.hh"



void write_to_json(std::string &jstring, std::string key, std::string value, int level) {
    std::string output = "";

    // Insert indention for the line.
    for (int i = 0; i < level; i++) {
        output += "    "; // Use 4 spaces instead of a tab
    }

    // If it has a tag -- `"tag": `
    if (0 != tag.length()) {
        output += ("\"" + tag + "\": ");
    }

    output +=  buf + "\n";

    jstring += output;
}

std::string json_comma_remover(std::string &jstring) {
    size_t jlen = jstring.length();
    std::string ret = "";
    int count_spaces = 0;
    // int count_line = 1;

    for (size_t i = 0; i < jlen; i++) {
        if (COMMA == jstring[i] && NEWLINE == jstring[i + 1]) {
            // std::clog << "Line " << count_line++ << ": " << count_spaces << std::endl;
            int temp = i + 2;
            int next_space = 0;
            while (SPACE == jstring[temp]) {
                next_space += 1;
                temp += 1;
            }
            if (next_space == count_spaces) {
                ret += COMMA;
            }
            count_spaces = next_space;
        } else if (NEWLINE == jstring[i]) {
            // std::clog << "Line " << count_line++ << ": " << count_spaces << std::endl;
            int temp = i + 1;
            int next_space = 0;
            while (SPACE == jstring[temp]) {
                next_space += 1;
                temp += 1;
            }
            ret += NEWLINE;
            count_spaces = next_space;
        } else {
            ret += jstring[i];
        }
    }

    return ret;
}
