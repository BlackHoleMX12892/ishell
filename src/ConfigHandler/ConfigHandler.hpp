#pragma once

#include <vector>
#include <string>

class ConfigHandler {
private:
    
public:
    static std::vector<std::vector<std::string>> env;
    void handleConfigFile();
};
