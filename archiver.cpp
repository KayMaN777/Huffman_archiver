#include "parser.h"
#include "node.h"
#include "code.h"
#include <deque>

void CompressFiles(const std::string& archive_name, const std::vector<std::string>& names) {
    Code code;
    std::map<Char9, MyNum> codes;
    std::ofstream fout;
    fout.open(archive_name, std::ios::out | std::ios_base::binary);
    for (const auto& name : names) {
        codes = builder::GetCanonicialCodes(name);
        code.BuildCode(fout, name, codes);
        if (name != names.back()) {
            code.Add(codes[ONE_MORE_FILE].Getv());
        }
        code.Print(fout);
    }
    code.Add(codes[ARCHIVE_END].Getv());
    code.Print(fout);
    code.PrintBuffer(fout);
    fout.close();
    std::cout << std::endl;
}

Char9 GetCur(std::deque<bool>& q) {
    Char9 cur = 0;
    for (int x = 0; x < 9; ++x) {
        cur <<= 1;
        cur += q.front();
        q.pop_front();
    }
    return cur;
}

bool CanGetCode(std::deque<bool>& q, const std::map<std::string, Char9>& cnt) {
    std::string cur;
    for (size_t i = 0; i < q.size(); ++i) {
        cur.push_back(char('0' + q[i]));
        if (cnt.count(cur)) {
            return true;
        }
    }
    return false;
}

std::string GetCode(std::deque<bool>& q, const std::map<std::string, Char9>& cnt) {
    std::string cur;
    while (!cnt.count(cur)) {
        cur.push_back(char('0' + q.front()));
        q.pop_front();
    }
    return cur;
}

void Decompress(const std::string& archive_name) {
    std::ifstream fin;
    fin.open(archive_name, std::ios::in | std::ios_base::binary);
    char symb = 0;
    std::deque<bool> tmp;
    bool count_symbols_founded = false;
    Char9 count_symbols = 0;
    Char9 readed_symbols = 0;
    Char9 getted_symbol_code = 0;
    MyNum current_code;
    Char9 code_size = 1;
    std::vector<Char9> symbols;
    std::vector<MyNum> codes;
    std::map<std::string, Char9> cnt;
    bool maked_map = false;
    std::string file_name;
    bool file_name_founded = false;
    std::ofstream fout;
    bool fout_opened = false;
    while (fin.get(symb)) {
        for (int i = 7; i >= 0; --i) {
            tmp.push_back((symb & (1 << i)));
        }
        if (!maked_map || !count_symbols_founded) {
            while (tmp.size() >= 9) {
                Char9 cur = GetCur(tmp);
                if (!count_symbols_founded) {
                    count_symbols = cur;
                    count_symbols_founded = true;
                } else if (readed_symbols < count_symbols) {
                    symbols.push_back(cur);
                    ++readed_symbols;
                } else if (getted_symbol_code < count_symbols) {
                    while (current_code.GetSz() < code_size) {
                        current_code.Add('0');
                    }
                    for (Char9 i = 0; i < cur; ++i) {
                        codes.push_back(current_code);
                        ++current_code;
                    }
                    getted_symbol_code += cur;
                    ++code_size;
                    if (getted_symbol_code == count_symbols) {
                        for (int i = 0; i < count_symbols; ++i) {
                            cnt[codes[i].Getv()] = symbols[i];
                        }
                        maked_map = true;
                        break;
                    }
                }
            }
        } else {
            while (CanGetCode(tmp, cnt)) {
                std::string code = GetCode(tmp, cnt);
                Char9 c = cnt[code];
                if (c == FILENAME_END) {
                    fout.open(file_name, std::ios::out | std::ios_base::binary);
                    fout_opened = true;
                    file_name_founded = true;
                } else if (!file_name_founded) {
                    file_name.push_back(char(c));
                } else if (c == ONE_MORE_FILE) {
                    count_symbols_founded = false;
                    count_symbols = 0;
                    readed_symbols = 0;
                    getted_symbol_code = 0;
                    current_code = MyNum();
                    code_size = 1;
                    symbols.clear();
                    codes.clear();
                    cnt.clear();
                    maked_map = false;
                    file_name.clear();
                    file_name_founded = false;
                    fout.close();
                    fout_opened = false;
                    break;
                } else if (c == ARCHIVE_END) {
                    break;
                } else {
                    fout.put(char(c));
                }
            }
        }
    }
    if (fout_opened) {
        fout.close();
    }
    fin.close();
}

int main(int argc, char** argv) {
    Parser pars = Parser(argc, argv);
    /*std::cout << pars.GetArgc() << std::endl;
    pars.Print();*/
    if (pars.GetKey() == Parser::Keys::ERROR) {
        std::cout << "ERROR\nYou must enter key\nFor help print archiver -h" << std::endl;
    } else if (pars.GetKey() == Parser::Keys::C) {
        if (argc <= 3) {
            std::cout
                    << "ERROR\nThere are missed some of arguments: archive_name or file_name\nFor help print archiver -h"
                    << std::endl;
        } else {
            // архивируем
            std::string archive_name = pars.GetArchiveName();
            std::vector<std::string> names = pars.GetFileNames();
            try {
                CompressFiles(archive_name, names);
            } catch (const std::exception& e) {
                std::cout << e.what() << std::endl;
                return 111;
            }
        }
    } else if (pars.GetKey() == Parser::Keys::D) {
        if (argc != 3) {
            std::cout << "ERROR\nYou should write only archive_name\nFor help print archiver -h" << std::endl;
        } else {
            // разархивируваем file_name
            std::string archive_name = pars.GetArchiveName();
            try {
                Decompress(archive_name);
            } catch (const std::exception& e) {
                std::cout << e.what() << std::endl;
                return 111;
            }
        }
    } else {
        std::cout << "Commands must have next view:\narchiver -c ARCHIVE_NAME file1 file2 ... to compress files file1 "
                     "file2 ... in ARCHIVE_NAME\n"
                  << "archiver -d ARCHIVE_NAME to decompress files from ARCHIVE_NAME" << std::endl;
    }
    return 0;
}
