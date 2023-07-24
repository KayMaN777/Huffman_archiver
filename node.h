#pragma once
#include <string>
#include <fstream>
#include <map>
#include <set>
#include "my_num.h"
#include "CONSTANTS.h"

class Node {
public:
    Node();
    explicit Node(Char9 c);
    Node(Node* left, Node* right);
    bool GetSv() const;
    Char9 GetC() const;
    Node* GetLeft();
    Node* GetRight();

private:
    Char9 c_;
    bool sv_;
    Node* left_;
    Node* right_;
};

namespace builder {
    Node* Build(std::string name);
    void GetKodes(Node* root, MyNum& code, std::map<Char9, MyNum>& codes);
    void DeleteNode(Node* node);
    std::map<Char9, MyNum> GetCanonicialCodes(std::string name);
}  // namespace builder
