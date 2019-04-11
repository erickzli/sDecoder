#ifndef __JSON_WRITER__
#define __JSON_WRITER__

#include <fstream>
#include <string>
#include <iostream>

void write_to_json(std::ofstream &jsonfile, std::string tag, std::string buf,
                   int level, int type);


#endif
