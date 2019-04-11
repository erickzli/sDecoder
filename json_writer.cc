#include "json_writer.hh"

void write_to_json(std::ofstream &jsonfile, std::string tag, std::string buf, int level) {
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

    // flush output to json.
    jsonfile << output;
}
