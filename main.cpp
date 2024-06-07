#include <iostream>
#include <fstream>
#include <sstream>

#include "grammar.h"
#include "lexer.h"
#include "predict.h"
#include "syntax.h"

using namespace std;

int main() {
    // 从文件读取源代码
    ifstream file("E:/Desktop/2023-2024/three2/C++/LL1/source.txt");
    if (!file.is_open()) {
        cout << "无法打开源文件" << '\n';
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string source = buffer.str();

    // 创建词法分析器并进行标记化
    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();

    // 输出标记流
    cout << "Token(" << "值" << ", " << "类型" << ", " << "行号" << ", " << "列号" << ")" << endl;
    for (const Token& token : tokens) {
        cout << token << '\n';
    }
    cout << "词法分析结束。" << '\n';

    // 输出原文法以及first集follow集
    cout << "原文法：\n";
    Grammar grammar(tokens);
    grammar.printGrammar();

    // // 输出更新为LL(1)文法的文法以及first集follow集
    // cout << "更新后的LL(1)文法：\n";
    // grammar.toLL1();
    // grammar.printGrammar();

    Predict predict(tokens);
    predict.printTable();

    // 创建语法分析器并进行语法分析
    Syntax syntax(tokens);
    syntax.parse();

    return 0;


}
