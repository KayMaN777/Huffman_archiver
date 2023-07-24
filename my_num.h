#pragma once

#include <string>
#include <algorithm>
#include <cmath>
#include "CONSTANTS.h"

class MyNum {
public:
    MyNum() {
        v_ = "";
    }
    explicit MyNum(size_t k) {
        v_.resize(k, '0');
    }
    explicit MyNum(const std::string& v) {
        v_ = v;
    }
    MyNum& operator++();
    void Add(char c);
    void Pop();
    std::string Getv() const;
    void PopFront();
    size_t GetSz() const;
    MyNum& operator=(const MyNum& other);

private:
    std::string v_;
};
