//
//  json_writer.hh
//
//  Created by Erick Li on 04/11/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#ifndef __JSON_WRITER__
#define __JSON_WRITER__

#include <fstream>
#include <string>
#include <iostream>

const char SPACE = ' ';
const char NEWLINE = '\n';
const char COMMA = ',';

/**
 * NOTE: This is a note explaining some project-wide params
 * \param cursor: this is a double pointer pointing to the current manipulating
 *   position to the data block.
 *   Q: Why a double pointer is needed for the cursor?
 *   A: Because by using double pointer, the program can track the cursor
 *     movement within the function even though we do not return the moved
 *     cursor.
 * \param jstring: this is a std::string in JSON format. It is for formatting
 *   the information from the data block, and then eventually return to the
 *   user for further manipulation. jstring will be written top-down following
 *   the order of the style symbol file.
 * \param level: this represents the indentation of the next jstring key-Value
 *   pair
 *   Example: if we state that the level is 3, the next jstring key-value pair
 *     will be indented with three tabs.
 */

/**
 * Write a string into JSON file given tag name, and level(indention).
 * \param (see NOTE)
 * \param the key, or empty string if there is no key. (For example, "}")
 * \param the value of the key. ("key": "value")
 * \param (see NOTE)
 */
void write_to_json(std::string &jstring, std::string key, std::string value, int level);

/**
 * Remove additional commas from the jstring
 * \param (see NOTE)
 * \return jstring after removing additional commas
 */
std::string json_comma_remover(std::string &jstring);

#endif
