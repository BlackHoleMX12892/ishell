#include "RCHandler.hpp"
#include "../CommandHandler/CommandHandler.hpp"

void RCHandler::rcFromConfig(std::vector<std::string> rc) {
    for (size_t i = 0; i < rc.size(); i++) {
        CommandHandler commandhandler;
        commandhandler.executeCommand(rc[i]);
    }
}
