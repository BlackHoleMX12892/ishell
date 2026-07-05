#include "CommandHandler.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <rang.hpp>

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

void CommandHandler::executeInternalCommand(std::vector<std::string> splitcommand) {
    if (splitcommand[0] == "exit") {
        std::cout << rang::fg::green << "Thank you for using ishell.\n" << rang::fg::reset;
        std::exit(EXIT_SUCCESS);
    } else if (splitcommand[0] == "help") {
        std::cout << rang::fg::green << "Welcome to ishell help menu:\n" << rang::fg::reset;
        std::cout << "This is a shell designed as an alternative to the bourne-like shells.\n";
        std::cout << "Execute" << rang::fg::red << " \"exit\" " << rang::fg::reset << "to exit the shell.\n";
    } else if (splitcommand[0] == "cd") {
        if (splitcommand[1].empty()) {
            chdir(getenv("HOME"));
        } else {
            chdir(splitcommand[1].c_str());
        }
    } else if (splitcommand[0] == "export") {
        if (!splitcommand[1].empty())
        {
            bool afterEquals = false;
            std::string var;
            std::string val;
            for (int i = 0; i < splitcommand[1].size(); i++) {
                if (splitcommand.at(1)[i] == '=')
                {
                    afterEquals = true;
                } else {
                    if (afterEquals == false) {
                        var.push_back(splitcommand.at(1)[i]);
                    } else {
                        val.push_back(splitcommand.at(1)[i]);
                    }
                }
            }
            setenv(var.c_str(), val.c_str(), 1);
        }
    }
}

bool CommandHandler::checkIfInternal(std::string input) {
    if (input == "exit" || input == "help" || input == "cd" || input == "export")
    {
        return true;
    }
    return false;
}

void CommandHandler::executeCommand(std::string command) {
    std::vector<std::string> splitcommand = handleCommand(command);
    if (!splitcommand.empty()) {
        if (checkIfInternal(splitcommand[0]) == true) {
            executeInternalCommand(splitcommand);
        } else {
            executeExternalCommand(splitcommand);
        }
    }
}
