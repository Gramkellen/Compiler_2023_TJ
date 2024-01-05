#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <string>
#include <climits>
#include <algorithm>
#include <fstream>
#include "lexical.h"
#include "syntaxtree.h"
#include "symboltable.h"
using namespace std;

const string EMPTYCHARATER = "eslong";  //给空语句加个标识
/*---------------语法树------------------*/
class SyntaxAnalyzer {
public:
    SyntaxAnalyzer(const vector<Token>& lexicalOutput)
        : _lexicalOutput(lexicalOutput), _currentWordIndex(0), _root(nullptr), counter(0) {}

    string syntaxanalyze(SyntaxTreeNode* rootNode, NodeType nodetype);

    // 获取语法分析树根节点
    SyntaxTreeNode* getSyntaxTreeRoot() const {
        return _root;
    }
    
    void printcode();

    //<程序⾸部>→PROGRAM <标识符>
    string _PROGRAM(SyntaxTreeNode* TreeNode);

    //<分程序>→[<常量说明>][<变量说明>]<语句>
    string _SUBPROG(SyntaxTreeNode* TreeNode);

    //<常量说明>→CONST <常量定义>{，<常量定义>}; 
    string _CONSTDECLARATION(SyntaxTreeNode* TreeNode);

    //<常量定义>→<标识符>: = <⽆符号整数>
    string _CONSTDEFINITION(SyntaxTreeNode* TreeNode);

    //<⽆符号整数>→<数字>{ <数字> }
    string _UNSIGNEDINTEGER(SyntaxTreeNode* TreeNode);

    //<变量说明>→VAR<标识符>{，<标识符>}
    string _VARDECLARATION(SyntaxTreeNode* TreeNode);

    //<标识符>→<字⺟>{<字⺟> | <数字>}
    string _IDENTIFIER(SyntaxTreeNode* TreeNode);

    //<复合语句>→BEGIN <语句>{; <语句>} END
    string _COMPOUNDSTATEMENT(SyntaxTreeNode* TreeNode);

    //<语句>→<赋值语句> | <条件语句 > | <循环语句> | <复合语句> | <空语句>
    string _STATEMENT(SyntaxTreeNode* TreeNode);

    //<赋值语句>→<标识符>: = <表达式>
    string _ASSIGNMENTSTATEMENT(SyntaxTreeNode* TreeNode);

    //<表达式>→[+| -]项 | <表达式> <加法运算符> <项>
    /*-----------这里需要消除左递归---------------*/
    //<表达式>→[+| -]<项><表达式'>   
    //<表达式'>→<加法运算符> <项><表达式'> | <空语句>
    string _EXPRESSION(SyntaxTreeNode* TreeNode);

    string _EXPRESSION2(SyntaxTreeNode* TreeNode);

    //<项>→<因⼦> | <项><乘法运算符> <因⼦>
    /*-----------这里需要消除左递归---------------*/
    //<项> →<因⼦> <项">
    //<项">→<乘法运算符> <因⼦><项"> | <空语句>
    string _TERM(SyntaxTreeNode* TreeNode);

    string _TERM2(SyntaxTreeNode* TreeNode);

    //<因⼦>→<标识符> | <⽆符号整数> | (<表达式>)
    string _FACTOR(SyntaxTreeNode* TreeNode);

    //<加法运算符 > → + | -
    string _ADDITIVEOPERATOR(SyntaxTreeNode* TreeNode);

    //16 <乘法运算符>→ * | /
    string _MULTIPLICATIVEOPERATOR(SyntaxTreeNode* TreeNode);

    //17 <条件语句>→IF <条件> THEN <语句>
    string _CONDITIONALSTATEMENT(SyntaxTreeNode* TreeNode);

    //18 <循环语句>→WHILE <条件> DO <语句>
    string _LOOPSTATEMENT(SyntaxTreeNode* TreeNode);

    //19 <条件>→<表达式> <关系运算符> <表达式>
    string _CONDITION(SyntaxTreeNode* TreeNode);

    //20 <关系运算符>→ = | <> | < | <= | > | >=
    string _RELATIONOPERATOR(SyntaxTreeNode* TreeNode);

    //21 <字⺟>→a | b … | x | y | z
    string _LETTER(SyntaxTreeNode* TreeNode);

    //22 <数字>→0 | 1 | … | 8 | 9
    string _NUMBER(SyntaxTreeNode* TreeNode);

    //<空语句>
    string _EMPTYSTATEMENT(SyntaxTreeNode* TreeNode);

    string allocateTemporary() {
        string temporary = "t" + to_string(counter);
        counter++;
        return temporary;
    }

    string lastTemporary() {
        string temporary = "t" + to_string(counter-1);
        return temporary;
    }

private:
    const vector<Token>& _lexicalOutput;  // 词法分析结果
    size_t _currentWordIndex;             // 当前处理的单词索引
    SyntaxTreeNode* _root;                // 语法分析树根节点
    SymbolTable _stable;                  //建立符号表
    vector<string> mediate_code;
    int mediatecode_index;
    int counter;
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
//15 <加法运算符 > → + | -
//16 <乘法运算符>→ * | /
//17 <条件语句>→IF <条件> THEN <语句>
//18 <循环语句>→WHILE <条件> DO <语句>
//19 <条件>→<表达式> <关系运算符> <表达式>
//20 <关系运算符>→ = | <> | < | <= | > | >=
//21 <字⺟>→a | b … | x | y | z
//22 <数字>→0 | 1 | … | 8 | 9