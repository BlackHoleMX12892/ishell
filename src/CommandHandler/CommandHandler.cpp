#include "CommandHandler.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <rang.hpp>
#include <filesystem>
#include "../HistoryHandler/HistoryHandler.hpp"

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

void CommandHandler::executePipe(std::vector<std::string> splitcommand1, std::vector<std::string> splitcommand2) {

    int pipefd[2];

    if (pipe(pipefd) == -1)
    {
        perror("\033[31mishell\033[0m");
    }

    pid_t pid1 = fork();

    if (pid1 < 0) {
        std::cout << "Failed to fork process\n";
    } else if (pid1 == 0) {
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("\033[31mishell\033[0m");
            std::exit(EXIT_FAILURE);
        }

        close(pipefd[0]);
        close(pipefd[1]);

        std::vector<char*> arguments;
        
        for (size_t i = 0; i < splitcommand1.size(); i++) {
            arguments.push_back(const_cast<char*>(splitcommand1[i].c_str()));
        }
        arguments.push_back(nullptr);

        execvp(arguments[0], arguments.data());

        perror("\033[31mishell\033[0m");
        std::exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();

    if (pid2 < 0) {
        std::cout << "Failed to fork process\n";
    } else if (pid2 == 0) {
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("\033[31mishell\033[0m");
            std::exit(EXIT_FAILURE);
        }

        close(pipefd[0]);
        close(pipefd[1]);

        std::vector<char*> arguments;
        
        for (size_t i = 0; i < splitcommand2.size(); i++) {
            arguments.push_back(const_cast<char*>(splitcommand2[i].c_str()));
        }
        arguments.push_back(nullptr);

        execvp(arguments[0], arguments.data());

        perror("\033[31mishell\033[0m");
        std::exit(EXIT_FAILURE);
    }
    
    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);
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
            std::filesystem::path pathtochdirto = splitcommand[1].c_str();
            if (std::filesystem::exists(pathtochdirto)) {
                chdir(splitcommand[1].c_str());
            } else {
                perror("\033[31mishell\033[0m");
            }
        }
    } else if (splitcommand[0] == "export") {
        if (!splitcommand[1].empty())
        {
            bool afterEquals = false;
            std::string var;
            std::string val;
            for (int i = 0; i < splitcommand[1].size(); i++) {
                if (splitcommand.at(1)[i] == '=') {
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
    } else if (splitcommand[0] == "history") {
        if (!splitcommand[1].empty() && splitcommand[1] == "-n") {
            std::stringstream output;
            HistoryHandler historyhandler;
            for (int i = 1; i <= std::stoi(splitcommand[2]); i++) {
                output << " " << i << " " << historyhandler.getPrevious() << '\n';
            }
            std::cout << output.str();
        } else {
            std::cout << "History command:\n";
            std::cout << " -n [number] Print the last commands.\n";
        }
    }
}

bool CommandHandler::checkIfInternal(std::string input) {
    if (input == "exit" || input == "help" || input == "cd" || input == "export" || input == "history") {
        return true;
    }
    return false;
}

void CommandHandler::executeCommand(std::string command) {
    // temporal solution, need to implement something that can handle multiple cases like "echo $HOME"

    bool afterSymbol = false;
    std::stringstream command1;
    std::stringstream command2;

    for (int i = 0; i < command.size(); i++) {
        if (command[i] == '|') {
            afterSymbol = true;
        } else {
            if (afterSymbol == false) {
                command1 << command[i];
            } else {
                command2 << command[i];
            }
        }
    }

    std::vector<std::string> splitcommand1 = handleCommand(command1.str());
    std::vector<std::string> splitcommand2 = handleCommand(command2.str());

    if (!splitcommand2.empty()) {
        executePipe(splitcommand1, splitcommand2);
    } else {
        if (!splitcommand1.empty()) {
            if (checkIfInternal(splitcommand1[0]) == true) {
                executeInternalCommand(splitcommand1);
            } else {
                executeExternalCommand(splitcommand1);
            }
        }
    }
}
