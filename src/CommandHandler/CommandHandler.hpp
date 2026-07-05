#pragma once
#include <vector>
#include <string>

class CommandHandler {
private:
    std::vector<std::string> handleCommand(std::string command);
    void executeExternalCommand(std::vector<std::string> splitcommand);
    void executeInternalCommand(std::vector<std::string> splitcommand);
    bool checkIfInternal(std::string input);
public:
    void executeCommand(std::string command);
};
