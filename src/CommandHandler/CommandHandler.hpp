#pragma once
#include <vector>
#include <string>

class CommandHandler {
private:
    std::vector<std::string> handleCommand(std::string command);
    void executeExternalCommand(std::vector<std::string> splitcommand);
    void executePipe(std::vector<std::string> splitcommand1, std::vector<std::string> splitcommand2);
    void executeInternalCommand(std::vector<std::string> splitcommand);
    bool checkIfInternal(std::string input);
public:
    void executeCommand(std::string command);
};
