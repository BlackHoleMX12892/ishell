#include "CWDHandler.hpp"
#include <string>
#include <limits.h>
#include <unistd.h>

std::string CWDHandler::getPath() {
    char buffer[PATH_MAX];
    getcwd(buffer, sizeof(buffer));

    std::string output = static_cast<std::string>(buffer);

    if (output == getenv("HOME")) {
        output = "~";
    }

    return output;
}
