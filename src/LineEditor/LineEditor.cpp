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
                std::cout << "\b \b" << std::flush;
                command.pop_back();
            }
        } else if (character == 1 && std::isprint(c)) {
            std::cout << c << std::flush;
            command.push_back(c);
        }
    }
    return command;
}
