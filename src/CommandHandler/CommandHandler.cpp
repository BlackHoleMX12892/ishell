#include "CommandHandler.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>

std::vector<std::string> CommandHandler::handleCommand(std::string command) {
    std::stringstream commandstream(command);
    std::vector<std::string> splitcommand;
    std::string currentargument;

    while (commandstream >> currentargument) {
        splitcommand.push_back(currentargument);
    }

    return splitcommand;
}

void CommandHandler::executeExternalCommand(std::vector<std::string> splitcommand) {
    pid_t pid = fork();
    if (pid < 0) {
        std::cout << "Failed to fork process\n";
        std::exit(EXIT_FAILURE);
    } else if (pid == 0) {
        std::vector<char*> arguments;
            
        for (size_t i = 0; i < splitcommand.size(); i++) {
            arguments.push_back(const_cast<char*>(splitcommand[i].c_str()));
        }
        arguments.push_back(nullptr);
                
        execvp(arguments[0], arguments.data());
        perror("\033[31mishell\033[0m");
        std::exit(EXIT_FAILURE);
    }
    waitpid(pid, nullptr, 0);
}
