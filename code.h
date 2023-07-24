#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include "my_num.h"
#include <string>
#include <iostream>
#include "CONSTANTS.h"
#include <deque>

class Code {
public:
    Code() {
    }
    void Add(const std::string& code);
    void Print(std::ofstream& fout);
    void PrintBuffer(std::ofstream& fout);
    void BuildCode(std::ofstream& fout, const std::string& name, std::map<Char9, MyNum>& codes);
    std::deque<bool> GetData();

private:
    std::deque<bool> data_;
};
