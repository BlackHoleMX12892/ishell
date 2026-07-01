#include "LineEditor.hpp"
#include <termios.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>

struct termios original;

void LineEditor::enableRaw() {
    tcgetattr(STDIN_FILENO, &original);

    struct termios raw = original;

    cfmakeraw(&raw);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void LineEditor::disableRaw() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original);
}

std::string LineEditor::readLine() {
    enableRaw();
    char c;
    int position = 0;
    std::string command;
    while (true)
    {
        ssize_t character = read(STDIN_FILENO, &c, 1);
        if (character == 1 && c == '\r')
        {
            std::cout << "\n\r" << std::flush;
            disableRaw();
            break;
        } else if (character == 1 && (c == '\b' || c == 127)) {
            if (!command.empty()) {
                if (position == command.size())
                {
                    std::cout << "\b \b" << std::flush;
                    command.pop_back();
                    position--;
                } else if (position > 0){
                    std::stringstream backcharacters;
                    command.erase(position - 1, 1);
                    position--;
                    for (int i = 0; i < command.size() - position; i++)
                    {
                        backcharacters << "\033[1D";
                    }
                    std::cout << "\b \b" << command.substr(position, command.size()) << std::flush;
                    std::cout << " \b" << std::flush;
                    std::cout << backcharacters.str() << std::flush;
                }
            }
        } else if (character == 1 && c == '\033') {
            if (read(STDIN_FILENO, &c, 1) == 1 && c == '[') {
                ssize_t character2 = read(STDIN_FILENO, &c, 1);
                if (character2 == 1 && c == 'D') {
                    if (position > 0) {
                        std::cout << "\033[1D" << std::flush;
                        position--;
                    }
                } else if (character2 == 1 && c == 'C') {
                    if (position < command.size())
                    {
                        std::cout << "\033[1C" << std::flush;
                        position++;
                    }
                } else if (character2 == 1 && c == 'A') {
                    //std::cout << "up" << std::flush;
                } else if (character2 == 1 && c == 'B') {
                    //std::cout << "down" << std::flush;
                }
            }
        } else if (character == 1 && std::isprint(c)) {
            if (position == command.size()) {
                std::cout << c << std::flush;
                command.push_back(c);
                position++;
            } else {
                std::stringstream backcharacters;
                command.insert(position, 1, c);
                position++;
                for (int i = 0; i < command.size() - position; i++)
                {
                    backcharacters << "\033[1D";
                }
                std::cout << command.substr(position - 1, command.size()) << std::flush;
                std::cout << backcharacters.str() << std::flush; 
            }
        }
    }
    return command;
}
