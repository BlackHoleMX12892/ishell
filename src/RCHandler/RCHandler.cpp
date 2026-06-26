#include "RCHandler.hpp"
#include "../ConfigHandler/ConfigHandler.hpp"
#include "../CommandHandler/CommandHandler.hpp"

void RCHandler::rcfromconfig() {
    for (size_t i = 0; i < ConfigHandler::rc.size(); i++) {
        std::vector<std::string> splitcommand = CommandHandler::handleCommand(ConfigHandler::rc[i]);
        CommandHandler::executeExternalCommand(splitcommand);
    }
}
