#include "CWDHandler.hpp"
#include <string>
#include <limits.h>
#include <unistd.h>

std::string CWDHandler::getPath() {
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) == NULL) {
        return "\x1b[31mError getting directory";
    }

    std::string output = static_cast<std::string>(buffer);

    if (output == getenv("HOME")) {
        output = "~";
    }

    return output;
}
