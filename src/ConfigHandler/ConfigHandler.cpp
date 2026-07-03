#include "ConfigHandler.hpp"
#include <toml++/toml.hpp>
#include <vector>
#include <filesystem>
#include <iostream>
#include <rang.hpp>
#include <unistd.h>

using namespace std::literals;

int ConfigHandler::maxcommands() {
    std::filesystem::path configFile = file.c_str();
    int tempmaxcommands {50};
    if (std::filesystem::exists(configFile)) {
        // bad variable name - already used it before but with a capital f
        auto configfile = toml::parse_file(file.c_str());

        auto* historytable = configfile["history"].as_table();

        if (historytable)
        {
            for (auto&& [key, value] : *historytable)
            {
                if (key == "max-commands" && value.value<int>())
                {
                    tempmaxcommands = value.value_or(50);
                }
            }
        }
    } else {
        std::cout << rang::fg::yellow << "No config file has been detected, make sure you create a file named \"ishellconfig.toml\" under the home or working directory.\n" << rang::fg::reset;
    }

    return tempmaxcommands;
}

std::vector<std::string> ConfigHandler::rc() {
    std::vector<std::string> temprc;
    std::filesystem::path configFile = file.c_str();
    if (std::filesystem::exists(configFile)) {
        // bad variable name - already used it before but with a capital f
        auto configfile = toml::parse_file(file.c_str());

        auto* rctable = configfile["rc"].as_table();

        if (rctable) {
            for (auto&& [key, value] : *rctable) {
                if (value.value<std::string>())
                {
                    temprc.push_back(std::string(value.value_or(""sv)));
                }
            }
        }
    } else {
        std::cout << rang::fg::yellow << "No config file has been detected, make sure you create a file named \"ishellconfig.toml\" under the home or working directory.\n" << rang::fg::reset;
    }

    return temprc;
}

std::vector<std::vector<std::string>> ConfigHandler::env() {
    std::filesystem::path configFile = file.c_str();
    std::vector<std::vector<std::string>> tempenv;
    if (std::filesystem::exists(configFile)) {
        // bad variable name - already used it before but with a capital f
        auto configfile = toml::parse_file(file.c_str());

        auto* envtable = configfile["env"].as_table();

        if (envtable) {
            for (auto&& [key, value] : *envtable) {
                if (value.value<std::string>()) {
                    std::vector<std::string> temp = {std::string(key), std::string(value.value_or(""sv))};
                    tempenv.push_back(temp);
                }
            }
        }
    } else {
        std::cout << rang::fg::yellow << "No config file has been detected, make sure you create a file named \"ishellconfig.toml\" under the home or working directory.\n" << rang::fg::reset;
    }

    return tempenv;
}

void ConfigHandler::handleConfigFile() {
    char* home = getenv("HOME");
    if (home != nullptr) {
        file = static_cast<std::string>(home) + "/" + file;
    }
}
