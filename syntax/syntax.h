#pragma once
#include "lexical.h"
#include "syntaxtree.h"
using namespace std;

// 节点类型枚举
// 随意列举的，有不对的可以对应修改下
enum NodeType {
    PROGRAM,
    PROGRAM_HEADER,
    SUB_PROGRAM,
    CONST_DECLARATION,
    CONST_DEFINITION,
    VAR_DECLARATION,
    IDENTIFIER,
    COMPOUND_STATEMENT,
    STATEMENT,
    ASSIGNMENT_STATEMENT,
    EXPRESSION,
    TERM,
    FACTOR,
    RELATION_EXPRESSION,
    RELATION_OPERATOR,
    LETTER,
    NUMBER
};

class SyntaxAnalyzer {
public:
    SyntaxAnalyzer(const vector<Word>& lexicalOutput)
        : _lexicalOutput(lexicalOutput), _currentWordIndex(0), _root(nullptr) {}

    bool syntaxanalyze(SyntaxTreeNode* rootNode, NodeType nodetype);
    // 获取语法分析树根节点
    SyntaxTreeNode* getSyntaxTreeRoot() const {
        return _root;
    }

private:
    const vector<Word>& _lexicalOutput;  // 词法分析结果
    size_t _currentWordIndex;           // 当前处理的单词索引
    SyntaxTreeNode* _root;                   // 语法分析树根节点

};


// 语法规则：每个语法分析树节点对应其中一个序号
//1  <程序>→<程序⾸部> <分程序>
//2  <程序⾸部>→PROGRAM <标识符>
//3  <分程序>→[<常量说明>][<变量说明>]<语句>（注：[]中的项表⽰可选）
//4  <常量说明>→CONST <常量定义>{，<常量定义>}; （注：{ }中的项表⽰可重复若⼲次）
//5  <常量定义>→<标识符>: = <⽆符号整数>
//6  <⽆符号整数>→<数字>{ <数字> }
//7  <变量说明>→VAR<标识符>{，<标识符>};
//8  <标识符>→<字⺟>{<字⺟> | <数字>}
//9  <复合语句>→BEGIN <语句>{; <语句>} END
//10 <语句>→<赋值语句> | <条件语句 > | <循环语句> | <复合语句> | <空语句>
//11 <赋值语句>→<标识符>: = <表达式>
//12 <表达式>→[+| -]项 | <表达式> <加法运算符> <项>
//13 <项>→<因⼦> | <项><乘法运算符> <因⼦>
//14 <因⼦>→<标识符> | <⽆符号整数> | (<表达式>)
//15 < 加法运算符 > → + | -
//16 <乘法运算符>→ * | /
//17 <条件语句>→IF <条件> THEN <语句>
//18 <循环语句>→WHILE <条件> DO <语句>
//19 <条件>→<表达式> <关系运算符> <表达式>
//20 <关系运算符>→ = | <> | < | <= | > | >=
//21 <字⺟>→a | b … | x | y | z
//22 <数字>→0 | 1 | … | 8 | 9