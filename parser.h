#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "CONSTANTS.h"
#include <fstream>

class Parser {
public:
    enum class Keys { C, D, H, ERROR };
    Parser() {
        argc_ = 0;
    }
    Parser(int argc, char** argv);
    int GetArgc() const;
    void Print();
    Keys GetKey();
    std::vector<std::string> GetFileNames();
    std::string GetArchiveName();

private:
    int argc_;
    std::vector<std::string> argv_;
};
