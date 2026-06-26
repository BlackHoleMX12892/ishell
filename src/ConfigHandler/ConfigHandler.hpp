#pragma once

#include <vector>
#include <string>

namespace ConfigHandler {
    inline std::vector<std::vector<std::string>> env;
    inline std::vector<std::string> rc;
    void handleConfigFile();
};
