#include "node.h"

Node::Node() {
    left_ = nullptr;
    right_ = nullptr;
    sv_ = false;
}
Node::Node(Char9 c) {
    c_ = c;
    left_ = nullptr;
    right_ = nullptr;
    sv_ = true;
}
Node::Node(Node* left, Node* right) {
    left_ = left;
    right_ = right;
    c_ = std::min(left_->c_, right_->c_);
    sv_ = false;
}
bool Node::GetSv() const {
    return sv_;
}
Char9 Node::GetC() const {
    return c_;
}
Node* Node::GetLeft() {
    return left_;
}
Node* Node::GetRight() {
    return right_;
}

Char9 GetC(Node* node) {
    return ((node == nullptr) ? 0 : node->GetC());
}

struct Cmp {
    bool operator()(const std::pair<int, Node*>& a, const std::pair<int, Node*>& b) const {
        if (a.first == b.first) {
            return GetC(a.second) < GetC(b.second);
        } else {
            return a.first < b.first;
        }
    }
};

bool Comp(const std::pair<MyNum, Char9>& a, const std::pair<MyNum, Char9>& b) {
    if (a.first.GetSz() == b.first.GetSz()) {
        return a.second < b.second;
    } else {
        return a.first.GetSz() < b.first.GetSz();
    }
}

Node* builder::Build(std::string name) {
    std::ifstream fin;
    fin.open(name, std::ios::in | std::ios::binary);
    std::map<Char9, int> cnt;
    Char9 c = 0;
    char symb = 0;
    while (fin.get(symb)) {
        // std::cout << (int) symb << std::endl;
        c = static_cast<Char9>(static_cast<unsigned char>(symb));
        // c = Char9(symb);
        ++cnt[c];
    }
    fin.close();
    for (char i : name) {
        c = static_cast<Char9>(static_cast<unsigned char>(i));
        // c = Char9(i);
        ++cnt[Char9(c)];
    }
    std::set<std::pair<int, Node*>, Cmp> order;
    for (const auto& i : cnt) {
        Node* cur = new Node(i.first);
        order.insert({i.second, cur});
    }
    order.insert({1, new Node(FILENAME_END)});
    order.insert({1, new Node(ONE_MORE_FILE)});
    order.insert({1, new Node(ARCHIVE_END)});
    while (order.size() >= 2) {
        std::pair<int, Node*> cur1 = *order.begin();
        order.erase(order.begin());
        std::pair<int, Node*> cur2 = *order.begin();
        order.erase(order.begin());
        Node* cur = new Node(cur1.second, cur2.second);
        order.insert({cur1.first + cur2.first, cur});
    }
    if (order.empty()) {
        return nullptr;
    } else {
        return order.begin()->second;
    }
}

void builder::GetKodes(Node* root, MyNum& code, std::map<Char9, MyNum>& codes) {
    if (root == nullptr) {
        return;
    }
    if (root->GetSv()) {
        codes[root->GetC()] = code;
    }
    if (root->GetLeft() != nullptr) {
        code.Add('0');
        GetKodes(root->GetLeft(), code, codes);
        code.Pop();
    }
    if (root->GetRight()) {
        code.Add('1');
        GetKodes(root->GetRight(), code, codes);
        code.Pop();
    }
}
void builder::DeleteNode(Node* node) {
    if (node == nullptr) {
        return;
    }
    if (node->GetLeft() != nullptr) {
        DeleteNode(node->GetLeft());
    }
    if (node->GetRight() != nullptr) {
        DeleteNode(node->GetRight());
    }
    delete node;
}
std::map<Char9, MyNum> builder::GetCanonicialCodes(std::string name) {
    std::map<Char9, MyNum> codes;
    Node* root = Build(name);
    MyNum cur;
    GetKodes(root, cur, codes);
    std::vector<std::pair<MyNum, Char9> > tmp;
    for (const auto& i : codes) {
        // codes[i.first].PopFront();
        tmp.push_back({i.second, i.first});
    }
    sort(tmp.begin(), tmp.end(), Comp);
    if (!tmp.empty()) {
        cur = MyNum(tmp[0].first.GetSz());
        tmp[0].first = cur;
        for (size_t i = 1; i < tmp.size(); ++i) {
            ++cur;
            while (cur.GetSz() < tmp[i].first.GetSz()) {
                cur.Add('0');
            }
            tmp[i].first = cur;
        }
    }
    std::map<Char9, MyNum> res;
    for (size_t i = 0; i < tmp.size(); ++i) {
        res[tmp[i].second] = tmp[i].first;
    }
    DeleteNode(root);
    return res;
}
