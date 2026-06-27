#include "CWDHandler.hpp"
#include <string>
#include <limits.h>
#include <unistd.h>

std::string CWDHandler::getFormattedPath() { // will change to getFormattedPath() - done
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) == NULL) {
        return "\x1b[31mError getting directory";
    }

    std::string output = static_cast<std::string>(buffer);

    if (output == getenv("HOME")) { // this will handle HOME replacing to ~ even when outside the HOME directory
        output = "~";
    }

    return output;
}
