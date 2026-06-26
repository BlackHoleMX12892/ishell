#include "EnvHandler.hpp"
#include "../ConfigHandler/ConfigHandler.hpp"

void EnvHandler::setenvfromconfig() {
    for (size_t i = 0; i < ConfigHandler::env.size(); i++) {
        setenv(ConfigHandler::env.at(i).at(0).c_str(), const_cast<char*>(ConfigHandler::env.at(i).at(1).c_str()), 1);
    }
}
