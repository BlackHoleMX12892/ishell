#include "ConfigHandler.hpp"
#include <toml++/toml.hpp>
#include <vector>
#include <filesystem>

using namespace std::literals;

std::vector<std::vector<std::string>> ConfigHandler::env;
std::filesystem::path configFile = "ishellconfig.toml";


void ConfigHandler::handleConfigFile() {
    if (std::filesystem::exists(configFile)) {
        auto configfile = toml::parse_file("ishellconfig.toml");

        auto* envtable = configfile["env"].as_table();

        if (envtable) {
            for (auto&& [key, value] : *envtable) {
                if (value.value<std::string>()) {
                    std::vector<std::string> temp = {std::string(key), std::string(value.value_or(""sv))};
                    env.push_back(temp);
                }
            }
        }
    }
}
