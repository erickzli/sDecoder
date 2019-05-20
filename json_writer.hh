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


/**
 * Write a string into JSON file given tag name, and level(indention).
 * \param the output file stream.
 * \param the tag, or empty string if no tags is needed.
 * \param the value after the tag. ("tag": "value")
 * \param the level(indention)
 */
 void write_to_json(std::string &jstring, std::string tag, std::string buf, int level);


#endif
