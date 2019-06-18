#include <cstring>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

double error_handler(std::string err_msg) {
    std::clog << err_msg << std::endl;
    return -1;
}

int main() {
    struct stat mystat;
    std::string path_to_file = "Files/tests/a.irx";
    const char *filepath = path_to_file.c_str();
    int myfd = open(filepath, O_RDONLY);
    size_t filesize = 0;
    char *addr;

    // Cannot open the file
    if (-1 == myfd) {
        return error_handler("open()");
    }

    // Cannot retrieve file stat
    if (-1 == fstat(myfd, &mystat)) {
        return error_handler("fstat()");
    }

    // Get the file size
    filesize = mystat.st_size;

    // Map the file on memory
    addr = (char *)malloc(filesize);
    char *fixed = addr;
    addr = (char *)mmap(addr, filesize, PROT_READ, MAP_PRIVATE, myfd, 0);

    std::cout << *addr << std::endl;

    // return 0;

    int lower = 0;
    int upper = filesize / 2;
    int upper2 = 23;
    srand(time(NULL));
    for (size_t i = 0; i < 100; i++) {
        // addr = fixed;
        int num1 = (rand() % (upper - lower + 1)) + lower;
        char *messup = fixed + num1;
        int num2 = (rand() % (upper2 - lower + 1)) + lower;
        memset(messup, num2, 1);
        // *messup = '3';
        FILE *fp;
        std::string nfile = "Files/tests/test" + std::to_string(i);
        fp = fopen(nfile.c_str(), "wb");
        std::cout << *addr << std::endl;
        int count = fwrite(addr, 1, filesize, fp);
        // std::cout << count << std::endl;
        fclose(fp);
    }

    free(fixed);
    munmap(fixed, filesize);
    close(myfd);

    return 0;
}
