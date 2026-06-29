#pragma once

#include <string>

class LineEditor {
private:
    void enableRaw();
    void disableRaw();
public:
    std::string readLine();
};
