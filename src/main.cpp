#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <sstream>
#include <csignal>
#include "ConfigHandler/ConfigHandler.hpp"

bool isItBuiltIn(std::string input) {
    if (input == "exit" || input == "help" || input == "cd")
    {
        return true;
    }
    return false;
}

void handleBuiltIn(std::vector<std::string> arguments) {
    if (arguments[0] == "exit") {
        std::cout << "Thank you for using ishell.\n";
        std::exit(EXIT_SUCCESS);
    } else if (arguments[0] == "help") {
        std::cout << "Welcome to ishell help menu:\n";
        std::cout << "This is a shell designed as an alternative to the bourne-like shells.\n";
        std::cout << "Execute 'exit' to exit the shell.\n";
    } else if (arguments[0] == "cd") {
        if (arguments[1].empty()) {
            chdir(getenv("HOME"));
        } else {
            chdir(arguments[1].c_str());
        }
    }
}

std::vector<std::string> handleCommand(std::string command) {
    std::stringstream ss(command);
    std::vector<std::string> splitcommand;
    std::string currentargument;

    while (ss >> currentargument) {
        splitcommand.push_back(currentargument);
    }

    return splitcommand;
}

int main() {
    ConfigHandler config;
    config.handleConfigFile();
    for (size_t i = 0; i < config.env.size(); i++)
    {
        setenv(config.env.at(i).at(0).c_str(), const_cast<char*>(config.env.at(i).at(1).c_str()), 1);
    }
    std::signal(SIGINT, SIG_IGN);
    std::string command;
    while(true) {
        std::cout << "ishell v1.0 " << "> ";
        std::getline(std::cin, command);
        std::vector<std::string> splitcommand = handleCommand(command);
        if (!splitcommand.empty()) {
            if (isItBuiltIn(splitcommand[0]) == true)
            {
                handleBuiltIn(splitcommand);
            } else {
                pid_t pid = fork();
                if (pid < 0)
                {
                    std::cout << "Failed to fork process\n";
                    std::exit(EXIT_FAILURE);
                } else if (pid == 0) {
                    std::vector<char*> arguments;
                
                    for (size_t i = 0; i < splitcommand.size(); i++)
                    {
                        arguments.push_back(const_cast<char*>(splitcommand[i].c_str()));
                    }
                    arguments.push_back(nullptr);
                    
                    execvp(arguments[0], arguments.data());
                    perror("Error");
                    std::exit(EXIT_FAILURE);
                }
                waitpid(pid, nullptr, 0);
            }
        }
    }
    return 0;
}
