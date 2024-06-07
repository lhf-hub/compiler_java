# Java 编译器
## 基于 C++ 实现
### 目前支持的功能
- 词法分析
- 语法分析

## 项目结构说明
- main.cpp：程序入口
- lexer.h：词法分析器的声明
- lexer.cpp：词法分析器的实现
- grammar.h：文法初始化，first集合、follow集合的求解
- predict.h：预测分析表的构造
- syntax.h：语法分析过程的声明（进出栈，递归下降）
- syntax.cpp：语法分析过程的实现
- 
- souce.txt：测试用例
- predictTable.txt：预测分析表的输出
- syntaxParse.txt：语法分析过程的输出（状态栈、输入串、动作）
- 
- lexer_idea.md：词法分析的思路
- syntax_idea.md：语法分析的思路
- readme.md：项目说明