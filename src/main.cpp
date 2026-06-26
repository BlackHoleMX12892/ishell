#include <iostream>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <csignal>
#include <rang.hpp>
#include <limits.h>
#include "EnvHandler/EnvHandler.hpp"
#include "CommandHandler/CommandHandler.hpp"
#include "ConfigHandler/ConfigHandler.hpp"
#include "RCHandler/RCHandler.hpp"

bool isItBuiltIn(std::string input) {
    if (input == "exit" || input == "help" || input == "cd" || input == "export")
    {
        return true;
    }
    return false;
}

void handleBuiltIn(std::vector<std::string> arguments) {
    if (arguments[0] == "exit") {
        std::cout << rang::fg::green << "Thank you for using ishell.\n" << rang::fg::reset;
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
    } else if (arguments[0] == "export") {
        if (!arguments[1].empty())
        {
            bool afterEquals = false;
            std::string var;
            std::string val;
            for (int i = 0; i < arguments[1].size(); i++) {
                if (arguments.at(1)[i] == '=')
                {
                    afterEquals = true;
                } else {
                    if (afterEquals == false) {
                        var.push_back(arguments.at(1)[i]);
                    } else {
                        val.push_back(arguments.at(1)[i]);
                    }
                }
            }
            setenv(var.c_str(), val.c_str(), 1);
        }
    }
}

int main() {
    ConfigHandler::handleConfigFile();
    EnvHandler::setenvfromconfig();
    RCHandler::rcfromconfig();
    std::signal(SIGINT, SIG_IGN);
    std::string command;
    while(true) {
        char buffer[PATH_MAX];
        getcwd(buffer, sizeof(buffer));
        std::string path = static_cast<std::string>(buffer);
        if (path == getenv("HOME")) {
            path = "~";
        }
        std::cout << "ishell v0.1.0 " << rang::fg::green << path << rang::fg::reset << " > ";
        std::getline(std::cin, command);
        std::vector<std::string> splitcommand = CommandHandler::handleCommand(command);
        if (!splitcommand.empty()) {
            if (isItBuiltIn(splitcommand[0]) == true)
            {
                handleBuiltIn(splitcommand);
            } else {
                CommandHandler::executeExternalCommand(splitcommand);
            }
        }
    }
    return 0;
}
