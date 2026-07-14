#include "ConfigHandler.hpp"
#include <toml++/toml.hpp>
#include <vector>
#include <filesystem>
#include <iostream>
#include <rang.hpp>
#include <unistd.h>

using namespace std::literals::string_literals;

int ConfigHandler::maxcommands() {
    int tempmaxcommands {50};
    auto* historytable = parsedconfigfile["history"].as_table();

    if (historytable) {
        for (auto&& [key, value] : *historytable)
        {
            if (key == "max-commands" && value.value<int>()) {
                tempmaxcommands = value.value_or(50);
            }
        }
    }

    return tempmaxcommands;
}

std::vector<std::string> ConfigHandler::rc() {
    std::vector<std::string> temprc;
    auto* rctable = parsedconfigfile["rc"].as_table();

    if (rctable) {
        for (auto&& [key, value] : *rctable) {
            if (value.value<std::string>())
            {
                temprc.push_back(value.value_or(""s));
            }
        }
    }

    return temprc;
}

std::vector<std::vector<std::string>> ConfigHandler::env() {
    std::vector<std::vector<std::string>> tempenv;
    auto* envtable = parsedconfigfile["env"].as_table();

    if (envtable) {
        for (auto&& [key, value] : *envtable) {
            if (value.value<std::string>()) {
                std::vector<std::string> temp = {std::string(key), value.value_or(""s)};
                tempenv.push_back(temp);
            }
        }
    }

    return tempenv;
}

void ConfigHandler::handleConfigFile() {
    char* home = getenv("HOME");
    if (home != nullptr) {
        file = static_cast<std::string>(home) + "/" + file;
    }

    std::filesystem::path configFile = file.c_str();
    if (std::filesystem::exists(configFile)) {
        parsedconfigfile = toml::parse_file(file.c_str());
    } else {
        std::cout << rang::fg::yellow << "No config file has been detected, make sure you create a file named \"ishellconfig.toml\" under the home or working directory.\n" << rang::fg::reset;
    }
}
