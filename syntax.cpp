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
        this->_root = rootNode;
    }
    return _PROGRAM(this->_root) && _SUBPROG(this->_root);
}

//<程序⾸部>→PROGRAM <标识符>
bool SyntaxAnalyzer::_PROGRAM(SyntaxTreeNode* TreeNode)
{
    //因为产生式右部有两个符号，进行递归下降
    //读取下一个符号
    if (_lexicalOutput[_currentWordIndex]._value == "PROGRAM") {
        ++_currentWordIndex; 
        SyntaxTreeNode* childNode = new SyntaxTreeNode(PROGRAM, "PROGRAM"); //分析树添加节点
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        //_stable.addEntry("PROGRAM", "0");   //符号表添加符号,这里需要修改
        TreeNode->addChild(childNode);
    }
    else{
        return false;
    }
    if (_IDENTIFIER(TreeNode))
    {
        SyntaxTreeNode* childNode = new SyntaxTreeNode(identifier, ""); //分析树添加节点
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode);
    }
    else {
        return false;
    }
    return true;
}

//<分程序>→[<常量说明>][<变量说明>]<语句>
bool SyntaxAnalyzer::_SUBPROG(SyntaxTreeNode* TreeNode) 
{
    if (_lexicalOutput[_currentWordIndex]._value == "CONST")
    {
        if (_CONSTDECLARATION(TreeNode)) {
            ++_currentWordIndex;  //读取下一个字符
            SyntaxTreeNode* childNode = new SyntaxTreeNode(const_declaration, "");
            cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
            TreeNode->addChild(childNode);
        }
        else {
            return false;
        }
    }
    else if (_lexicalOutput[_currentWordIndex]._value == "VAR")
    {
        if (_VARDECLARATION(TreeNode)) {
            ++_currentWordIndex;  //读取下一个字符
            SyntaxTreeNode* childNode = new SyntaxTreeNode(var_declaration, "");
            cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
            TreeNode->addChild(childNode);
        }
        else {
            return false;
        }
    }
    if (_STATEMENT(TreeNode))
    {
        SyntaxTreeNode* childNode = new SyntaxTreeNode(statement, "");
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode);
    }
    else {
        cout << _lexicalOutput[_currentWordIndex]._value << " " << _lexicalOutput[_currentWordIndex]._value << endl;
        cout << "statement error" << endl;
        return false;
    }
    return true;
}

// <常量说明>→CONST <常量定义>{，<常量定义>}; 
bool SyntaxAnalyzer::_CONSTDECLARATION(SyntaxTreeNode* TreeNode)
{
    //因为之前在分程序已经判断了常量说明，所以这里可以认定第一个token是CONST
    SyntaxTreeNode* childNode = new SyntaxTreeNode(CONST, "CONST");
    cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
    TreeNode->addChild(childNode);
    //_stable.addEntry("PROGRAM", "0");   //符号表添加符号,这里需要修改
    if (_CONSTDEFINITION(TreeNode)) {
        SyntaxTreeNode* childNode = new SyntaxTreeNode(const_definition, "");
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode);
    }
    else {
        return false;
    }
    while (_lexicalOutput[_currentWordIndex]._value == ",") {
        ++_currentWordIndex;    //读取下一个字符
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(COMMA, ",");  //这里可以修改，最后再修改趴
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode2);
        if (_CONSTDEFINITION(TreeNode))
        {
            SyntaxTreeNode* childNode = new SyntaxTreeNode(const_definition, "");
            TreeNode->addChild(childNode);
        }
        else {
            return false;
        }
    }
    return true;
}

//<常量定义>→<标识符>: = <⽆符号整数>
bool SyntaxAnalyzer::_CONSTDEFINITION(SyntaxTreeNode* TreeNode)
{
    if (_IDENTIFIER(TreeNode)) 
    {
        SyntaxTreeNode* childNode = new SyntaxTreeNode(identifier, "");
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode);
    }
    else {
        return false;
    }
    if (_lexicalOutput[_currentWordIndex]._value == ":=")
    {
        ++_currentWordIndex;  //读取下一个字符
        SyntaxTreeNode* childNode = new SyntaxTreeNode(ASSIGNMENT, ":=");
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode);
    }
    else
    {
        return false;
    }
    if (_UNSIGNEDINTEGER(TreeNode))
    {
        SyntaxTreeNode* childNode = new SyntaxTreeNode(unsigned_integer, "");
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode);
    }
    else {
        return false;
    }
    return true;
    
}

//<⽆符号整数>→<数字>{ <数字> }
//这里有个坑是第一位为0的情况
bool SyntaxAnalyzer::_UNSIGNEDINTEGER(SyntaxTreeNode* TreeNode)
{
        if (_NUMBER(TreeNode)) {
        SyntaxTreeNode* childNode = new SyntaxTreeNode(NUMBER, TreeNode->getValue());
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode);
    }
    else {
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
        ++_currentWordIndex;  //读取下一个字符
        if (_NUMBER(TreeNode)) {
            // 这里没想好怎么处理循环
            SyntaxTreeNode* childNode = new SyntaxTreeNode(NUMBER, TreeNode->getValue());
            cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
            TreeNode->addChild(childNode);
        }
        else {
            return false;
        }
    }
    return true;
}

//<变量说明>→VAR<标识符>{，<标识符>}
bool SyntaxAnalyzer::_VARDECLARATION(SyntaxTreeNode* TreeNode)
{
    ++_currentWordIndex;
    //因为之前在分程序已经判断了变量说明，所以这里可以认定第一个token是VAR
    SyntaxTreeNode* childNode = new SyntaxTreeNode(VAR_DECLARATION, "VAR");
    cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
    TreeNode->addChild(childNode);
    //_stable.addEntry("PROGRAM", "0");   //符号表添加符号,这里需要修改

    if (_IDENTIFIER(TreeNode)) {
        SyntaxTreeNode* childNode = new SyntaxTreeNode(identifier, "");
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode);
    }
    else {
        return false;
    }
    while (_lexicalOutput[_currentWordIndex]._value == ",") {
        ++_currentWordIndex;
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(COMMA, ","); 
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode2);
        if (_IDENTIFIER(TreeNode))
        {
            SyntaxTreeNode* childNode3 = new SyntaxTreeNode(identifier, "");
            cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
            TreeNode->addChild(childNode3);
        }
        else {
            return false;
        }
    }
    if (_lexicalOutput[_currentWordIndex]._value == ";") {
        ++_currentWordIndex;
    }
    else {
        return false;
    }
    return true;
}

//<标识符>→<字⺟>{<字⺟> | <数字>}
// 这里是不是少了个循环
bool SyntaxAnalyzer::_IDENTIFIER(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._type == "IDENTIFIER") {
        
        SyntaxTreeNode* childNode = new SyntaxTreeNode(IDENTIFIER, _lexicalOutput[_currentWordIndex]._value);
        ++_currentWordIndex; //读取下一个token
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode);
        return true;
    }
    return false;
}

//<复合语句>→BEGIN <语句>{; <语句>} END
// 这个函数有问题
bool SyntaxAnalyzer::_COMPOUNDSTATEMENT(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._value == "BEGIN") {
        ++_currentWordIndex;  //读取下一个字符
        //。。。可以添加
        SyntaxTreeNode* childNode = new SyntaxTreeNode(BEGIN, "BEGIN");
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode);
    }

    //判断语句
    if (_STATEMENT(TreeNode))
    {
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(statement, "");
        cout << "add node: " << childNode2->getType() << " " << childNode2->getValue() << endl;
        TreeNode->addChild(childNode2);
    }
    else {
        return false;
    }
    cout << _lexicalOutput[_currentWordIndex]._type << " " << _lexicalOutput[_currentWordIndex]._value << endl;
    while (_lexicalOutput[_currentWordIndex]._value == ";") {
        ++_currentWordIndex;  //读取下一个字符
        if (_STATEMENT(TreeNode))
        {
            SyntaxTreeNode* childNode3 = new SyntaxTreeNode(statement, "");
            cout << "add node: " << childNode3->getType() << " " << childNode3->getValue() << endl;
            TreeNode->addChild(childNode3);
        }
        else {
            return false;
        }
    }
    if (_lexicalOutput[_currentWordIndex]._value == "END") {
        ++_currentWordIndex;  //读取下一个字符
        SyntaxTreeNode* childNode4 = new SyntaxTreeNode(END, "END");
        cout << "add node: " << childNode4->getType() << " " << childNode4->getValue() << endl;
        TreeNode->addChild(childNode4);
        return true;
    }
    return false;
}

//<语句>→<赋值语句> | <条件语句 > | <循环语句> | <复合语句> | <空语句>
// 这里需要插入节点吗，是不是在调用的时候已经插入了
bool SyntaxAnalyzer::_STATEMENT(SyntaxTreeNode* TreeNode)
{
    SyntaxTreeNode* childNode = new SyntaxTreeNode(statement, "");
    cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
    cout << _lexicalOutput[_currentWordIndex]._type <<" "<< _lexicalOutput[_currentWordIndex]._value << endl;
    if (_lexicalOutput[_currentWordIndex]._type == "IDENTIFIER" ) {  //赋值语句
        return _ASSIGNMENTSTATEMENT(TreeNode);
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
        SyntaxTreeNode* childNode = new SyntaxTreeNode(identifier, "");
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode);
    }
    else {
        return false;
    }
    if (_lexicalOutput[_currentWordIndex]._value == ":=") {
        ++_currentWordIndex;
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(ASSIGNMENT, ":=");
        cout << "add node: " << childNode2->getType() << " " << childNode2->getValue() << endl;
        TreeNode->addChild(childNode2);
    }
    else{
        return false;
    }
    if (_EXPRESSION(TreeNode)) {
        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(expression, "");
        cout << "add node: " << childNode3->getType() << " " << childNode3->getValue() << endl;
        TreeNode->addChild(childNode3);
    }
    else {
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
        SyntaxTreeNode* childNode = new SyntaxTreeNode(ADDITIVE_OPERATOR, _lexicalOutput[_currentWordIndex]._value);
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode);
        ++_currentWordIndex;  //读取下一个字符
    }
    if (_TERM(TreeNode)) {
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(term, "");
        cout << "add node: " << childNode2->getType() << " " << childNode2->getValue() << endl;
        TreeNode->addChild(childNode2);
    }
    else {
        return false;
    }
    if (_EXPRESSION2(TreeNode)) {
        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(expression2, "");
        cout << "add node: " << childNode3->getType() << " " << childNode3->getValue() << endl;
        TreeNode->addChild(childNode3);
    }
    else {
        return false;
    }
    return true;
}
//<表达式'>:
bool SyntaxAnalyzer::_EXPRESSION2(SyntaxTreeNode* TreeNode) {

    if (_lexicalOutput[_currentWordIndex]._value == "+" ||
        _lexicalOutput[_currentWordIndex]._value == "-") {

        ++_currentWordIndex;  //读取下一个字符
        if (_ADDITIVEOPERATOR(TreeNode)) {
            SyntaxTreeNode* childNode = new SyntaxTreeNode(additive_operator, "");
            cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
            TreeNode->addChild(childNode);
        }
        else {
            return false;
        }
        if (_TERM(TreeNode)) {
            SyntaxTreeNode* childNode2 = new SyntaxTreeNode(term, "");
            cout << "add node: " << childNode2->getType() << " " << childNode2->getValue() << endl;
            TreeNode->addChild(childNode2);
        }
        else {
            return false;
        }
        if (_EXPRESSION2(TreeNode)) {
            SyntaxTreeNode* childNode3 = new SyntaxTreeNode(expression2, "");
            cout << "add node: " << childNode3->getType() << " " << childNode3->getValue() << endl;
            TreeNode->addChild(childNode3);
        }
        else {
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
    if (_FACTOR(TreeNode)) {
        SyntaxTreeNode* childNode = new SyntaxTreeNode(factor, "");
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode);
    }
    else {
        return false;
    }
    if (_TERM2(TreeNode)) {
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(term, "");
        cout << "add node: " << childNode2->getType() << " " << childNode2->getValue() << endl;
        TreeNode->addChild(childNode2);
    }
    else {
        return false;
    }
    return true;
}

bool SyntaxAnalyzer::_TERM2(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._value == "*" ||
        _lexicalOutput[_currentWordIndex]._value == "/") {
        ++_currentWordIndex;  //读取下一个字符
        if (_MULTIPLICATIVEOPERATOR(TreeNode)) {
            SyntaxTreeNode* childNode = new SyntaxTreeNode(multiplicative_operator, "");
            cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
            TreeNode->addChild(childNode);
        }
        else {
            return false;
        }
        if (_FACTOR(TreeNode)) {
            SyntaxTreeNode* childNode = new SyntaxTreeNode(factor, "");
            cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
            TreeNode->addChild(childNode);
        }
        else {
            return false;
        }
        if (_TERM2(TreeNode)) {
            SyntaxTreeNode* childNode = new SyntaxTreeNode(term2, "");
            cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
            TreeNode->addChild(childNode);
        }
        else {
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
        if (_EXPRESSION(TreeNode)) {
            SyntaxTreeNode* childNode = new SyntaxTreeNode(opening_parentheses, "");
            cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
            TreeNode->addChild(childNode);
        }
        else {
            return false;
        }
        if (_lexicalOutput[_currentWordIndex]._value == ")") {
            ++_currentWordIndex;
            SyntaxTreeNode* childNode = new SyntaxTreeNode(closing_parentheses, "");
            cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
            TreeNode->addChild(childNode);
        }
        else {
            return false;
        }
       
        return true;
    }
    return false;
}

// 15 <加法运算符> → + | -
bool SyntaxAnalyzer::_ADDITIVEOPERATOR(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._value == "+" || 
        _lexicalOutput[_currentWordIndex]._value == "-") {
        ++_currentWordIndex;
        SyntaxTreeNode* childNode = new SyntaxTreeNode(additive_operator, "");
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode);
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
        SyntaxTreeNode* childNode = new SyntaxTreeNode(multiplicative_operator, "");
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode); 
        return true;
    }
    return false;
}

//17 <条件语句>→IF <条件> THEN <语句>
bool SyntaxAnalyzer::_CONDITIONALSTATEMENT(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._value == "IF") {
        _currentWordIndex++;  //读取下一个字符
        SyntaxTreeNode* childNode = new SyntaxTreeNode(IF, "IF");
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode);
    }
    else {
        return false;
    }

    if (_CONDITION(TreeNode)) {
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(condition, "");
        cout << "add node: " << childNode2->getType() << " " << childNode2->getValue() << endl;
        TreeNode->addChild(childNode2);
    }
    else {
        return false;
    }
    //读取下一个标识符
    if (_lexicalOutput[_currentWordIndex]._value == "THEN") {
        _currentWordIndex++;  //读取下一个字符
        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(THEN, "THEN");
        cout << "add node: " << childNode3->getType() << " " << childNode3->getValue() << endl;
        TreeNode->addChild(childNode3);
    }
    else {
        return false;
    }
    if (_STATEMENT(TreeNode)) {
        SyntaxTreeNode* childNode4 = new SyntaxTreeNode(statement, "");
        cout << "add node: " << childNode4->getType() << " " << childNode4->getValue() << endl;
        TreeNode->addChild(childNode4);
    }
    else {
        return false;
    }
    return true;
}

//18 <循环语句>→WHILE <条件> DO <语句>
bool SyntaxAnalyzer::_LOOPSTATEMENT(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._value == "WHILE") {
        _currentWordIndex++;  //读取下一个字符
        SyntaxTreeNode* childNode = new SyntaxTreeNode(WHILE, "WHILE");
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode);
    }
    else {
        return false;
    }

    if (_CONDITION(TreeNode)) {
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(condition, "");
        cout << "add node: " << childNode2->getType() << " " << childNode2->getValue() << endl;
        TreeNode->addChild(childNode2);
    }
    else {
        return false;
    }
    //读取下一个标识符
    if (_lexicalOutput[_currentWordIndex]._value == "DO") {
        _currentWordIndex++;  //读取下一个字符
        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(DO, "DO");
        cout << "add node: " << childNode3->getType() << " " << childNode3->getValue() << endl;
        TreeNode->addChild(childNode3);
    }
    else {
        return false;
    }
    if (_STATEMENT(TreeNode)) {
        SyntaxTreeNode* childNode4 = new SyntaxTreeNode(statement, "");
        cout << "add node: " << childNode4->getType() << " " << childNode4->getValue() << endl;
        TreeNode->addChild(childNode4);
    }
    else {
        return false;
    }
    return true;
}

//19 <条件>→<表达式> <关系运算符> <表达式>
bool SyntaxAnalyzer::_CONDITION(SyntaxTreeNode* TreeNode)
{
    if (_EXPRESSION(TreeNode)) {
        SyntaxTreeNode* childNode = new SyntaxTreeNode(expression, "");
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode);
    }
    else {
        return false;
    }
    if (_RELATIONOPERATOR(TreeNode)) {
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(relation_operator, "");
        cout << "add node: " << childNode2->getType() << " " << childNode2->getValue() << endl;
        TreeNode->addChild(childNode2);
    }
    else {
        return false;
    }
    if (_EXPRESSION(TreeNode)) {
        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(expression, "");
        cout << "add node: " << childNode3->getType() << " " << childNode3->getValue() << endl;
        TreeNode->addChild(childNode3);
    }
    else {
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
        
        SyntaxTreeNode* childNode = new SyntaxTreeNode(RELATION_OPERATOR, _lexicalOutput[_currentWordIndex]._value);
        ++_currentWordIndex;
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode);
    }
    else {
        return false;
    }
    return true;
}

//21 <字⺟>→a | b … | x | y | z
bool SyntaxAnalyzer::_LETTER(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._value>="a" && 
        _lexicalOutput[_currentWordIndex]._value <= "z") {
        ++_currentWordIndex;
        SyntaxTreeNode* childNode = new SyntaxTreeNode(letter, "");
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode);
    }
    else {
        return false;
    }
    return false;
}

//22 <数字>→0 | 1 | … | 8 | 9
bool SyntaxAnalyzer::_NUMBER(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._value >= "0" &&
        _lexicalOutput[_currentWordIndex]._value <= "9") {
        
        SyntaxTreeNode* childNode = new SyntaxTreeNode(NUMBER, _lexicalOutput[_currentWordIndex]._value);
        ++_currentWordIndex;
        cout << "add node: " << childNode->getType() << " " << childNode->getValue() << endl;
        TreeNode->addChild(childNode);
    }
    else {
        return false;
    }
    return true;
}

//<空语句>
bool SyntaxAnalyzer::_EMPTYSTATEMENT(SyntaxTreeNode* TreeNode)
{
    return true;
}