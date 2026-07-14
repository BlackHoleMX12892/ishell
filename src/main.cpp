#include <iostream>
#include <rang.hpp>
#include "EnvHandler/EnvHandler.hpp"
#include "CommandHandler/CommandHandler.hpp"
#include "ConfigHandler/ConfigHandler.hpp"
#include "RCHandler/RCHandler.hpp"
#include "CWDHandler/CWDHandler.hpp"
#include "LineEditor/LineEditor.hpp"
#include "HistoryHandler/HistoryHandler.hpp"

int main() {
    ConfigHandler config;
    ConfigHandler::handleConfigFile();
    HistoryHandler historyhandler;
    HistoryHandler::handleHome();
    EnvHandler::setEnvFromConfig();
    RCHandler::rcFromConfig();
    LineEditor lineeditor;
    CommandHandler commandhandler;
    while(true) {
        std::cout << "ishell v0.3.2 " << rang::fg::green << CWDHandler::getFormattedPath() << rang::fg::reset << " > " << std::flush;
        std::string command = lineeditor.readLine();
        historyhandler.saveToFile(command);
        commandhandler.executeCommand(command);
    }
}
