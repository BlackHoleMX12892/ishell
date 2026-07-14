#include "RCHandler.hpp"
#include "../CommandHandler/CommandHandler.hpp"
#include "../ConfigHandler/ConfigHandler.hpp"

void RCHandler::rcFromConfig() {
    ConfigHandler confighandler;
    std::vector<std::string> rc = confighandler.rc();

    for (size_t i = 0; i < rc.size(); i++) {
        CommandHandler commandhandler;
        commandhandler.executeCommand(rc[i]);
    }
}
