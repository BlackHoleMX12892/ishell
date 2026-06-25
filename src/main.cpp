#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <sstream>
#include <csignal>
#include <rang.hpp>
#include <limits.h>
#include "EnvHandler/EnvHandler.hpp"

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

std::vector<std::string> handleCommand(std::string command) {
    std::stringstream commandstream(command);
    std::vector<std::string> splitcommand;
    std::string currentargument;

    while (commandstream >> currentargument) {
        splitcommand.push_back(currentargument);
    }

    return splitcommand;
}

int main() {
    EnvHandler::setenvfromconfig();
    std::signal(SIGINT, SIG_IGN);
    std::string command;
    while(true) {
        char buffer[PATH_MAX];
        getcwd(buffer, sizeof(buffer));
        std::string path = static_cast<std::string>(buffer);
        if (path == getenv("HOME")) {
            path = "~";
        }
        std::cout << "ishell v1.0 " << rang::fg::green << path << rang::fg::reset << " > ";
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
                    perror("\033[31mishell\033[0m");
                    std::exit(EXIT_FAILURE);
                }
                waitpid(pid, nullptr, 0);
            }
        }
    }
    return 0;
}
