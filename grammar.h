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
    // 程序
    grammar["A"] = {
        {"B", "A"}, // 类声明, 程序
        {"ε"}
    };

    // 类声明
    grammar["B"] = {
        {"L", "class", "ID", "D"} // 类体
    };

    // // 接口声明
    // grammar["C"] = {
    //     {"L", "interface", "ID", "E"} // 接口体
    // };

    // 类体
    grammar["D"] = {
        {"{", "F", "}"} // 类成员声明
    };

    // // 接口体
    // grammar["E"] = {
    //     {"{", "G", "}"} // 接口成员声明
    // };

//         F --> H F | L F' | ε    // F：类成员定义
// F' --> I F' | public static void C | ε    // F'：类成员定义
    // 类成员声明
    grammar["F"] = {
        {"H", "F"},
        {"I", "F"},
        {"ε"}
    };
    // // 接口成员声明
    // grammar["G"] = {
    //     {"I", "G"}, // 方法声明, 接口成员声明
    //     {"ε"}
    // };

    // 变量声明或定义
    grammar["H"] = {
        {"J", "ID", "H'"}
    };
        grammar["H'"] = {
        {";"},
        {"=", "K", ";"}
        };

//         I --> L J I'    // I：方法声明
// I' --> ID ( M ) N | main ( String [ ] ID ) N    // I'：方法调用
    // 方法定义
    grammar["I"] = {
        {"L", "J", "I'"}
    };
        grammar["I'"] = {
        {"ID", "(", "M", ")", "N"},
        {"main", "(", "String", "[", "]", "ID", ")", "N"}
        };

    // 类型
    grammar["J"] = {
        {"int"},
        {"char"},
        {"String"},
        {"float"},
        {"double"},
        {"boolean"},
        {"void"}
    };

    // 表达式,运算符优先级
    grammar["K"] = {
        {"KA", "K'"}
    };
        grammar["K'"] = {
        {"||", "KA", "K'"},
        {"ε"}
        };

        grammar["KA"] = {
            {"KB", "KA'"}
        };
        grammar["KA'"] = {
            {"&&", "KB", "KA'"},
            {"ε"}
        };

        grammar["KB"] = {
            {"KC", "KB'"}
        };
        grammar["KB'"] = {
            {"|", "KC", "KB'"},
            {"ε"}
        };

        grammar["KC"] = {
            {"KD", "KC'"}
        };
        grammar["KC'"] = {
            {"&", "KD", "KC'"},
            {"ε"}
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
        {"ε"}
        };

        grammar["KE"] = {
        {"O", "KE'"}
        };
        grammar["KE'"] = {
        {"+", "O", "KE'"},
        {"-", "O", "KE'"},
        {"ε"}
        };

    // 方法修饰符
    grammar["L"] = {
        {"public"},
        {"private"},
        {"protected"},
        {"static"},
        {"ε"}
    };

    // 参数列表
    grammar["M"] = {
        {"J", "ID", "M'"},
        {"ε"}
    };
        grammar["M'"] = {
        {",", "J", "ID", "M'"},
        {"ε"}
        };

    // 方法体
    grammar["N"] = {
        {"{", "P", "}"} // 语句列表
    };

    // 项
    grammar["O"] = {
        {"Q", "O'"} // 因子, 项'
    };
        grammar["O'"] = {
        {"*", "Q", "O'"},
        {"/", "Q", "O'"},
        {"%", "Q", "O'"},
            {"++"},
            {"--"},
        {"ε"}
            };

    // 语句列表
    grammar["P"] = {
        {"R", "P"}, // 语句, 语句列表
        {"ε"}
    };

    // 因子
    grammar["Q"] = {
        {"(", "K", ")"}, // 表达式
        {"NUM", "Q'"},
        {"ID"}
    };
        grammar["Q'"] = {
        {"++"},
        {"--"},
        {"ε"}
        };

    // 语句
    grammar["R"] = {
        {"H"}, // 变量定义
        {"S"}, // 赋值语句
        {"T"}, // if语句
        {"U"}, // while语句
        {"V"}, // for语句
        {"W"},  // return语句
    };

    // 赋值语句：S --> ID = K ; | ID ++ ; | ID -- ; | ID += ; | ID -= ;  // S：赋值语句
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

    // if语句
    grammar["T"] = {
        {"if", "(", "K", ")", "N", "else", "N"} // 表达式, 方法体（语句列表）
    };

    // while语句
    grammar["U"] = {
        {"while", "(", "K", ")", "N"} // 表达式, 方法体（语句列表）
    };

    // for语句
    grammar["V"] = {
        {"for", "(", "S", "K", ";", "K", ")", "N"}
    };

    // return语句
    grammar["W"] = {
        {"return", "K", ";"} // 表达式
    };

        // X --> ID . ID ( M ) ;    // X：方法调用
        grammar["X"] = {
            {".", "ID", "(", "M", ")", ";"}
        };
        // Y --> ID = new ID ( ) ;  // Y：对象创建
        grammar["Y"] = {
            {"ID", "=", "new", "ID", "(", ")", ";"}
        };


        // 将token流中的ID加到文法中
        for (const Token& token : tokens) {
            if (token.type == TokenType::Identifier) {
                if (grammar.count("ID") == 0) {
                    grammar["ID"] = {
                        {token.lexeme}
                    };
                } else {
                    // 如果要加的ID文法中已存在，则不加
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
        // 数字
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
        // 计算FIRST集合
        for (const auto& rule : grammar) {
            first[rule.first] = calculateFirst(rule.first);
        }

        // 计算FOLLOW集合
        follow[grammar.begin()->first].insert("#"); // 开始符号的FOLLOW集合中添加"#"
        for (const auto& rule : grammar) {
            calculateFollow(rule.first);
        }
    }

    set<string> calculateFirst(const string& nonTerminal) {
        set<string> result;

        // 对于每个产生式
        for (const auto& production : grammar[nonTerminal]) {
            if (grammar.count(production[0]) == 0) {
                // 如果是终结符
                result.insert(production[0]);
            } else {
                // 如果是非终结符
                auto firstSet = calculateFirst(production[0]);
                // 去掉"ε"
                firstSet.erase("ε");
                result.insert(firstSet.begin(), firstSet.end());
            }
        }

        return result;
    }

    void calculateFollow(const string& nonTerminal) {
        // 对于每个产生式
        for (const auto& rule : grammar) {
            for (int i = 0; i < rule.second.size(); i++) {
                for (int j = 0; j < rule.second[i].size(); j++) {
                    // 如果找到非终结符
                    if (rule.second[i][j] == nonTerminal) {
                        // 如果非终结符不是产生式的最后一个符号
                        if (j != rule.second[i].size() - 1) {
                            // 计算后继符号的FIRST集合
                                // 如果后继符号是非终结符
                            if (rule.second[i][j + 1][0] <= 'Z' && rule.second[i][j + 1][0] >= 'A') {
                                auto firstSet = first[rule.second[i][j + 1]];
                                // 将FIRST集合中除了"ε"的所有元素添加到FOLLOW集合中
                                for (const auto& symbol : firstSet) {
                                    if (symbol != "ε") {
                                        follow[nonTerminal].insert(symbol);
                                    } else { // 如果first集合中有ε，即代表可以广义推导出ε，此时将左部非终结符的follow集合加入到follow集合中
                                        for (const auto& symbol2 : follow[rule.first]) {
                                            follow[nonTerminal].insert(symbol2);
                                        }
                                    }
                                }
                            } else { // 后继符号是终结符
                                follow[nonTerminal].insert(rule.second[i][j + 1]);
                            }

                        }
                        // 如果非终结符是产生式的最后一个符号
                        if (j == rule.second[i].size() - 1) {
                            // 将Follow(产生式左部)添加到Follow(nonTerminal)中
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

        cout << "FIRST集合：" << '\n';
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

        cout << "FOLLOW集合：" << '\n';
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

    // 消除左递归：该文法已是无左递归文法
    // 提取左公共因子
    // 例如：S → aB1|aB2|aB3|aB4|...|aBn|y
    // 变为：S → aS' | y   S' → B1|B2|B3|B4|...|Bn
    void toLL1() {
        first.clear();
        follow.clear();
        // 提取左公共因子 有可能是局部的公共因子 将公共前缀部分后边的部分提取出来作为一个新的产生式（左部为原来的左部加一个'）
        for (const auto& rule : grammar) {
            string nonTerminal = rule.first;
            auto& productions = grammar[nonTerminal];
            sort(productions.begin(), productions.end());

            for (int i = 0; i < productions.size() - 1; ++i) { // 从右部第一个产生式开始，与后面的产生式比较
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
        terminals.erase("ε");
        terminals.insert("#");
        return vector<string>(terminals.begin(), terminals.end());
    }


};



#endif //GRAMMAR_H
