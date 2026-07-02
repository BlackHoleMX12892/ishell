#pragma once

#include <string>
#include <stdlib.h>

class HistoryHandler {
private:
    int index = 0;
    int linecount();
    std::string file = ".ishellhistory";
    std::string getFromFile(int line);
public:
    void handleHome();
    void saveToFile(std::string input);
    std::string getPrevious();
    std::string getNext();
};
