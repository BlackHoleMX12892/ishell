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
#include "LineEditor/LineEditor.hpp"
#include "HistoryHandler/HistoryHandler.hpp"

int main() {
    ConfigHandler config;
    config.handleConfigFile();
    EnvHandler::setEnvFromConfig(config.env);
    RCHandler::rcFromConfig(config.rc);
    while(true) {
        std::cout << "ishell v0.3.0 " << rang::fg::green << CWDHandler::getFormattedPath() << rang::fg::reset << " > " << std::flush;
        LineEditor lineeditor;
        std::string command = lineeditor.readLine();
        HistoryHandler historyhandler;
        historyhandler.handleHome();
        historyhandler.saveToFile(command);
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
