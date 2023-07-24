#include "my_num.h"

MyNum& MyNum::operator++() {
    size_t i = v_.size();
    while (i > 0 && v_[i - 1] == '1') {
        v_[i - 1] = '0';
        --i;
    }
    if (i == 0) {
        std::reverse(v_.begin(), v_.end());
        v_.push_back('1');
        std::reverse(v_.begin(), v_.end());
    } else {
        v_[i - 1] = '1';
    }
    return *this;
}
void MyNum::Add(char c) {
    v_.push_back(c);
}
void MyNum::Pop() {
    v_.pop_back();
}
std::string MyNum::Getv() const {
    return v_;
}
void MyNum::PopFront() {
    std::reverse(v_.begin(), v_.end());
    v_.pop_back();
    std::reverse(v_.begin(), v_.end());
}
size_t MyNum::GetSz() const {
    return v_.size();
}
MyNum& MyNum::operator=(const MyNum& other) {
    v_ = other.v_;
    return *this;
}
