#ifndef __VERSION_SEEKER__
#define __VERSION_SEEKER__

#include <cstring>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>

/**
 * Find Southgis iData version number from a given file.
 * \param the path to the wanted file.
 * \return iData version in std::string form, or when an error occurs,
 *         return error message.
 */
std::string find_version(std::string path_to_file);

/**
 * A helper function that handles errors.
 * \param the error message.
 * \return the error message with "ERROR" flag in the front.
 */
std::string _error_handler(std::string err_msg);

#endif
