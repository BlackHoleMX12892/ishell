#pragma once

#include <vector>
#include <string>

struct ConfigHandler {
    std::vector<std::vector<std::string>> env;
    std::vector<std::string> rc;
    void handleConfigFile();
};
