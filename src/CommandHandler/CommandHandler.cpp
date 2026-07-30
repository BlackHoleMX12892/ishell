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
#include "Parser.hpp"

/*std::vector<std::string> CommandHandler::handleCommand(std::string command) {
    std::stringstream commandstream(command);
    std::vector<std::string> splitcommand;
    std::string currentargument;

    while (commandstream >> currentargument) {
        splitcommand.push_back(currentargument);
    }

    return splitcommand;
}*/ // i might remove this

struct PipeObject {
    int pipefd[2];
};

int CommandHandler::executeExternalCommand(std::vector<std::string> splitcommand) {
    pid_t pid = fork();
    int exitcode = 0;
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
    waitpid(pid, &exitcode, 0);
    return exitcode;
}

int CommandHandler::executePipe(std::vector<Command> pipeline) {
    // I need to fix this - done

    std::vector<PipeObject> pipes;
    std::vector<pid_t> processes;

    for (size_t i = 0; i < pipeline.size() - 1; i++) {
        PipeObject temp;
        pipes.push_back(temp);
    }

    for (size_t i = 0; i < pipeline.size() - 1; i++) {
        if (pipe(pipes[i].pipefd) < 0) {
            perror("\033[31mishell\033[0m");
            return 1;
        }
    }

    for (size_t i = 0; i < pipeline.size(); i++) {
        pid_t pid = fork();

        if (pid < 0) {
            std::cout << "Failed to fork process\n";
            return 1;
        } else if (pid == 0) {
            //child process

            if (i > 0) {
                if (dup2(pipes[i - 1].pipefd[0], STDIN_FILENO) < 0) {
                    perror("\033[31mishell\033[0m");
                    std::exit(EXIT_FAILURE);
                }
            }

            if (i < pipeline.size() - 1) {
                if (dup2(pipes[i].pipefd[1], STDOUT_FILENO) < 0) {
                    perror("\033[31mishell\033[0m");
                    std::exit(EXIT_FAILURE);
                }
            }

            // I thoght you had to close all ends except the ones wired to the process, but no, you actually close all
            // that is why dup2 duplicates the file descriptor

            for (size_t x = 0; x < pipes.size(); x++) {
                close(pipes[x].pipefd[1]);
                close(pipes[x].pipefd[0]);
            }
            
            std::vector<char*> arguments;

            for (size_t y = 0; y < pipeline[i].command.size(); y++) {
                arguments.push_back(const_cast<char*>(pipeline[i].command[y].c_str()));
            }
            arguments.push_back(nullptr);

            execvp(arguments[0], arguments.data());

            // if we reach the next code something was wrong with execvp obviously, as it completely replaces the execution

            perror("\033[31mishell\033[0m");
            std::exit(EXIT_FAILURE);
        }

        processes.push_back(pid);
    }

    for (size_t i = 0; i < pipes.size(); i++) {
        close(pipes[i].pipefd[1]);
        close(pipes[i].pipefd[0]);
    }

    int exitcode {0};
    
    for (pid_t pid : processes) {
        waitpid(pid, &exitcode, 0);
    }

    return exitcode;
}

int CommandHandler::executeInternalCommand(std::vector<std::string> splitcommand) {
    if (splitcommand[0] == "exit") {
        std::cout << rang::fg::green << "Thank you for using ishell.\n" << rang::fg::reset;
        std::exit(EXIT_SUCCESS);
        return 0;
    } else if (splitcommand[0] == "help") {
        std::cout << rang::fg::green << "Welcome to ishell help menu:\n" << rang::fg::reset;
        std::cout << "This is a shell designed as an alternative to the bourne-like shells.\n";
        std::cout << "Execute" << rang::fg::red << " \"exit\" " << rang::fg::reset << "to exit the shell.\n";
        return 0;
    } else if (splitcommand[0] == "cd") {
        if (splitcommand.size() == 1) {
            chdir(getenv("HOME"));
            return 0;
        } else {
            if (chdir(splitcommand[1].c_str()) == -1) {
                perror("\033[31mishell\033[0m");
                return 1;
            }
        }
    } else if (splitcommand[0] == "export") {
        if (splitcommand.size() > 1) {
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
            return 0;
        }
    } else if (splitcommand[0] == "history") {
        if (splitcommand.size() > 2) {
            if (splitcommand[1] == "-n") {
                std::stringstream output;
                HistoryHandler historyhandler;
                for (int i = 1; i <= std::stoi(splitcommand[2]); i++) {
                    output << " " << i << " " << historyhandler.getPrevious() << '\n';
                }
                std::cout << output.str();
                return 0;
            }
        } else {
            std::cout << "History command:\n";
            std::cout << " -n [number] Print the last commands.\n";
            return 0;
        }
    }
    return 1;
}

bool CommandHandler::checkIfInternal(std::string input) {
    if (input == "exit" || input == "help" || input == "cd" || input == "export" || input == "history") {
        return true;
    }
    return false;
}

void CommandHandler::executeCommand(std::string command) {
    // temporal solution, need to implement something that can handle multiple cases like "echo $HOME" - done

    /*bool afterSymbol = false;
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
    }*/
    if (!command.empty()) {
        auto commandstructure = Parser::parse(command);

        int lastexitcode {0};
        
        //for the and token we should check whether to execute based on the other pipeline exit code - done
        //might not implement that if it is too difficult - it wasn't
        for (std::vector<Command> pipeline : commandstructure) {
            for (Command cmd : pipeline) {
                if (pipeline.size() >= 2) {
                    lastexitcode = executePipe(pipeline);
                    break;
                } else {
                    if (checkIfInternal(cmd.command[0]) == true) {
                        lastexitcode = executeInternalCommand(cmd.command);
                    } else {
                        lastexitcode = executeExternalCommand(cmd.command);
                    }
                }
            }
            if (lastexitcode != 0) {
                break;
            }
        }
    }
}

