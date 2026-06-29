#include "EnvHandler.hpp"
#include "../ConfigHandler/ConfigHandler.hpp"

void EnvHandler::setEnvFromConfig(std::vector<std::vector<std::string>> env) {
    for (size_t i = 0; i < env.size(); i++) {
        setenv(env.at(i).at(0).c_str(), const_cast<char*>(env.at(i).at(1).c_str()), 1);
    }
}
