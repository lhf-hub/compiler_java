#include "syntax.h"

#include "predict.h"

#include <fstream>
#include <iomanip>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


Syntax::Syntax(const vector<Token>& tokens) {
    Predict predict(tokens);
    pridictTable = predict.getTable();
    st.push("#");
    st.push("A");
    for (auto token = tokens.begin(); token != tokens.end(); ++token) {
        input.push_back(token->lexeme);
    }
    input.push_back("#");
}
void Syntax::printStack(ofstream& file) {
    vector<string> tempVec;
    for (stack<string> tempStack = st; !tempStack.empty(); tempStack.pop()) {
        tempVec.push_back(tempStack.top());
    }
    reverse(tempVec.begin(), tempVec.end());
    for (const auto& str : tempVec) {
        file << str << " ";
    }
    file << "       ";
}

void Syntax::printDeque(ofstream& file) {
    deque<string> temDeque = input;
    while (!temDeque.empty()) {
        file << temDeque.front() << " ";
        temDeque.pop_front();
    }
    file << "       ";
}
void Syntax::parse() {
    // 获取当前目录
    char buf[256];
    GetCurrentDirectory(256, buf); // windows系统下
    filesystem::__cxx11::path currentPath = buf;
    filesystem::__cxx11::path parentPath = currentPath.parent_path();
    string filePath = (parentPath/ "syntaxParse.txt").string();
    ofstream file(filePath);
    if (!file) {
        cerr << "无法打开文件\n";
        return;
    }

    file << "开始语法分析: " << '\n';
    file << setw(20) << left << "栈"
         << setw(100 )<< left << "输入串"
         << setw(30) << left << "动作" << '\n';
    while (st.top() != "#" || input.front() != "#") {
        file.flush();
        string top = st.top();
        string front = input.front();
        if (top == front) {
            printStack(file);
            st.pop();
            printDeque(file);
            input.pop_front();
            file << left << "匹配 " << top << '\n';
        } else if (top == "ε") {
            printStack(file);
            st.pop();
            printDeque(file);
            file << "移出 ε " << '\n';
        } else if (pridictTable.find(top) != pridictTable.end() && pridictTable[top].count(front) > 0) { // 非终结符
            vector<string> production = pridictTable[top][front];
            printStack(file);
            st.pop();
            printDeque(file);
            file << top << " -> ";
            for (const auto& symbol : production) {
                file << symbol << " ";
            }
            for (auto it = production.rbegin(); it != production.rend(); ++it) {
                st.push(*it);
            }
            file << '\n';
        }
        else {
            printStack(file);
            printDeque(file);
            // 查找对应的 Token
            auto it = find_if(tokens.begin(), tokens.end(), [&](const Token& token) {
                return token.lexeme == front;
            });
            if (it != tokens.end()) {
                file << "错误！位置（行号，列号）: ("
                     << it->line << ", "
                     << it->column << ")" << '\n';
            } else {
                file << "错误！" << '\n';
            }
            file << "语法分析异常退出。" << '\n';
            file.close();
            return;
        }
    }
    file << "#      #       通过\n";
    file << "语法分析成功结束。" << '\n';

    file.close();
}

