#ifndef SYNTAX_H
#define SYNTAX_H

#include <map>
#include <stack>
#include <vector>
#include "lexer.h"

using namespace std;

class Syntax {
private:
    map<string, map<string, vector<string>>> pridictTable;
    stack<string> st; // 栈
    deque<string> input; // 输入串
    vector<Token> tokens;
public:
    Syntax(const vector<Token>& tokens);
    void parse();
    void printStack(ofstream& file);
    void printDeque(ofstream& file);
};

#endif // SYNTAX_H
