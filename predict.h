#ifndef PREDICT_H
#define PREDICT_H
#include <fstream>
#include <iomanip>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <filesystem>
#include "grammar.h"


// ����first��follow���Ϲ���һ��Ԥ�������
class Predict {
private:
    // Ԥ�������: ��ά�����Ϊ���ս������Ϊ�ս����ÿ����Ԫ��洢һ������ʽ
    map<string, map<string, vector<string>>> table;
    // �ķ�
    Grammar grammar;
    // first����
    map<string, set<string>> first;
    // follow����
    map<string, set<string>> follow;

public:
    Predict(const vector<Token>& tokens) : grammar(tokens){ // ���캯��
        first = grammar.getFirst();
        follow = grammar.getFollow();
        for (const auto& rule : grammar.getGrammar()) {
            string nonTerminal = rule.first;
            for (const auto& production : rule.second) { // ����ÿ������ʽ���Ҳ�
                set<string> firstSet;
                if (production[0] > "Z" || production[0] < "A") {
                    firstSet.insert(production[0]);
                } else {
                    firstSet = first[production[0]];
                }
                for (const auto& terminal : firstSet) {
                    if (terminal != "��") {
                        table[nonTerminal][terminal] = production;
                    }
                }
                if (firstSet.count("��") > 0) {
                    for (const auto& terminal : follow[nonTerminal]) {
                        table[nonTerminal][terminal] = production;
                    }
                }
            }
        }
    }

    void printTable() {
        char buf[256];
        GetCurrentDirectory(256, buf); // windowsϵͳ��
        // string s = string(buf) + "\\predictTable.txt";
        filesystem::__cxx11::path currentPath = buf;
        filesystem::__cxx11::path parentPath = currentPath.parent_path();
        string filePath = (parentPath/ "predictTable.txt").string();
        ofstream file(filePath);
        if (!file) {
            cerr << "�޷����ļ�\n";
            return;
        }

        file << "Ԥ�������" << '\n';
        file << setw(5) << left << " ";
        for (const auto& terminal : grammar.getTerminals()) {
            file << left  << setw(30)<< terminal; // �����ֶο�ȺͶ��뷽ʽ
        }
        file << '\n';
        for (const auto& nonTerminal : grammar.getNonTerminals()) {
            file << setw(5) << left << nonTerminal;
            for (const auto& terminal : grammar.getTerminals()) {
                if (table[nonTerminal].count(terminal) > 0) {
                    string temProduction = "";
                    for (const auto& symbol : table[nonTerminal][terminal]) {
                        if (symbol == "��") {
                            temProduction += "epsilon ";
                            continue;
                        }
                        temProduction += symbol + " ";
                    }
                    file << left << setw(30)<< nonTerminal + " -> " + temProduction;
                } else {
                    file << left << setw(30) << "X"; // �����ֶο�ȺͶ��뷽ʽ
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
