#include "RCHandler.hpp"
#include "../ConfigHandler/ConfigHandler.hpp"
#include "../CommandHandler/CommandHandler.hpp"

void RCHandler::rcfromconfig(std::vector<std::string> rc) {
    for (size_t i = 0; i < rc.size(); i++) {
        std::vector<std::string> splitcommand = CommandHandler::handleCommand(rc[i]);
        CommandHandler::executeExternalCommand(splitcommand);
    }
}
