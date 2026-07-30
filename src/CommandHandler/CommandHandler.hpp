#pragma once
#include <vector>
#include <string>
#include "Parser.hpp"

class CommandHandler {
private:
    std::vector<std::string> handleCommand(std::string command);
    int executeExternalCommand(std::vector<std::string> splitcommand);
    int executePipe(std::vector<Command> pipeline);
    int executeInternalCommand(std::vector<std::string> splitcommand);
    bool checkIfInternal(std::string input);
public:
    void executeCommand(std::string command);
};
