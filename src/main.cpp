#include <iostream>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <csignal>
#include <rang.hpp>
#include <limits.h>
#include "EnvHandler/EnvHandler.hpp"
#include "CommandHandler/CommandHandler.hpp"
#include "ConfigHandler/ConfigHandler.hpp"
#include "RCHandler/RCHandler.hpp"
#include "CWDHandler/CWDHandler.hpp"

int main() {
    ConfigHandler config;
    config.handleConfigFile();
    EnvHandler::setenvfromconfig(config.env);
    RCHandler::rcfromconfig(config.rc);
    std::signal(SIGINT, SIG_IGN);
    std::string command;
    while(true) {
        std::cout << "ishell v0.2.0 " << rang::fg::green << CWDHandler::getFormattedPath() << rang::fg::reset << " > ";
        std::getline(std::cin, command);
        std::vector<std::string> splitcommand = CommandHandler::handleCommand(command);
        if (!splitcommand.empty()) {
            if (CommandHandler::checkIfInternal(splitcommand[0]) == true)
            {
                CommandHandler::executeInternalCommand(splitcommand);
            } else {
                CommandHandler::executeExternalCommand(splitcommand);
            }
        }
    }
}
