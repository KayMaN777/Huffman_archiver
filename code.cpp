#include "code.h"

bool Comp(const std::pair<MyNum, Char9>& a, std::pair<MyNum, Char9>& b) {
    if (a.first.GetSz() == b.first.GetSz()) {
        return a.second < b.second;
    } else {
        return a.first.GetSz() < b.first.GetSz();
    }
}

void Code::Add(const std::string& code) {
    for (char i : code) {
        data_.push_back(i - '0');
    }
}
void Code::Print(std::ofstream& fout) {
    unsigned char buffer = 0;
    while (data_.size() >= 8) {
        for (int i = 0; i < 8; ++i) {
            buffer <<= 1;
            buffer += data_.front();
            // std::cout << data_.front();
            data_.pop_front();
        }
        fout.put(buffer);
    }
}
void Code::PrintBuffer(std::ofstream& fout) {
    if (data_.empty()) {
        return;
    }
    while (data_.size() < 8) {
        data_.push_back(false);
    }
    Print(fout);
}
void Code::BuildCode(std::ofstream& fout, const std::string& name, std::map<Char9, MyNum>& codes) {
    int symb_count = codes.size();
    // выводим symb_count
    // std::cout << symb_count << std::endl;
    for (int i = 8; i >= 0; --i) {
        data_.push_back(symb_count & (1 << i));
    }
    std::vector<std::pair<MyNum, Char9> > tmp;
    std::map<int, int> symb_sizes;
    size_t mx = 0;
    // ищем MAX_SYMBOL_SIZE и добавляем в tmp символы
    for (const auto& i : codes) {
        ++symb_sizes[i.second.GetSz()];
        mx = std::max(mx, i.second.GetSz());
        tmp.push_back({i.second, i.first});
    }
    sort(tmp.begin(), tmp.end(), Comp);
    // Выводим i-й символ
    for (const auto& i : tmp) {
        for (int j = 8; j >= 0; --j) {
            data_.push_back((i.second & (1 << j)));
        }
    }
    // Выводим количество канонических кодов
    for (size_t i = 1; i <= mx; ++i) {
        int cnt = symb_sizes[i];
        for (int j = 8; j >= 0; --j) {
            data_.push_back(cnt & (1 << j));
        }
    }
    Print(fout);
    std::ifstream fin;
    fin.open(name, std::ios::in | std::ios_base::binary);
    for (char c : name) {
        Char9 symb = static_cast<Char9>(static_cast<unsigned char>(c));
        // Char9 symb = Char9(c);
        std::string code = codes[symb].Getv();
        Add(code);
        Print(fout);
    }
    std::string code_f = codes[FILENAME_END].Getv();
    Add(code_f);
    Print(fout);
    char c = 0;
    while (fin.get(c)) {
        Char9 symb = static_cast<Char9>(static_cast<unsigned char>(c));
        // Char9 symb = Char9(c);
        std::string code = codes[symb].Getv();
        Add(code);
        Print(fout);
    }
}
std::deque<bool> Code::GetData() {
    return data_;
}
