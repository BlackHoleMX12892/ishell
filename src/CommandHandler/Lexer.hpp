#pragma once

#include <string>
#include <vector>

enum TokenType {
    WORD,
    PIPE,
    AND
};

struct Token {
    TokenType type;
    std::string value;
};

namespace Lexer {
    std::vector<Token> tokenize(std::string command) {
        std::vector<Token> temp {};
        bool isQuoted = false;
        char currentquote;
        std::string currentword;

        for (size_t i = 0; i < command.size(); i++) {
            char c = command[i];

            if (!isQuoted) {
                switch (c) {
                    case '|':
                        if (!currentword.empty()) {
                            temp.push_back({TokenType::WORD, currentword});
                            currentword.clear();
                        }
                        temp.push_back({TokenType::PIPE, "|"});
                        break;
                    case '&':
                        if (command[i + 1] == '&') {
                            if (!currentword.empty()) {
                                temp.push_back({TokenType::WORD, currentword});
                                currentword.clear();
                            }
                            temp.push_back({TokenType::AND, "&&"});
                        }
                        break;
                    case '\"':
                        isQuoted = true;
                        currentquote = '\"';
                        break;
                    case '\'':
                        isQuoted = true;
                        currentquote = '\'';
                        break;
                    case ' ':
                        if (!currentword.empty()) {
                            temp.push_back({TokenType::WORD, currentword});
                            currentword.clear();
                        }
                    break;
                    case '\t':
                    case '\n':
                    case '\r':
                        break;
                    default:
                        currentword.push_back(c);
                        break;
                }
            } else if (isQuoted) {
                if ((c == '\"' || c == '\'') && c == currentquote) {
                    isQuoted = false;
                } else {
                    currentword.push_back(c);
                }
            }
        }

        if (!currentword.empty()) {
            temp.push_back({TokenType::WORD, currentword});
            currentword.clear();
        }

        return temp;
    }
}
