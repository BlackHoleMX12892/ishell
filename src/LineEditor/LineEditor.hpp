#pragma once

#include <string>

class LineEditor {
private:
    void enableRaw();
    void disableRaw();
    std::string clearCommand(std::string command, int position);
public:
    std::string readLine();
};
