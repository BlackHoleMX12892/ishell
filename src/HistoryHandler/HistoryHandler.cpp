#include "HistoryHandler.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <limits>

void HistoryHandler::handleHome() {
    char* home = getenv("HOME");
    if (home != nullptr) {
        file = static_cast<std::string>(home) + "/" + file;
    }
}

void HistoryHandler::saveToFile(std::string input) {
    if (!input.empty())
    {
        std::ofstream outputstream(file, std::ios::app);

        if (outputstream.is_open()) {
            outputstream << input << '\n';
        }
    }
}

std::string HistoryHandler::getFromFile(int line) {
    std::ifstream inputstream(file);
    std::string output;

    if (inputstream.is_open())
    {
        for (int i = 0; i < line; i++)
        {
            std::getline(inputstream, output);
        }

        inputstream.close();
        return output;
    }

    return "oops";
}

int HistoryHandler::linecount() {
    std::ifstream inputstream(file);

    int output = 0;

    while (inputstream.ignore(std::numeric_limits<std::streamsize>::max(), '\n')) {
        output++;
    }

    return output;
}

std::string HistoryHandler::getNext() {
    if (index > 0) {
        index--;
        int lines = linecount();

        return getFromFile(lines - index);
    }
    return "";
}

std::string HistoryHandler::getPrevious() {
    int lines = linecount();
    if (index < lines) {
        index++;
        return getFromFile(lines - index);
    }
    return "";
}
