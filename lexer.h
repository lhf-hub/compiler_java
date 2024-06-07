#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

using namespace std;

// ����������
enum class TokenType {
    Identifier, // ��ʶ��
    NUMBER, // ����������
    CHAR_LITERAL, // �ַ�������
    STRING_LITERAL, // �ַ���������
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



// �ṹ���ʾ���
struct Token {
    TokenType type;    // ������ͣ����Ƿ���
    string lexeme;     // ��ǵ�ʵ���ı�
    int line;          // �к�
    int column;        // �к�
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

// ���� << ��������ڴ�ӡ���
ostream& operator<<(ostream& os, const Token& token);

// �ʷ��������࣬���ڽ�Դ�����ǻ�
class Lexer {
public:
    // ���캯������Դ������Ϊ����
    Lexer(const string& source);
    
    // ����������ǻ�����
    vector<Token> tokenize();

private:
    string source;  // ��Ҫ��ǻ���Դ����
    size_t position;     // ��ǰ��Դ�����е�λ��
    int line;           // ��ǰ�к�
    int column;         // ��ǰ�к�

    // �����������ڱ�ǻ�
    char peek();                // �鿴��ǰ�ַ�
    char advance();             // ǰ������һ���ַ�
    Token identifier();         // ʶ���ʶ���͹ؼ���
    Token number();             // ʶ������������
    Token string_literal();     // ʶ���ַ���������
    Token char_literal();       // ʶ���ַ�������
    Token op();                 // ʶ�������
    Token punct(char currentChar); // ʶ�������
    TokenType keyword_or_identifier(const string& lexeme); // �ж��ǹؼ��ֻ��Ǳ�ʶ��
};

#endif // LEXER_H
