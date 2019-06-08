//
// Created by timfame on 08.06.19.
//

#include <cstring>
#include <iostream>
#include "file_archiver.h"
#include "file_dearchiver.h"

const std::string path = "";

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cout << "You have to pass 4 arguments (including name of program as first)\n";
        std::cout << "Second argument is type:\n";
        std::cout << "   zip  --  to zip file\n";
        std::cout << " unzip  --  to unzip file\n";
        std::cout << "   all  --  to zip and then unzip file\n";
        std::cout << "3rd and 4th arguments are input and output file names\n";
        return 0;
    }
    try {
        if (!strcmp(argv[1], "zip")) {
            file_archiver f(path + argv[2], path + argv[3]);
            f.archive();
        }
        if (!strcmp(argv[1], "unzip")) {
            file_dearchiver f(path + argv[2], path + argv[3]);
            f.dearchive();
        }
        if (!strcmp(argv[1], "all")) {
            {
                file_archiver f1(path + argv[2], path + argv[3]);
                f1.archive();
            }
            {
                file_dearchiver f2(path + argv[3], path + argv[2]);
                f2.dearchive();
            }
        }
        std::cout << "All done.\n";
    } catch (const std::runtime_error& re) {
        std::cout << "Error: " << re.what() << "\n";
        return 3;
    }
    return 0;
}