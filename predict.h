#ifndef PREDICT_H
#define PREDICT_H
#include <fstream>
#include <iomanip>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <filesystem>
#include "grammar.h"


// 根据first和follow集合构造一个预测分析表
class Predict {
private:
    // 预测分析表: 二维表格，行为非终结符，列为终结符，每个单元格存储一个产生式
    map<string, map<string, vector<string>>> table;
    // 文法
    Grammar grammar;
    // first集合
    map<string, set<string>> first;
    // follow集合
    map<string, set<string>> follow;

public:
    Predict(const vector<Token>& tokens) : grammar(tokens){ // 构造函数
        first = grammar.getFirst();
        follow = grammar.getFollow();
        for (const auto& rule : grammar.getGrammar()) {
            string nonTerminal = rule.first;
            for (const auto& production : rule.second) { // 遍历每个产生式的右部
                set<string> firstSet;
                if (production[0] > "Z" || production[0] < "A") {
                    firstSet.insert(production[0]);
                } else {
                    firstSet = first[production[0]];
                }
                for (const auto& terminal : firstSet) {
                    if (terminal != "ε") {
                        table[nonTerminal][terminal] = production;
                    }
                }
                if (firstSet.count("ε") > 0) {
                    for (const auto& terminal : follow[nonTerminal]) {
                        table[nonTerminal][terminal] = production;
                    }
                }
            }
        }
    }

    void printTable() {
        char buf[256];
        GetCurrentDirectory(256, buf); // windows系统下
        // string s = string(buf) + "\\predictTable.txt";
        filesystem::__cxx11::path currentPath = buf;
        filesystem::__cxx11::path parentPath = currentPath.parent_path();
        string filePath = (parentPath/ "predictTable.txt").string();
        ofstream file(filePath);
        if (!file) {
            cerr << "无法打开文件\n";
            return;
        }

        file << "预测分析表：" << '\n';
        file << setw(5) << left << " ";
        for (const auto& terminal : grammar.getTerminals()) {
            file << left  << setw(30)<< terminal; // 设置字段宽度和对齐方式
        }
        file << '\n';
        for (const auto& nonTerminal : grammar.getNonTerminals()) {
            file << setw(5) << left << nonTerminal;
            for (const auto& terminal : grammar.getTerminals()) {
                if (table[nonTerminal].count(terminal) > 0) {
                    string temProduction = "";
                    for (const auto& symbol : table[nonTerminal][terminal]) {
                        if (symbol == "ε") {
                            temProduction += "epsilon ";
                            continue;
                        }
                        temProduction += symbol + " ";
                    }
                    file << left << setw(30)<< nonTerminal + " -> " + temProduction;
                } else {
                    file << left << setw(30) << "X"; // 设置字段宽度和对齐方式
                }
            }
            file << '\n';
        }

        file.close();
    }

    map<string, map<string, vector<string>>> getTable() {
        return table;
    }
};



#endif //PREDICT_H
