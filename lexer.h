#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

using namespace std;

// 定义标记类型
enum class TokenType {
    Identifier, // 标识符
    NUMBER, // 数字字面量
    CHAR_LITERAL, // 字符字面量
    STRING_LITERAL, // 字符串字面量
    MAIN, // main
    CHAR, // char
    BOOLEAN, // boolean
    BYTE, // byte
    SHORT, // short
    INT, // int
    LONG, // long
    FLOAT, // float
    DOUBLE, // double
    STRING, // String
    TRUE, // true
    FALSE, // false
    IF, // if
    ELSE, // else
    WHILE, // while
    FOR, // for
    RETURN, // return
    VOID, // void
    CLASS, // class
    PUBLIC, // public
    PRIVATE, // private
    PROTECTED, // protected
    STATIC, // static
    PLUS, // +
    MINUS, // -
    INCREMENT, // ++
    DECREMENT, // --
    MULTIPLY, // *
    DIVIDE, // /
    MODULUS, // %
    ASSIGN, // =
    EQUAL, // ==
    NOT_EQUAL, // !=
    LESS_THAN, // <
    GREATER_THAN, // >
    LESS_OR_EQUAL, // <=
    GREATER_OR_EQUAL, // >=
    BITWISE_AND, // &
    BITWISE_OR, // |
    BITWISE_XOR, // ^
    LOGICAL_AND, // &&
    LOGICAL_OR, // ||
    LOGICAL_NOT, // !
    COMMA, // ,
    SEMICOLON, // ;
    LEFT_PAREN, // (
    RIGHT_PAREN, // )
    LEFT_BRACKET, // [
    RIGHT_BRACKET, // ]
    LEFT_ANGLE_BRACKET, // <
    RIGHT_ANGLE_BRACKET, // >
    LEFT_BRACE, // {
    RIGHT_BRACE, // }
    DOT, // .
    UNKNOWN
};



// 结构体表示标记
struct Token {
    TokenType type;    // 标记类型（助记符）
    string lexeme;     // 标记的实际文本
    int line;          // 行号
    int column;        // 列号
    Token(TokenType type, const string& lexeme, int line, int column)
        : type(type), lexeme(lexeme), line(line), column(column) {}
    Token(string lex) {
        type = TokenType::UNKNOWN;
        lexeme = lex;
        line = 0;
        column = 0;
    }

    bool operator==(const Token& other) const {
        return lexeme == other.lexeme;
    }
};

// 重载 << 运算符用于打印标记
ostream& operator<<(ostream& os, const Token& token);

// 词法分析器类，用于将源代码标记化
class Lexer {
public:
    // 构造函数接受源代码作为输入
    Lexer(const string& source);
    
    // 函数启动标记化过程
    vector<Token> tokenize();

private:
    string source;  // 需要标记化的源代码
    size_t position;     // 当前在源代码中的位置
    int line;           // 当前行号
    int column;         // 当前列号

    // 辅助函数用于标记化
    char peek();                // 查看当前字符
    char advance();             // 前进到下一个字符
    Token identifier();         // 识别标识符和关键字
    Token number();             // 识别数字字面量
    Token string_literal();     // 识别字符串字面量
    Token char_literal();       // 识别字符字面量
    Token op();                 // 识别操作符
    Token punct(char currentChar); // 识别标点符号
    TokenType keyword_or_identifier(const string& lexeme); // 判断是关键字还是标识符
};

#endif // LEXER_H
