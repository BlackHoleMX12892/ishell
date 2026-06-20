#include "EnvHandler.hpp"
#include "../ConfigHandler/ConfigHandler.hpp"

void EnvHandler::setenvfromconfig() {
    ConfigHandler config;
    config.handleConfigFile();
    for (size_t i = 0; i < config.env.size(); i++)
    {
        setenv(config.env.at(i).at(0).c_str(), const_cast<char*>(config.env.at(i).at(1).c_str()), 1);
    }
}
