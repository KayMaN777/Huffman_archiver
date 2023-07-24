#include "parser.h"

Parser::Parser(int argc, char** argv) {
    argc_ = argc;
    for (int i = 0; i < argc_; ++i) {
        argv_.push_back(argv[i]);
    }
}
int Parser::GetArgc() const {
    return argc_;
}
void Parser::Print() {
    for (int i = 0; i < argc_; ++i) {
        std::cout << argv_[i] << std::endl;
    }
}
Parser::Keys Parser::GetKey() {
    if (argc_ == 1) {
        return Keys::ERROR;
    } else if (argv_[1] == "-c") {
        return Keys::C;
    } else if (argv_[1] == "-d") {
        return Keys::D;
    } else if (argv_[1] == "-h") {
        return Keys::H;
    } else {
        return Keys::ERROR;
    }
}
std::vector<std::string> Parser::GetFileNames() {
    std::vector<std::string> res;
    for (size_t i = 3; i < argv_.size(); ++i) {
        res.push_back(argv_[i]);
    }
    return res;
}
std::string Parser::GetArchiveName() {
    return argv_[2];
}
