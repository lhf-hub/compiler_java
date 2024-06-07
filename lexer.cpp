#include "lexer.h"
#include <iostream>
#include <cctype>
#include <unordered_map>

using namespace std;
//总映射map,key为TokenType，value为对应的字符串,用于输出
const unordered_map<TokenType, string> tokenTypeStrings = {
    {TokenType::Identifier, "Identifier"},
    {TokenType::NUMBER, "NUMBER"},
    {TokenType::CHAR_LITERAL, "CHAR_LITERAL"},
    {TokenType::STRING_LITERAL, "STRING_LITERAL"},
    {TokenType::MAIN, "MAIN"},
    {TokenType::CHAR, "CHAR"},
    {TokenType::BOOLEAN, "BOOLEAN"},
    {TokenType::BYTE, "BYTE"},
    {TokenType::SHORT, "SHORT"},
    {TokenType::INT, "INT"},
    {TokenType::LONG, "LONG"},
    {TokenType::FLOAT, "FLOAT"},
    {TokenType::DOUBLE, "DOUBLE"},
    {TokenType::STRING, "STRING"},
    {TokenType::TRUE, "TRUE"},
    {TokenType::FALSE, "FALSE"},
    {TokenType::IF, "IF"},
    {TokenType::ELSE, "ELSE"},
    {TokenType::WHILE, "WHILE"},
    {TokenType::FOR, "FOR"},
    {TokenType::RETURN, "RETURN"},
    {TokenType::VOID, "VOID"},
    {TokenType::CLASS, "CLASS"},
    {TokenType::PUBLIC, "PUBLIC"},
    {TokenType::PRIVATE, "PRIVATE"},
    {TokenType::PROTECTED, "PROTECTED"},
    {TokenType::STATIC, "STATIC"},
    {TokenType::PLUS, "PLUS"},
    {TokenType::MINUS, "MINUS"},
    {TokenType::INCREMENT, "INCREMENT"},
    {TokenType::DECREMENT, "DECREMENT"},
    {TokenType::MULTIPLY, "MULTIPLY"},
    {TokenType::DIVIDE, "DIVIDE"},
    {TokenType::MODULUS, "MODULUS"},
    {TokenType::ASSIGN, "ASSIGN"},
    {TokenType::EQUAL, "EQUAL"},
    {TokenType::NOT_EQUAL, "NOT_EQUAL"},
    {TokenType::LESS_THAN, "LESS_THAN"},
    {TokenType::GREATER_THAN, "GREATER_THAN"},
    {TokenType::LESS_OR_EQUAL, "LESS_OR_EQUAL"},
    {TokenType::GREATER_OR_EQUAL, "GREATER_OR_EQUAL"},
    {TokenType::BITWISE_AND, "BITWISE_AND"},
    {TokenType::BITWISE_OR, "BITWISE_OR"},
    {TokenType::BITWISE_XOR, "BITWISE_XOR"},
    {TokenType::LOGICAL_AND, "LOGICAL_AND"},
    {TokenType::LOGICAL_OR, "LOGICAL_OR"},
    {TokenType::LOGICAL_NOT, "LOGICAL_NOT"},
    {TokenType::COMMA, "COMMA"},
    {TokenType::SEMICOLON, "SEMICOLON"},
    {TokenType::LEFT_PAREN, "LEFT_PAREN"},
    {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
    {TokenType::LEFT_BRACKET, "LEFT_BRACKET"},
    {TokenType::RIGHT_BRACKET, "RIGHT_BRACKET"},
    {TokenType::LEFT_ANGLE_BRACKET, "LEFT_ANGLE_BRACKET"},
    {TokenType::RIGHT_ANGLE_BRACKET, "RIGHT_ANGLE_BRACKET"},
    {TokenType::LEFT_BRACE, "LEFT_BRACE"},
    {TokenType::RIGHT_BRACE, "RIGHT_BRACE"},
    {TokenType::DOT, "DOT"},
};

// 定义关键字、运算符和标点符号的对应TokenType映射map
const unordered_map<string, TokenType> keywords = {
    {"main", TokenType::MAIN},
    {"char", TokenType::CHAR},
    {"boolean", TokenType::BOOLEAN},
    {"byte", TokenType::BYTE},
    {"short", TokenType::SHORT},
    {"int", TokenType::INT},
    {"long", TokenType::LONG},
    {"float", TokenType::FLOAT},
    {"double", TokenType::DOUBLE},
    {"string", TokenType::STRING},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"while", TokenType::WHILE},
    {"for", TokenType::FOR},
    {"return", TokenType::RETURN},
    {"void", TokenType::VOID},
    {"class", TokenType::CLASS},
    {"public", TokenType::PUBLIC},
    {"private", TokenType::PRIVATE},
    {"protected", TokenType::PROTECTED},
    {"static", TokenType::STATIC}
};

const unordered_map<string, TokenType> operators = {
    {"+", TokenType::PLUS},
    {"-", TokenType::MINUS},
    {"++", TokenType::INCREMENT},
    {"--", TokenType::DECREMENT},
    {"*", TokenType::MULTIPLY},
    {"/", TokenType::DIVIDE},
    {"%", TokenType::MODULUS},
    {"=", TokenType::ASSIGN},
    {"==", TokenType::EQUAL},
    {"!=", TokenType::NOT_EQUAL},
    {"<", TokenType::LESS_THAN},
    {">", TokenType::GREATER_THAN},
    {"<=", TokenType::LESS_OR_EQUAL},
    {">=", TokenType::GREATER_OR_EQUAL},
    {"&", TokenType::BITWISE_AND},
    {"|", TokenType::BITWISE_OR},
    {"^", TokenType::BITWISE_XOR},
    {"&&", TokenType::LOGICAL_AND},
    {"||", TokenType::LOGICAL_OR},
    {"!", TokenType::LOGICAL_NOT}
};

const unordered_map<char, TokenType> punctuation = {
    {',', TokenType::COMMA},
    {';', TokenType::SEMICOLON},
    {'(', TokenType::LEFT_PAREN},
    {')', TokenType::RIGHT_PAREN},
    {'[', TokenType::LEFT_BRACKET},
    {']', TokenType::RIGHT_BRACKET},
    {'<', TokenType::LEFT_ANGLE_BRACKET},
    {'>', TokenType::RIGHT_ANGLE_BRACKET},
    {'{', TokenType::LEFT_BRACE},
    {'}', TokenType::RIGHT_BRACE},
    {'.', TokenType::DOT}
};

// 重载 << 运算符用于打印标记
ostream& operator<<(ostream& os, const Token& token) {
    os << "Token(" << token.lexeme << ", " << tokenTypeStrings.at(token.type) << ", " << token.line << ", " << token.column << ")";
    return os;
}

// 构造函数初始化源代码和位置
Lexer::Lexer(const string& source) : source(source), position(0), line(1), column(1) {}

// 标记化源代码并返回标记向量
vector<Token> Lexer::tokenize() {
    vector<Token> tokens;
    while (position < source.size()) {
        char currentChar = peek();
        if (isspace(currentChar)) { // isspace() 函数用于检查字符是否为空白字符
            if (currentChar == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            advance();  // 跳过空白字符
        } else if (isalpha(currentChar) || currentChar == '_') { // isalpha() 函数用于检查字符是否为字母
            tokens.push_back(identifier());  // 识别标识符或关键字
        } else if (isdigit(currentChar)) {
            tokens.push_back(number());  // 识别数字字面量
        } else if (currentChar == '"') {
            tokens.push_back(string_literal());  // 识别字符串字面量
        } else if (currentChar == '\'') {
            tokens.push_back(char_literal()); // 识别字符字面量
        } else if (operators.count(string(1, currentChar))) {
            tokens.push_back(op());  // 识别操作符
        } else if (punctuation.count(currentChar)) {
            tokens.push_back(punct(currentChar));  // 识别标点符号
            advance();
        } else {
            tokens.push_back({TokenType::UNKNOWN, string(1, currentChar), line, column});  // 识别未知标记
            advance();
            column++;
        }
    }
    return tokens;
}

// 查看当前字符
char Lexer::peek() {
    return source[position];
}

// 前进到下一个字符
char Lexer::advance() {
    return source[position++];
}

// 识别标识符和关键字
Token Lexer::identifier() {
    string lexeme;
    int startColumn = column;
    while (isalnum(peek()) || peek() == '_') { // isalnum() 函数用于检查字符是否为字母或数字
        lexeme += advance();
        column++;
    }
    TokenType type = keyword_or_identifier(lexeme);
    return {type, lexeme, line, startColumn};
}

// 识别数字字面量
Token Lexer::number() {
    string lexeme;
    int startColumn = column;
    while (isdigit(peek()) || peek() == '.'){
        lexeme += advance();
        column++;
    }
    return {TokenType::NUMBER, lexeme, line, startColumn};
}

// 识别字符串字面量
Token Lexer::string_literal() {
    string lexeme;
    int startColumn = column;
    advance(); // 跳过开头的双引号
    column++;
    while (peek() != '"' && position < source.size()) {
        lexeme += advance();
        column++;
    }
    advance(); // 跳过结尾的双引号
    column++;
    return {TokenType::STRING_LITERAL, lexeme, line, startColumn};
}

// 识别字符字面量
Token Lexer::char_literal() {
    string lexeme;
    int startColumn = column;
    advance(); // 跳过开头的单引号
    column++;
    while (peek() != '\'' && position < source.size()) {
        lexeme += advance();
        column++;
    }
    advance(); // 跳过结尾的单引号
    column++;
    return {TokenType::CHAR_LITERAL, lexeme, line, startColumn};
}

// 识别操作符
Token Lexer::op() {
    string lexeme;
    int startColumn = column;
    char currentChar = peek();

    // 处理双字符运算符
    if (currentChar == '+' || currentChar == '-' || currentChar == '=' ||
        currentChar == '!' || currentChar == '<' || currentChar == '>' ||
        currentChar == '&' || currentChar == '|') {
        lexeme += advance();  // 读取第一个字符
        char nextChar = peek(); // 查看下一个字符
        if ((currentChar == '+' && nextChar == '+') ||
            (currentChar == '-' && nextChar == '-') ||
            (currentChar == '=' && nextChar == '=') ||
            (currentChar == '!' && nextChar == '=') ||
            (currentChar == '<' && nextChar == '=') ||
            (currentChar == '>' && nextChar == '=') ||
            (currentChar == '&' && nextChar == '&') ||
            (currentChar == '|' && nextChar == '|')) {
            lexeme += advance();  // 读取第二个字符
            }
        } else {
            // 处理单字符运算符
            lexeme += advance();
        }

    column += lexeme.size();

    return {operators.at(lexeme), lexeme, line, startColumn};
}


// 识别标点符号
Token Lexer::punct(char currentChar) {
    string lexeme(1, currentChar);
    return {punctuation.at(currentChar), lexeme, line, column};
}

// 判断是关键字还是标识符
TokenType Lexer::keyword_or_identifier(const string& lexeme) {
    auto it = keywords.find(lexeme);
    if (it != keywords.end()) {
        return it->second;
    } else {
        return TokenType::Identifier;
    }
}
