#include "ConfigHandler.hpp"
#include <toml++/toml.hpp>
#include <vector>
#include <filesystem>
#include <iostream>
#include <rang.hpp>
#include <unistd.h>

using namespace std::literals;

void ConfigHandler::handleConfigFile() {
    std::string file = "ishellconfig.toml";
    char* home = getenv("HOME");
    if (home != nullptr) {
        file = static_cast<std::string>(home) + "/ishellconfig.toml";
    }
    std::filesystem::path configFile = file.c_str();
    if (std::filesystem::exists(configFile)) {
        auto configfile = toml::parse_file(file.c_str());

        auto* envtable = configfile["env"].as_table();

        if (envtable) {
            for (auto&& [key, value] : *envtable) {
                if (value.value<std::string>()) {
                    std::vector<std::string> temp = {std::string(key), std::string(value.value_or(""sv))};
                    env.push_back(temp);
                }
            }
        }

        auto* rctable = configfile["rc"].as_table();

        if (rctable) {
            for (auto&& [key, value] : *rctable) {
                if (value.value<std::string>())
                {
                    rc.push_back(std::string(value.value_or(""sv)));
                }
            }
        }
    } else {
        std::cout << rang::fg::yellow << "No config file has been detected, make sure you create a file named \"ishellconfig.toml\" under the home or working directory.\n" << rang::fg::reset;
    }
}
