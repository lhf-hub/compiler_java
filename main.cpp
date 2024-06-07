#include <iostream>
#include <fstream>
#include <sstream>

#include "grammar.h"
#include "lexer.h"
#include "predict.h"
#include "syntax.h"

using namespace std;

int main() {
    // ���ļ���ȡԴ����
    ifstream file("E:/Desktop/2023-2024/three2/C++/LL1/source.txt");
    if (!file.is_open()) {
        cout << "�޷���Դ�ļ�" << '\n';
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string source = buffer.str();

    // �����ʷ������������б�ǻ�
    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();

    // ��������
    cout << "Token(" << "ֵ" << ", " << "����" << ", " << "�к�" << ", " << "�к�" << ")" << endl;
    for (const Token& token : tokens) {
        cout << token << '\n';
    }
    cout << "�ʷ�����������" << '\n';

    // ���ԭ�ķ��Լ�first��follow��
    cout << "ԭ�ķ���\n";
    Grammar grammar(tokens);
    grammar.printGrammar();

    // // �������ΪLL(1)�ķ����ķ��Լ�first��follow��
    // cout << "���º��LL(1)�ķ���\n";
    // grammar.toLL1();
    // grammar.printGrammar();

    Predict predict(tokens);
    predict.printTable();

    // �����﷨�������������﷨����
    Syntax syntax(tokens);
    syntax.parse();

    return 0;


}
