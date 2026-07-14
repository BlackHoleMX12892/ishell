#pragma once

#include <string>
#include <stdlib.h>

class HistoryHandler {
private:
    int index = 0;
    int linecount();
    static std::string file;
    std::string getFromFile(int line);
public:
    static void handleHome(); // maybe worth being static - done
    void saveToFile(std::string input);
    std::string getPrevious();
    std::string getNext();
};

//todo: move the homehandling to a separate header, maybe could use a virtual member so it supports multiple files, maybe even a file handler, I don't know - discarded
