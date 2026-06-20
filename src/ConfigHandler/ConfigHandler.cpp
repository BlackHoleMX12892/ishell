#include "ConfigHandler.hpp"
#include <toml++/toml.hpp>
#include <vector>

using namespace std::literals;

std::vector<std::vector<std::string>> ConfigHandler::env;


void ConfigHandler::handleConfigFile() {
    auto configfile = toml::parse_file("ishellconfig.toml");

    auto* envtable = configfile["env"].as_table();

    if (envtable)
    {
        for (auto&& [key, value] : *envtable)
        {
            if (value.value<std::string>())
            {
                std::vector<std::string> temp = {std::string(key), std::string(value.value_or(""sv))};
                env.push_back(temp);
            }
        }
    }
}
