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
    // ��ȡ��ǰĿ¼
    char buf[256];
    GetCurrentDirectory(256, buf); // windowsϵͳ��
    filesystem::__cxx11::path currentPath = buf;
    filesystem::__cxx11::path parentPath = currentPath.parent_path();
    string filePath = (parentPath/ "syntaxParse.txt").string();
    ofstream file(filePath);
    if (!file) {
        cerr << "�޷����ļ�\n";
        return;
    }

    file << "��ʼ�﷨����: " << '\n';
    file << setw(20) << left << "ջ"
         << setw(100 )<< left << "���봮"
         << setw(30) << left << "����" << '\n';
    while (st.top() != "#" || input.front() != "#") {
        file.flush();
        string top = st.top();
        string front = input.front();
        if (top == front) {
            printStack(file);
            st.pop();
            printDeque(file);
            input.pop_front();
            file << left << "ƥ�� " << top << '\n';
        } else if (top == "��") {
            printStack(file);
            st.pop();
            printDeque(file);
            file << "�Ƴ� �� " << '\n';
        } else if (pridictTable.find(top) != pridictTable.end() && pridictTable[top].count(front) > 0) { // ���ս��
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
            // ���Ҷ�Ӧ�� Token
            auto it = find_if(tokens.begin(), tokens.end(), [&](const Token& token) {
                return token.lexeme == front;
            });
            if (it != tokens.end()) {
                file << "����λ�ã��кţ��кţ�: ("
                     << it->line << ", "
                     << it->column << ")" << '\n';
            } else {
                file << "����" << '\n';
            }
            file << "�﷨�����쳣�˳���" << '\n';
            file.close();
            return;
        }
    }
    file << "#      #       ͨ��\n";
    file << "�﷨�����ɹ�������" << '\n';

    file.close();
}

