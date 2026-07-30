#pragma once

#include <vector>
#include <string>
#include "Lexer.hpp"

struct Command {
    std::vector<std::string> command;

    void clear() {
        command.clear();
    }
};

namespace Parser {
    inline std::vector<std::vector<Command>> parse(std::string command) {
        std::vector<std::vector<Command>> commandstructure;
        std::vector<Token> commandtokens = Lexer::tokenize(command);
        Command currentcommand;
        std::vector<Command> pipeline;

        for (Token token : commandtokens) {
            if (token.type == WORD) {
                currentcommand.command.push_back(token.value);
            } else if (token.type == PIPE) {
                pipeline.push_back(currentcommand);
                currentcommand.clear();
            } else if (token.type == AND) {
                pipeline.push_back(currentcommand);
                currentcommand.clear();
                commandstructure.push_back(pipeline);
                pipeline.clear();
            }
        }
        
        pipeline.push_back(currentcommand);

        commandstructure.push_back(pipeline);

        return commandstructure;
    }
}
