#include "CWDHandler.hpp"
#include <string>
#include <limits.h>
#include <unistd.h>
#include <sstream>

std::string CWDHandler::getFormattedPath() { // will change to getFormattedPath() - done
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) == NULL) {
        return "\x1b[31mError getting directory";
    }

    std::string output = static_cast<std::string>(buffer);

    /*if (output == getenv("HOME")) { // this will handle HOME replacing to ~ even when outside the HOME directory - done
        output = "~";
    }*/

    char *home = getenv("HOME");
    std::string homestr = static_cast<std::string>(home);
    std::stringstream outputstream;

    int matches = 0;

    for (size_t i = 0; i < homestr.size(); i++) {
        if (output[i] == homestr[i]) {
            matches++;
        }
    }


    if (matches == homestr.size()) {
        for (size_t i = 0; i < output.size() - homestr.size() + 1; i++) {
            if (i == 0) {
                outputstream << '~';
                continue;
            }
            outputstream << output[(homestr.size() - 1) + (i)];
        }
        output = outputstream.str();
    }

    return output;
}
