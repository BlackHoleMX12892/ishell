#pragma once

#include <vector>
#include <string>

// I need to make this a namespace instead, terrible decision making it a struct.
// It makes no sense to create objects of this.
// It might actually make sense as the file could be handled multiple times, but it really doesn't unless each value is retrieved by an independent function.
class ConfigHandler {
private:
    std::string file = ".ishellconfig.toml";
public:
    std::vector<std::vector<std::string>> env();
    std::vector<std::string> rc();
    int maxcommands();
    void handleConfigFile();
};
