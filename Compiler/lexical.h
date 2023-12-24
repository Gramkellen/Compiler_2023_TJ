#pragma once
#include<iostream>
#include<string>
#include<map>
#include<unordered_map>
#include<unordered_set>
#include<vector>
using namespace std;

void Test01();
string readFile(string filename = "D:\\Files\\SoftWare_5\\编译原理\\期末\\lexical\\Compiler_2023_TJ\\lexical\\program.txt");
//使用hash底层来进行KEYWORDS查找
const unordered_set<string> KEYWORDS = {
    "PROGRAM",
    "BEGIN",
    "END",
    "CONST",
    "VAR",
    "WHILE",
    "DO",
    "IF",
    "THEN"
};

//这里约定了算符和界限符，考虑到后面可能有优先级（？），暂时使用map
const unordered_map<string, int> OPERATORWORD = {
        {"+",           1},
        {"-",           2},
        {"*",           3},
        {"/",           4},
        {":=",          5},
        {"=",           6},
        {"<>",          7},
        {">",           8},
        {">=",          9},
        {"<",           10},
        {"<=",          11},
        {"(",           14},
        {")",           15},
        {";",           16},
        {",",           17},
};

//单词标记
struct Token {
    Token(string type = "", string value = "") :_type(type), _value(value) {}
    string _type;      //单词类型
    string _value;    //单词的值
};

//-----------------词法分析器定义-----------------//
/*语法分析器：
* function : 对输入串进行词法分析，识别输入的token
* 错误检测 : 检测单词拼写是否错误和非法字符
*/

class LexicalAnalyzer {
public:
    LexicalAnalyzer() {};
    bool findKeyords(const string& str);       //查找关键字函数
    int findOperatorWords(const string& str);  //查找算符、界符
    void letterAnalysis(string& str, int& i);  //标识符识别
    bool digitAnalysis(string& str, int& i);   //数字识别
    bool operatorAnalysis(string& str, int& i); //算符检测
    bool Analyze(string str);                  //词法分析
    void Output();                             //输出函数
    const vector<Token>& getInput() {
        return LexicalAnalysisOutput;
    }
private:
    vector<Token> LexicalAnalysisOutput;//单词类型，属性值
};
