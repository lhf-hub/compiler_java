#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <string>

#include "lexer.h"


class Grammar {
private:
    map<string, vector<vector<string>>> grammar;
    map<string, set<string>> first, follow;

public:
    Grammar(const vector<Token>& tokens) {
    // ����
    grammar["A"] = {
        {"B", "A"}, // ������, ����
        {"��"}
    };

    // ������
    grammar["B"] = {
        {"L", "class", "ID", "D"} // ����
    };

    // // �ӿ�����
    // grammar["C"] = {
    //     {"L", "interface", "ID", "E"} // �ӿ���
    // };

    // ����
    grammar["D"] = {
        {"{", "F", "}"} // ���Ա����
    };

    // // �ӿ���
    // grammar["E"] = {
    //     {"{", "G", "}"} // �ӿڳ�Ա����
    // };

//         F --> H F | L F' | ��    // F�����Ա����
// F' --> I F' | public static void C | ��    // F'�����Ա����
    // ���Ա����
    grammar["F"] = {
        {"H", "F"},
        {"I", "F"},
        {"��"}
    };
    // // �ӿڳ�Ա����
    // grammar["G"] = {
    //     {"I", "G"}, // ��������, �ӿڳ�Ա����
    //     {"��"}
    // };

    // ������������
    grammar["H"] = {
        {"J", "ID", "H'"}
    };
        grammar["H'"] = {
        {";"},
        {"=", "K", ";"}
        };

//         I --> L J I'    // I����������
// I' --> ID ( M ) N | main ( String [ ] ID ) N    // I'����������
    // ��������
    grammar["I"] = {
        {"L", "J", "I'"}
    };
        grammar["I'"] = {
        {"ID", "(", "M", ")", "N"},
        {"main", "(", "String", "[", "]", "ID", ")", "N"}
        };

    // ����
    grammar["J"] = {
        {"int"},
        {"char"},
        {"String"},
        {"float"},
        {"double"},
        {"boolean"},
        {"void"}
    };

    // ���ʽ,��������ȼ�
    grammar["K"] = {
        {"KA", "K'"}
    };
        grammar["K'"] = {
        {"||", "KA", "K'"},
        {"��"}
        };

        grammar["KA"] = {
            {"KB", "KA'"}
        };
        grammar["KA'"] = {
            {"&&", "KB", "KA'"},
            {"��"}
        };

        grammar["KB"] = {
            {"KC", "KB'"}
        };
        grammar["KB'"] = {
            {"|", "KC", "KB'"},
            {"��"}
        };

        grammar["KC"] = {
            {"KD", "KC'"}
        };
        grammar["KC'"] = {
            {"&", "KD", "KC'"},
            {"��"}
        };

        grammar["KD"] = {
        {"KE", "KD'"}
        };
        grammar["KD'"] = {
        {"<", "KE"},
        {">", "KE"},
        {"<=", "KE"},
        {">=", "KE"},
        {"==", "KE"},
        {"!=", "KE"},
        {"��"}
        };

        grammar["KE"] = {
        {"O", "KE'"}
        };
        grammar["KE'"] = {
        {"+", "O", "KE'"},
        {"-", "O", "KE'"},
        {"��"}
        };

    // �������η�
    grammar["L"] = {
        {"public"},
        {"private"},
        {"protected"},
        {"static"},
        {"��"}
    };

    // �����б�
    grammar["M"] = {
        {"J", "ID", "M'"},
        {"��"}
    };
        grammar["M'"] = {
        {",", "J", "ID", "M'"},
        {"��"}
        };

    // ������
    grammar["N"] = {
        {"{", "P", "}"} // ����б�
    };

    // ��
    grammar["O"] = {
        {"Q", "O'"} // ����, ��'
    };
        grammar["O'"] = {
        {"*", "Q", "O'"},
        {"/", "Q", "O'"},
        {"%", "Q", "O'"},
            {"++"},
            {"--"},
        {"��"}
            };

    // ����б�
    grammar["P"] = {
        {"R", "P"}, // ���, ����б�
        {"��"}
    };

    // ����
    grammar["Q"] = {
        {"(", "K", ")"}, // ���ʽ
        {"NUM", "Q'"},
        {"ID"}
    };
        grammar["Q'"] = {
        {"++"},
        {"--"},
        {"��"}
        };

    // ���
    grammar["R"] = {
        {"H"}, // ��������
        {"S"}, // ��ֵ���
        {"T"}, // if���
        {"U"}, // while���
        {"V"}, // for���
        {"W"},  // return���
    };

    // ��ֵ��䣺S --> ID = K ; | ID ++ ; | ID -- ; | ID += ; | ID -= ;  // S����ֵ���
    grammar["S"] = {
        {"ID", "S'"}
    };
        // S' --> = S'' ; | ++ ; | -- ; | += K ; | -= K ; | *= K ; | /= K ; | %= K ;
        grammar["S'"] = {
            {"=", "K", ";"},
            {"++", ";"},
            {"--", ";"},
            {"+=", "K", ";"},
            {"-=", "K", ";"},
            {"*=", "K", ";"},
            {"/=", "K", ";"},
            {"%=", "K", ";"},
            {"Y"},
            {"X"}
        };

    // if���
    grammar["T"] = {
        {"if", "(", "K", ")", "N", "else", "N"} // ���ʽ, �����壨����б�
    };

    // while���
    grammar["U"] = {
        {"while", "(", "K", ")", "N"} // ���ʽ, �����壨����б�
    };

    // for���
    grammar["V"] = {
        {"for", "(", "S", "K", ";", "K", ")", "N"}
    };

    // return���
    grammar["W"] = {
        {"return", "K", ";"} // ���ʽ
    };

        // X --> ID . ID ( M ) ;    // X����������
        grammar["X"] = {
            {".", "ID", "(", "M", ")", ";"}
        };
        // Y --> ID = new ID ( ) ;  // Y�����󴴽�
        grammar["Y"] = {
            {"ID", "=", "new", "ID", "(", ")", ";"}
        };


        // ��token���е�ID�ӵ��ķ���
        for (const Token& token : tokens) {
            if (token.type == TokenType::Identifier) {
                if (grammar.count("ID") == 0) {
                    grammar["ID"] = {
                        {token.lexeme}
                    };
                } else {
                    // ���Ҫ�ӵ�ID�ķ����Ѵ��ڣ��򲻼�
                    bool flag = false;
                    for (const auto& production : grammar["ID"]) {
                        if (production[0] == token.lexeme) {
                            flag = true;
                            break;
                        }
                    }
                    if (!flag) {
                        grammar["ID"].push_back({token.lexeme});
                    }
                }
            }
        }
        // ����
        for (const Token& token : tokens) {
            if (token.type == TokenType::NUMBER) {
                if (grammar.count("NUM") == 0) {
                    grammar["NUM"] = {
                        {token.lexeme}
                    };
                } else {
                    grammar["NUM"].push_back({token.lexeme});
                }
            }
        }


    calculate();
}

    void calculate(){
        // ����FIRST����
        for (const auto& rule : grammar) {
            first[rule.first] = calculateFirst(rule.first);
        }

        // ����FOLLOW����
        follow[grammar.begin()->first].insert("#"); // ��ʼ���ŵ�FOLLOW���������"#"
        for (const auto& rule : grammar) {
            calculateFollow(rule.first);
        }
    }

    set<string> calculateFirst(const string& nonTerminal) {
        set<string> result;

        // ����ÿ������ʽ
        for (const auto& production : grammar[nonTerminal]) {
            if (grammar.count(production[0]) == 0) {
                // ������ս��
                result.insert(production[0]);
            } else {
                // ����Ƿ��ս��
                auto firstSet = calculateFirst(production[0]);
                // ȥ��"��"
                firstSet.erase("��");
                result.insert(firstSet.begin(), firstSet.end());
            }
        }

        return result;
    }

    void calculateFollow(const string& nonTerminal) {
        // ����ÿ������ʽ
        for (const auto& rule : grammar) {
            for (int i = 0; i < rule.second.size(); i++) {
                for (int j = 0; j < rule.second[i].size(); j++) {
                    // ����ҵ����ս��
                    if (rule.second[i][j] == nonTerminal) {
                        // ������ս�����ǲ���ʽ�����һ������
                        if (j != rule.second[i].size() - 1) {
                            // �����̷��ŵ�FIRST����
                                // �����̷����Ƿ��ս��
                            if (rule.second[i][j + 1][0] <= 'Z' && rule.second[i][j + 1][0] >= 'A') {
                                auto firstSet = first[rule.second[i][j + 1]];
                                // ��FIRST�����г���"��"������Ԫ����ӵ�FOLLOW������
                                for (const auto& symbol : firstSet) {
                                    if (symbol != "��") {
                                        follow[nonTerminal].insert(symbol);
                                    } else { // ���first�������Цţ���������Թ����Ƶ����ţ���ʱ���󲿷��ս����follow���ϼ��뵽follow������
                                        for (const auto& symbol2 : follow[rule.first]) {
                                            follow[nonTerminal].insert(symbol2);
                                        }
                                    }
                                }
                            } else { // ��̷������ս��
                                follow[nonTerminal].insert(rule.second[i][j + 1]);
                            }

                        }
                        // ������ս���ǲ���ʽ�����һ������
                        if (j == rule.second[i].size() - 1) {
                            // ��Follow(����ʽ��)��ӵ�Follow(nonTerminal)��
                            for (const auto& symbol : follow[rule.first]) {
                                follow[nonTerminal].insert(symbol);
                            }
                        }

                    }
                }
            }
        }
    }

    vector<vector<string>> getProductions(const string& nonTerminal) {
        return grammar[nonTerminal];
    }

    void printGrammar() {
        for (const auto& rule : grammar) {
            cout << "   " << rule.first << "--> ";
            int i = 0;
            for (const auto& production : rule.second) {
                for (const auto& symbol : production) {
                    cout << symbol << " ";
                }
                if (++i < rule.second.size()) {
                    cout << "| ";
                }
            }
            cout << '\n';
        }

        cout << "FIRST���ϣ�" << '\n';
        for (const auto& rule : first) {
            if(rule.first[0] <= 'Z' && rule.first[0] >= 'A') {
                cout << "   FIRST(" << rule.first << ") = ";
                cout << '{';
            }
            int i = 0;
            for (const auto& symbol : rule.second) {
                cout << symbol;
                if (++i < rule.second.size()) {
                    cout << ", ";
                }
            }
            if (rule.first[0] <= 'Z' && rule.first[0] >= 'A'){
                cout << '}';
                cout << '\n';
            }
        }

        cout << "FOLLOW���ϣ�" << '\n';
        for (const auto& rule : follow) {
            cout << "   FOLLOW(" << rule.first << ") = ";
            cout << '{';
            int i = 0;
            for (const auto& symbol : rule.second) {
                cout << symbol;
                if (++i < rule.second.size()) {
                    cout << ", ";
                }
            }
            cout << '}';
            cout << '\n';
        }
    }

    // ������ݹ飺���ķ���������ݹ��ķ�
    // ��ȡ�󹫹�����
    // ���磺S �� aB1|aB2|aB3|aB4|...|aBn|y
    // ��Ϊ��S �� aS' | y   S' �� B1|B2|B3|B4|...|Bn
    void toLL1() {
        first.clear();
        follow.clear();
        // ��ȡ�󹫹����� �п����Ǿֲ��Ĺ������� ������ǰ׺���ֺ�ߵĲ�����ȡ������Ϊһ���µĲ���ʽ����Ϊԭ�����󲿼�һ��'��
        for (const auto& rule : grammar) {
            string nonTerminal = rule.first;
            auto& productions = grammar[nonTerminal];
            sort(productions.begin(), productions.end());

            for (int i = 0; i < productions.size() - 1; ++i) { // ���Ҳ���һ������ʽ��ʼ�������Ĳ���ʽ�Ƚ�
                vector<string> commonPrefix;
                auto& prod1 = productions[i];
                auto& prod2 = productions[i + 1];

                for (int j = 0; j < min(prod1.size(), prod2.size()); ++j) {
                    if (prod1[j] == prod2[j]) {
                        commonPrefix.push_back(prod1[j]);
                    } else {
                        break;
                    }
                }

                if (commonPrefix.size() > 0) {
                    string newNonTerminal = nonTerminal + "'";
                    grammar[newNonTerminal] = {vector<string>(prod1.begin() + commonPrefix.size(), prod1.end()),
                                               vector<string>(prod2.begin() + commonPrefix.size(), prod2.end())};

                    prod1 = commonPrefix;
                    prod1.push_back(newNonTerminal);
                    productions.erase(productions.begin() + i + 1);
                    --i;
                }
            }
        }
        calculate();
    }

    map<string, set<string>> getFirst() {
        return first;
    }

    map<string, set<string>> getFollow() {
        return follow;
    }

    map<string, vector<vector<string>>> getGrammar(){
        return grammar;
    }

    vector<string> getNonTerminals() {
        vector<string> nonTerminals;
        for (const auto& rule : grammar) {
            nonTerminals.push_back(rule.first);
        }
        return nonTerminals;
    }

    vector<string> getTerminals() {
        set<string> terminals;
        for (const auto& rule : grammar) {
            for (const auto& production : rule.second) {
                for (const auto& symbol : production) {
                    if (symbol[0] > 'Z' || symbol[0] < 'A') {
                        terminals.insert(symbol);
                    }
                }
            }
        }
        terminals.erase("��");
        terminals.insert("#");
        return vector<string>(terminals.begin(), terminals.end());
    }


};



#endif //GRAMMAR_H
