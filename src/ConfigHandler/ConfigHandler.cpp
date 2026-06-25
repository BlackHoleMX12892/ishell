#include "ConfigHandler.hpp"
#include <toml++/toml.hpp>
#include <vector>
#include <filesystem>
#include <iostream>

using namespace std::literals;

std::vector<std::vector<std::string>> ConfigHandler::env;

char file[] = "ishellconfig.toml";

void ConfigHandler::handleConfigFile() {
    std::filesystem::path configFile = file;
    if (std::filesystem::exists(configFile)) {
        auto configfile = toml::parse_file(file);

        auto* envtable = configfile["env"].as_table();

        if (envtable) {
            for (auto&& [key, value] : *envtable) {
                if (value.value<std::string>()) {
                    std::vector<std::string> temp = {std::string(key), std::string(value.value_or(""sv))};
                    env.push_back(temp);
                }
            }
        }
    } else {
        std::cout << "No config file has been detected, make sure you create a file named \"ishellconfig.toml\" under the working directory.\n";
    }
}
