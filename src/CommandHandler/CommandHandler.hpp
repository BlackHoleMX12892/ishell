#pragma once
#include <vector>
#include <string>

namespace CommandHandler {
    std::vector<std::string> handleCommand(std::string command);
    void executeExternalCommand(std::vector<std::string> splitcommand);
}
