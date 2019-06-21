//
//  version_seeker.cpp
//
//  Created by Erick Li on 06/18/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#include "version_seeker.h"

std::string find_version(std::string path_to_file) {
    struct stat mystat;
    const char *filepath = path_to_file.c_str();
    int myfd = open(filepath, O_RDONLY);
    size_t filesize = 0;
    char *addr;

    // Cannot open the file
    if (-1 == myfd) {
        return _error_handler("open()");
    }

    // Cannot retrieve file stat
    if (-1 == fstat(myfd, &mystat)) {
        return _error_handler("fstat()");
    }

    // Get the file size
    filesize = mystat.st_size;

    // Map the file on memory
    addr = (char *)mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, myfd, 0);

    const char *version_header = "SOUTHGIS_IDATA_VERSION";

    // Get the length of version header
    //  Example: header: "SOUTHGIS_IDATA_VERSION" -> header_size: 22
    size_t header_size = strlen(version_header);

    // Locate the header in the mapped file, get a pointer pointing to the front of the header
	char *found = std::search(addr + filesize / 2, addr + filesize, version_header, version_header + header_size);
    if (found == addr + filesize) {
        found = std::search(addr, addr + filesize / 2, version_header, version_header + header_size);
        if (found == addr + filesize / 2) {
            std::clog << "WARNING: No version info" << std::endl;
            return "WARNING: No version info.";
        }
    }
    // Move pointer to the version number.
    found += header_size + 1;

    // Read version number
    std::string version_num_str = "";
    while (0 != *found) {
        version_num_str += *found;
        found++;
    }

    munmap(addr, filesize);
    close(myfd);

    return version_num_str;
}

std::string _error_handler(std::string err_msg) {
    std::clog << err_msg << std::endl;
    return "ERROR:" + err_msg;
}
