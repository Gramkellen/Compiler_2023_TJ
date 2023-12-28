#pragma once
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <string>
#include <climits>
#include <algorithm>
#include <fstream>
#include "syntax.h"
using namespace std;


//过程是遍历词法分析的输出，第一个分析的节点默认分析为PROGRAM，遍历暂时没写
bool SyntaxAnalyzer::syntaxanalyze(SyntaxTreeNode* rootNode, NodeType nodetype) {
    if (rootNode == nullptr) {
        rootNode = new SyntaxTreeNode(PROGRAMENTRY, "S");  //文法的开始节点
    }
    bool isOK = _PROGRAM(rootNode);
    isOK = isOK && _SUBPROG(rootNode);
    return isOK;
}

bool SyntaxAnalyzer::_PROGRAM(SyntaxTreeNode* TreeNode)
{
    //因为产生式右部有两个符号，进行递归下降
    //读取下一个符号
    if (_lexicalOutput[_currentWordIndex]._value == "PROGRAM") {
        ++_currentWordIndex; 
        SyntaxTreeNode* childNode = new SyntaxTreeNode(PROGRAM, "PROGRAM"); //分析树添加节点
        //_stable.addEntry("PROGRAM", "0");   //符号表添加符号,这里需要修改
        TreeNode->addChild(childNode);
    }
    else
    {
        return false;
    }
    if (!_IDENTIFIER(TreeNode))
    {
        return false;
    }
    return true;
}

bool SyntaxAnalyzer::_SUBPROG(SyntaxTreeNode* TreeNode) 
{
    if (_lexicalOutput[_currentWordIndex]._value == "CONST" && !_CONSTDECLARATION(TreeNode))
    {
        return false;
    }
    else if (_lexicalOutput[_currentWordIndex]._value == "VAR" && !_VARDECLARATION(TreeNode))
    {
        return false;
    }
    if (!_STATEMENT(TreeNode))
    {
        return false;
    }
    return true;
}

//<常量说明>→CONST <常量定义>{，<常量定义>}; 
bool SyntaxAnalyzer::_CONSTDECLARATION(SyntaxTreeNode* TreeNode)
{
    //因为之前在分程序已经判断了常量说明，所以这里可以认定第一个token是CONST
    SyntaxTreeNode* childNode = new SyntaxTreeNode(CONST_DECLARATION, "常量说明");
    TreeNode->addChild(childNode);
    //_stable.addEntry("PROGRAM", "0");   //符号表添加符号,这里需要修改
    if (!_CONSTDEFINITION(TreeNode)) {
        return false;
    }
    while (_lexicalOutput[_currentWordIndex]._value == ",") {
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(CONST, ",");  //这里可以修改，最后再修改趴
        ++_currentWordIndex;    //读取下一个字符
        TreeNode->addChild(childNode2);
        if (!_CONSTDEFINITION(TreeNode))
        {
            return false;
        }
    }
    return true;
}

//<常量定义>→<标识符>: = <⽆符号整数>
bool SyntaxAnalyzer::_CONSTDEFINITION(SyntaxTreeNode* TreeNode)
{
    SyntaxTreeNode* childNode = new SyntaxTreeNode(CONST_DEFINITION, "常量定义");
    TreeNode->addChild(childNode);
    if (!_IDENTIFIER(TreeNode)) 
    {
        return false;
    }
    if (_lexicalOutput[_currentWordIndex]._value != ":=")
    {
        return false;
    }
    else
    {
        ++_currentWordIndex;  //读取下一个字符
    }
    if (!_UNSIGNEDINTEGER(TreeNode))
    {
        return false;
    }
    return true;
    
}

//<⽆符号整数>→<数字>{ <数字> }
//这里有个坑是第一位为0的情况
bool SyntaxAnalyzer::_UNSIGNEDINTEGER(SyntaxTreeNode* TreeNode)
{
    if (!_NUMBER(TreeNode)) {
        return false;
    }
    //除去首部为0的多数字情形，此时默认为错误的
    if (_lexicalOutput[_currentWordIndex - 1]._value == "0" &&
        _lexicalOutput[_currentWordIndex]._type == "NUMBER")
    {
        return false;
    }
    while (_lexicalOutput[_currentWordIndex]._type == "NUMBER")
    {
        
        if (!_NUMBER(TreeNode)) {
            return false;
        }
    }
    return true;
}

//<变量说明>→VAR<标识符>{，<标识符>};
bool SyntaxAnalyzer::_VARDECLARATION(SyntaxTreeNode* TreeNode)
{
    ++_currentWordIndex;
    //因为之前在分程序已经判断了变量说明，所以这里可以认定第一个token是VAR
    SyntaxTreeNode* childNode = new SyntaxTreeNode(VAR_DECLARATION, "VAR");
    TreeNode->addChild(childNode);
    //_stable.addEntry("PROGRAM", "0");   //符号表添加符号,这里需要修改

    if (!_IDENTIFIER(TreeNode)) {
        return false;
    }
    while (_lexicalOutput[_currentWordIndex]._value == ",") {
        ++_currentWordIndex;    //读取下一个字符
        if (!_IDENTIFIER(TreeNode))
        {
            return false;
        }
    }
    if (_lexicalOutput[_currentWordIndex]._value == ";")
    {
        ++_currentWordIndex;
    }
    else {
        return false;
    }
    return true;
}

//<标识符>→<字⺟>{<字⺟> | <数字>}
bool SyntaxAnalyzer::_IDENTIFIER(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._type == "IDENTIFIER") {
        ++_currentWordIndex; //读取下一个token
        return true;
    }
    return false;
}

//<复合语句>→BEGIN <语句>{; <语句>} END
bool SyntaxAnalyzer::_COMPOUNDSTATEMENT(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._value == "BEGIN") {
        ++_currentWordIndex;  //读取下一个字符
        //。。。可以添加
    }

    //判断语句
    if (!_STATEMENT(TreeNode))
    {
        return false;
    }
    while (_lexicalOutput[_currentWordIndex]._value == ";") {
        ++_currentWordIndex;  //读取下一个字符
        if (!_STATEMENT(TreeNode))
        {
            return false;
        }
    }
    if (_lexicalOutput[_currentWordIndex]._value == "END") {
        ++_currentWordIndex;
        return true;
    }
    return false;
}

//<语句>→<赋值语句> | <条件语句 > | <循环语句> | <复合语句> | <空语句>
bool SyntaxAnalyzer::_STATEMENT(SyntaxTreeNode* TreeNode)
{
    
    if (_lexicalOutput[_currentWordIndex]._type == "IDENTIFIER" ) {  //赋值语句
        return _ASSIGNMENTSTATEMENT(TreeNode);  //进入
    } 
    else if (_lexicalOutput[_currentWordIndex]._value == "IF") {  //条件语句
        return _CONDITIONALSTATEMENT(TreeNode);
    }
    else if (_lexicalOutput[_currentWordIndex]._value == "WHILE") { //循环语句
        return _LOOPSTATEMENT(TreeNode);
    }
    else if (_lexicalOutput[_currentWordIndex]._value == "BEGIN") {  //复合语句
        return _COMPOUNDSTATEMENT(TreeNode);
    }
    else {
        return _EMPTYSTATEMENT(TreeNode);   //空语句
    }
    return true;
}

//<赋值语句>→<标识符>: = <表达式>
bool SyntaxAnalyzer::_ASSIGNMENTSTATEMENT(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._type == "IDENTIFIER") {
        ++_currentWordIndex;
    }
    else {
        return false;
    }
    if (_lexicalOutput[_currentWordIndex]._value == ":=") {
        ++_currentWordIndex;
    }else {
        return false;
    }
    if (!_EXPRESSION(TreeNode)) {
        return false;
    }
    return true;
}

/*-----------这里需要消除左递归---------------*/
//<表达式>→[+| -]<项> | <表达式> <加法运算符> <项>
//<表达式>→[+| -]<项><表达式'>   
//<表达式'>→<加法运算符> <项><表达式'> | <空语句>
//<表达式>:
bool SyntaxAnalyzer::_EXPRESSION(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._value == "+" ||
        _lexicalOutput[_currentWordIndex]._value == "-") {
        ++_currentWordIndex;  //读取下一个字符
    }
    if (!_TERM(TreeNode)) {
        return false;
    }
    if (!_EXPRESSION2(TreeNode)) {
        return false;
    }
    return true;
}
//<表达式'>:
bool SyntaxAnalyzer::_EXPRESSION2(SyntaxTreeNode* TreeNode) {

    if (_lexicalOutput[_currentWordIndex]._value == "+" ||
        _lexicalOutput[_currentWordIndex]._value == "-") {

        if (!_ADDITIVEOPERATOR(TreeNode)) {
            return false;
        }
        if (!_TERM(TreeNode)) {
            return false;
        }
        if (!_EXPRESSION2(TreeNode)) {
            return false;
        }
        return true;
    }
    else
    {
        return _EMPTYSTATEMENT(TreeNode);
    }
   
}

/*-----------这里需要消除左递归---------------*/
//<项>→<因⼦> | <项><乘法运算符> <因⼦>
//<项> →<因⼦> <项">
//<项">→<乘法运算符> <因⼦> <项"> | <空语句>
bool SyntaxAnalyzer::_TERM(SyntaxTreeNode* TreeNode)
{
    if (!_FACTOR(TreeNode)) {
        return false;
    }
    if (!_TERM2(TreeNode)) {
        return false;
    }
    return true;
}

bool SyntaxAnalyzer::_TERM2(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._value == "*" ||
        _lexicalOutput[_currentWordIndex]._value == "/") {

        if (!_MULTIPLICATIVEOPERATOR(TreeNode)) {
            return false;
        }
        if (!_FACTOR(TreeNode)) {
            return false;
        }
        if (!_TERM2(TreeNode)) {
            return false;
        }
        return true;
    }
    else
    {
        return _EMPTYSTATEMENT(TreeNode);
    }
   
}

//<因⼦>→<标识符> | <⽆符号整数> | (<表达式>)
bool SyntaxAnalyzer::_FACTOR(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._type == "IDENTIFIER") {
        return _IDENTIFIER(TreeNode);
    }
    else if (_lexicalOutput[_currentWordIndex]._type == "NUMBER") {
        return _UNSIGNEDINTEGER(TreeNode);
    }
    else if (_lexicalOutput[_currentWordIndex]._value == "(") {
        ++_currentWordIndex;       //读取下一个token
        if (!_EXPRESSION(TreeNode)) {
            return false;
        }
        if (_lexicalOutput[_currentWordIndex]._value != ")") {
            return false;
        }
        ++_currentWordIndex;
        return true;
    }
    return false;
}

//<加法运算符> → + | -
bool SyntaxAnalyzer::_ADDITIVEOPERATOR(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._value == "+" || 
        _lexicalOutput[_currentWordIndex]._value == "-") {
        ++_currentWordIndex;
        return true;
    }
    return false;
}

//16 <乘法运算符>→ * | /
bool SyntaxAnalyzer::_MULTIPLICATIVEOPERATOR(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._value == "*" ||
        _lexicalOutput[_currentWordIndex]._value == "/") {
        ++_currentWordIndex;
        return true;
    }
    return false;
}

//17 <条件语句>→IF <条件> THEN <语句>
bool SyntaxAnalyzer::_CONDITIONALSTATEMENT(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._value == "IF") {
        _currentWordIndex++;  //读取下一个字符
    }
    else {
        return false;
    }

    if (!_CONDITION(TreeNode)) {
        return false;
    }
    //读取下一个标识符
    if (_lexicalOutput[_currentWordIndex]._value == "THEN") {
        _currentWordIndex++;  //读取下一个字符
    }
    else {
        return false;
    }
    if (!_STATEMENT(TreeNode)) {
        return false;
    }
    return true;
}

//18 <循环语句>→WHILE <条件> DO <语句>
bool SyntaxAnalyzer::_LOOPSTATEMENT(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._value == "WHILE") {
        _currentWordIndex++;  //读取下一个字符
    }
    else {
        return false;
    }

    if (!_CONDITION(TreeNode)) {
        return false;
    }
    //读取下一个标识符
    if (_lexicalOutput[_currentWordIndex]._value == "DO") {
        _currentWordIndex++;  //读取下一个字符
    }
    else {
        return false;
    }
    if (!_STATEMENT(TreeNode)) {
        return false;
    }
    return true;
}

//19 <条件>→<表达式> <关系运算符> <表达式>
bool SyntaxAnalyzer::_CONDITION(SyntaxTreeNode* TreeNode)
{
    if (!_EXPRESSION(TreeNode)) {
        return false;
    }
    if (!_RELATIONOPERATOR(TreeNode)) {
        return false;
    }
    if (!_EXPRESSION(TreeNode)) {
        return false;
    }
    return true;
}

//20 <关系运算符>→ = | <> | < | <= | > | >=
bool SyntaxAnalyzer::_RELATIONOPERATOR(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._value == "=" || 
        _lexicalOutput[_currentWordIndex]._value == "<>" || 
        _lexicalOutput[_currentWordIndex]._value == "<" || 
        _lexicalOutput[_currentWordIndex]._value == "<=" || 
        _lexicalOutput[_currentWordIndex]._value == ">" || 
        _lexicalOutput[_currentWordIndex]._value == ">=") {
        ++_currentWordIndex;
        return true;
    }
    else {
        return false;
    }
}

//21 <字⺟>→a | b … | x | y | z
bool SyntaxAnalyzer::_LETTER(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._value>="a" && 
        _lexicalOutput[_currentWordIndex]._value <= "z") {
        ++_currentWordIndex;
        return true;
    }
    else {
        return false;
    }
}

//22 <数字>→0 | 1 | … | 8 | 9
bool SyntaxAnalyzer::_NUMBER(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._value >= "0" &&
        _lexicalOutput[_currentWordIndex]._value <= "9") {
        ++_currentWordIndex;
        return true;
    }
    else {
        return false;
    }
}

//<空语句>
bool SyntaxAnalyzer::_EMPTYSTATEMENT(SyntaxTreeNode* TreeNode)
{
    return true;
}