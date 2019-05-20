//
//  json_writer.cc
//
//  Created by Erick Li on 04/11/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#include "json_writer.hh"

void write_to_json(std::string &jstring, std::string tag, std::string buf, int level) {
    std::string output = "";

    // Insert indention for the line.
    for (int i = 0; i < level; i++) {
        output += "\t";
    }

    // If it has a tag -- `"tag": `
    if (0 != tag.length()) {
        output += ("\"" + tag + "\": ");
    }

    output +=  buf + "\n";

    jstring += output;
}
