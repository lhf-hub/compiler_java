# 词法分析器
## Token结构体设计
### 属性
- `type` : TokenType // 标记类型
- `lexeme` : String // 标记内容
- `line` : int // 行号
- `column` : int // 列号
### TokenType枚举
- `IDENTIFIER`
- `NUMBER`
- `STRING_LITERAL`
- `CHAR_LITERAL`
- `CHAR`
- `INT`
- `FLOAT`
- `等等（基本符号表中的类型）`
## 词法分析器类的设计
### 属性
- `source` : String
- `pos` : size_t // 当前读到的位置
- `line` : int // 当前读到的行号
- `column` : int // 当前读到的列号
### 方法
- `Lexer(source: String)` : void
- `tokenize()` : Token[]
- `peek()` : char
- `advance()` : char
- `identifier()` : Token
- `number()` : Token
- `string_literal()` : Token
- `char_literal()` : Token
- `op` : Token
- `punct` : Token
- `keyword_or_identifier()` : Token
