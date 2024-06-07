# 文法分析器
## 文法设计
### 1. 产生式
```text
A --> B A | ε   // A：程序开始
B --> L class ID D    // B：类声明
D --> { F }    // D：类体
F --> H F | I F | ε    // F：类成员定义

H --> J ID H'     // H：变量声明或定义
H' --> = K ; | ;

I --> L J I'    // I：方法声明
I' --> ID ( M ) N | main ( String [ ] ID ) N    // I'：方法调用

J --> int | char | String | float | double | boolean | void   // J：类型

K --> KA K'     // 逻辑或表达式
K' --> || KA K' | ε

KA --> KB KA'       // 逻辑与表达式
KA' --> && KB KA' | ε

KB --> KC KB'       // 按位或表达式
KB' --> | KC KB' | ε

KC --> KD KC'       // 按位与表达式
KC' --> & KD KC' | ε

KD --> KE KD'       // 关系表达式
KD' --> < KE | > KE | <= KE | >= KE | == KE | != KE | ε

KE --> O KE'        // 加减表达式
KE' --> + O KE' | - O KE' | ε

L --> public | private | protected | static | ε   // L：访问修饰符

M --> J ID M' | ε    // M：参数列表
M' --> , J ID M | ε

N --> { P }    // N：方法体（语句块）

O --> Q O'  // O：表达式项
O' --> * Q O' | / Q O' | % Q O' | ++ | -- | ε 

P --> R P | ε    // P：语句列表

Q --> ( K ) | ID Q' | NUM   // Q：表达式因子
Q' --> ++ | -- | ε

R --> H | S | T | U | V | W    // R：语句

S --> ID S'  // S：赋值语句或者X,Y
S' --> = K ; | ++ ; | -- ; | += K ; | -= K ; | *= K ; | /= K ; | %= K ; | Y | X ;

T --> if ( K ) N else N    // T：if语句
U --> while ( K ) N    // U：while语句
V --> for ( S  K ; K ) N    // V：for语句
W --> return K ;    // W：return语句
X --> . ID ( M ) ;    // X：方法调用
Y --> ID = new ID ( ) ;  // Y：对象创建

ID --> 动态获取token流中的终结符
NUM --> 动态获取token流中的终结符
```
### 2. 消除左递归
### 3. 提取左公因子
```text
    · 针对每一条文法规则，寻找右部各个产生式两两间的最长公共前缀，将其剩余部分提取出来作为新的产生式右部，左部命名为新的非终结符；
    · 然后将原来的产生式分解为两部分，一部分是提取出来的公共前缀，另一部分是新的产生式左部。
```
### 4. 求First集合
```text
    · map<string, set<string>> first，key是非终结符，value是该非终结符的first集合；
    · 求解过程：
        1. 两种情况；
            a. 如果右部第一个字符是终结符，则将该终结符加入到first集合中；
            b. 如果右部第一个字符是非终结符，则将该非终结符的first集合去掉空串加入到当前非终结符的first集合中；
```
### 5. 求Follow集合
```text
    · map<string, set<string>> follow，key是非终结符，value是该非终结符的follow集合；
    · 求解过程：
        1. follow(A) = {#}，文法开始；
        2. 两种情况：
            a. 如果A在右部的最后一个，则将左部非终结符的follow加入到follow(A)中；
            b. 如果不在最后一个，则将后继符号的first集合去掉空串加入到follow(A)中；
                b情况下需注意，如果后继符号可以广义推导出空串，则需要将左部非终结符的follow也加入到follow(A)中。
                可广义推导出空串，即代表后继符号的first集合中包含空串，因此直接判断后继符号的first集合是否包含空串即可。
```
### 6. 预测分析表
```text
    · 二维表 map<string, map<string, vector<string>>> table，行表示非终结符，列表示终结符，表中的每个元素是一个产生式的右部；
    · 构造过程：
        1. 针对文法的每一条产生式，遍历其右部；
        2. 如果右部某部分的第一位是终结符，则将该终结符加入到 临时first集合中；
        3. 如果右部某部分的第一位是非终结符，则将该非终结符的 first 集合加入到 临时first 集合中；
        4. 遍历刚才得到的 临时first 集合，先处理first的情况；
        5. 将表中行为产生式左部且列为 临时first（follow） 集合中元素的表格填上当前正在遍历的右部；
        6. 考虑空串的情况，如果 临时first集合中包含空串，则对当前非终结符的 follow 集合执行 4 步骤；
```
### 7. 基于预测分析表分析输入串文法
```text
    · 递归下降思想。
    · 输出三列，第一列是状态栈，第二列是输入串，第三列是语法分析器的动作（匹配、出错、通过或者递归下降到某个产生式）。
```



