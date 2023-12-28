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
    // 新建program_header和sub_program节点，分别插入根节点
    SyntaxTreeNode* child1 = new SyntaxTreeNode(program_header, "");
    SyntaxTreeNode* child2 = new SyntaxTreeNode(sub_program, "");
    rootNode->addChild(child1);
    rootNode->addChild(child2);
    // 分别判断两个分支
    return _PROGRAM(rootNode->getChildren()[0]) && _SUBPROG(rootNode->getChildren()[1]);
}

//<程序⾸部>→PROGRAM <标识符>
bool SyntaxAnalyzer::_PROGRAM(SyntaxTreeNode* TreeNode)
{
    //因为产生式右部有两个符号，进行递归下降
    if (_lexicalOutput[_currentWordIndex]._value == "PROGRAM") {
        ++_currentWordIndex; 
        // 添加终结符PROGRAM节点
        SyntaxTreeNode* childNode = new SyntaxTreeNode(PROGRAM, "PROGRAM"); 
        TreeNode->addChild(childNode);
    }
    else{
        return false;
    }

    // 新建子节点进行判断
    SyntaxTreeNode* childNode = new SyntaxTreeNode(identifier, "");
    TreeNode->addChild(childNode);
    // 判断标识符
    if (!_IDENTIFIER(childNode))
    {
        // 如果不是，将子节点弹出
        TreeNode->getChildren().pop_back(); 
        return false;
    }
    return true;
}

//<分程序>→[<常量说明>][<变量说明>]<语句>
bool SyntaxAnalyzer::_SUBPROG(SyntaxTreeNode* TreeNode) 
{
    if (_lexicalOutput[_currentWordIndex]._value == "CONST")
    {
        // 新建子节点进递归判断
        SyntaxTreeNode* childNode = new SyntaxTreeNode(const_declaration, "");
        TreeNode->addChild(childNode);
        // 传入子节点进行判断
        if (!_CONSTDECLARATION(childNode)) {
            // 如果不是，将子节点弹出
            TreeNode->getChildren().pop_back();
            return false;
        }
    }
    else if (_lexicalOutput[_currentWordIndex]._value == "VAR")
    {
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(var_declaration, "");
        TreeNode->addChild(childNode2);
        if (!_VARDECLARATION(childNode2)) {
            TreeNode->getChildren().pop_back();
            return false;
        }
    }

    // 将语句非标识符添加到父节点
    SyntaxTreeNode* childNode3 = new SyntaxTreeNode(statement, "");
    TreeNode->addChild(childNode3);
    if (_STATEMENT(childNode3)){
        return true;
    }
    else {
        TreeNode->getChildren().pop_back();
        return false;
    }
    
}

// <常量说明>→CONST <常量定义>{，<常量定义>}; 
bool SyntaxAnalyzer::_CONSTDECLARATION(SyntaxTreeNode* TreeNode)
{
    //因为之前在分程序已经判断了常量说明，所以这里可以认定第一个token是CONST
    SyntaxTreeNode* childNode = new SyntaxTreeNode(CONST, "CONST");
    TreeNode->addChild(childNode);

    SyntaxTreeNode* childNode1 = new SyntaxTreeNode(const_definition, "");
    TreeNode->addChild(childNode1);
    if (!_CONSTDEFINITION(childNode1)) {
        // 若不符合语法规则，则将节点弹出
        TreeNode->getChildren().pop_back();
        return false;
    }

    while (_lexicalOutput[_currentWordIndex]._value == ",") {
        ++_currentWordIndex;    //读取下一个字符
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(COMMA, ",");
        TreeNode->addChild(childNode2);
        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(const_definition, "");
        TreeNode->addChild(childNode3);
        if (!_CONSTDEFINITION(childNode2)){
            // 若不符合语法规则，则将节点弹出
            TreeNode->getChildren().pop_back();
            return false;
        }
    }
    if (_lexicalOutput[_currentWordIndex]._value == ";") {
        ++_currentWordIndex;
        SyntaxTreeNode* childNode = new SyntaxTreeNode(SEMICOLON, ";");
        TreeNode->addChild(childNode);
    }
    else {
        return false;
    }
    return true;
}

//<常量定义>→<标识符>: = <⽆符号整数>
bool SyntaxAnalyzer::_CONSTDEFINITION(SyntaxTreeNode* TreeNode)
{
    SyntaxTreeNode* childNode = new SyntaxTreeNode(identifier, "");
    TreeNode->addChild(childNode);
    if (!_IDENTIFIER(childNode)) {
        // 若不符合语法规则，则将节点弹出
        TreeNode->getChildren().pop_back();
        return false;
    }
    if (_lexicalOutput[_currentWordIndex]._value == ":=")
    {
        ++_currentWordIndex; 
        SyntaxTreeNode* childNode = new SyntaxTreeNode(ASSIGNMENT, ":=");
        TreeNode->addChild(childNode);
    }
    else
    {
        return false;
    }

    // 新建unsigned_integer非标识符，插入父节点之后进行判断
    SyntaxTreeNode* childNode2 = new SyntaxTreeNode(unsigned_integer, "");
    TreeNode->addChild(childNode2);
    if (!_UNSIGNEDINTEGER(childNode2))
    {
        // 若不符合语法规则，则将节点弹出
        TreeNode->getChildren().pop_back();
        return false;
    }
    return true;
    
}

//<⽆符号整数>→<数字>{ <数字> }
//这里有个坑是第一位为0的情况
bool SyntaxAnalyzer::_UNSIGNEDINTEGER(SyntaxTreeNode* TreeNode)
{
    SyntaxTreeNode* childNode = new SyntaxTreeNode(NUMBER, TreeNode->getValue());
    TreeNode->addChild(childNode);
    if (!_NUMBER(childNode)) {
        // 若不符合语法规则，则将节点弹出
        TreeNode->getChildren().pop_back();
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
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(NUMBER, TreeNode->getValue());
        TreeNode->addChild(childNode2);
        if (!_NUMBER(childNode2)) {
            // 若不符合语法规则，则将节点弹出
            TreeNode->getChildren().pop_back();
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
    TreeNode->addChild(childNode);


    SyntaxTreeNode* childNode1 = new SyntaxTreeNode(identifier, "");
    TreeNode->addChild(childNode1);
    if (!_IDENTIFIER(childNode1)) {
        // 若不符合语法规则，则将节点弹出
        TreeNode->getChildren().pop_back();
        return false;
    }
    while (_lexicalOutput[_currentWordIndex]._value == ",") {
        ++_currentWordIndex;
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(COMMA, ","); 
        TreeNode->addChild(childNode2);

        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(identifier, "");
        TreeNode->addChild(childNode3);
        if (!_IDENTIFIER(childNode3))
        {
            // 若不符合语法规则，则将节点弹出
            TreeNode->getChildren().pop_back();
            return false;
        }
    }


    if (_lexicalOutput[_currentWordIndex]._value == ";") {
        ++_currentWordIndex;
        SyntaxTreeNode* childNode = new SyntaxTreeNode(SEMICOLON, ";");
        TreeNode->addChild(childNode);
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
        // 将标识符作为节点添加到父节点当中
        SyntaxTreeNode* childNode = new SyntaxTreeNode(IDENTIFIER, _lexicalOutput[_currentWordIndex]._value);
        TreeNode->addChild(childNode);
        // 读取下一个字符
        ++_currentWordIndex; 
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
        SyntaxTreeNode* childNode = new SyntaxTreeNode(BEGIN, "BEGIN");
        TreeNode->addChild(childNode);
    }

    SyntaxTreeNode* childNode2 = new SyntaxTreeNode(statement, "");
    TreeNode->addChild(childNode2);
    //判断语句
    if (!_STATEMENT(childNode2))
    {
        // 若不符合语法规则，则将节点弹出
        TreeNode->getChildren().pop_back();
        return false;
    }

    while (_lexicalOutput[_currentWordIndex]._value == ";") {
        SyntaxTreeNode* childNode = new SyntaxTreeNode(SEMICOLON, ";");
        TreeNode->addChild(childNode);
        ++_currentWordIndex;  //读取下一个字符

        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(statement, "");
        TreeNode->addChild(childNode3);
        if (!_STATEMENT(childNode3))
        {
            // 若不符合语法规则，则将节点弹出
            TreeNode->getChildren().pop_back();
            return false;
        }
    }
    if (_lexicalOutput[_currentWordIndex]._value == "END") {
        ++_currentWordIndex;  //读取下一个字符
        SyntaxTreeNode* childNode4 = new SyntaxTreeNode(END, "END");
        TreeNode->addChild(childNode4);
        return true;
    }
    return false;
}

//<语句>→<赋值语句> | <条件语句 > | <循环语句> | <复合语句> | <空语句>
bool SyntaxAnalyzer::_STATEMENT(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._type == "IDENTIFIER" ) { //赋值语句
        SyntaxTreeNode* childNode1 = new SyntaxTreeNode(assignment_statement, "assignment_statement");
        TreeNode->addChild(childNode1);
        if (!_ASSIGNMENTSTATEMENT(childNode1)) {
            TreeNode->getChildren().pop_back();
            return false;
        }
        return true;
    } 
    else if (_lexicalOutput[_currentWordIndex]._value == "IF") {  //条件语句
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(conditional_statement, "conditional_statement");
        TreeNode->addChild(childNode2);
        if (!_CONDITIONALSTATEMENT(childNode2)) {
            TreeNode->getChildren().pop_back();
            return false;
        }
        return true;
    }
    else if (_lexicalOutput[_currentWordIndex]._value == "WHILE") { //循环语句
        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(loop_statement, "loop_statement");
        TreeNode->addChild(childNode3);
        if (!_LOOPSTATEMENT(childNode3)) {
            TreeNode->getChildren().pop_back();
            return false;
        }
        return true;

    }
    else if (_lexicalOutput[_currentWordIndex]._value == "BEGIN") {  //复合语句
        SyntaxTreeNode* childNode4 = new SyntaxTreeNode(compound_statement, "compound_statement");
        TreeNode->addChild(childNode4);
        if (!_COMPOUNDSTATEMENT(childNode4)) {
            TreeNode->getChildren().pop_back();
            return false;
        }
        return true;
    }
    else {
        return _EMPTYSTATEMENT(TreeNode);   //空语句
    }
}

//<赋值语句>→<标识符>: = <表达式>
bool SyntaxAnalyzer::_ASSIGNMENTSTATEMENT(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._type == "IDENTIFIER") {
        
        SyntaxTreeNode* childNode = new SyntaxTreeNode(IDENTIFIER, _lexicalOutput[_currentWordIndex]._value);
        TreeNode->addChild(childNode);
        ++_currentWordIndex;
    }
    else {
        return false;
    }
    if (_lexicalOutput[_currentWordIndex]._value == ":=") {
        ++_currentWordIndex;
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(ASSIGNMENT, ":=");
        TreeNode->addChild(childNode2);
    }
    else{
        return false;
    }

    SyntaxTreeNode* childNode3 = new SyntaxTreeNode(expression, "");
    TreeNode->addChild(childNode3);
    if (!_EXPRESSION(childNode3)) {
        TreeNode->getChildren().pop_back();
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
        TreeNode->addChild(childNode);
        ++_currentWordIndex;  //读取下一个字符
    }

    SyntaxTreeNode* childNode2 = new SyntaxTreeNode(term, "");
    TreeNode->addChild(childNode2);
    if (!_TERM(childNode2)) {
        TreeNode->getChildren().pop_back();
        return false;
    }

    SyntaxTreeNode* childNode3 = new SyntaxTreeNode(expression2, "");
    TreeNode->addChild(childNode3);
    if (!_EXPRESSION2(childNode3)) {
        // 若不符合语法规则，则将子节点弹出，并返回错误
        TreeNode->getChildren().pop_back();
        return false;
    }
    return true;
}
//<表达式'>:
bool SyntaxAnalyzer::_EXPRESSION2(SyntaxTreeNode* TreeNode) {

    if (_lexicalOutput[_currentWordIndex]._value == "+" ||
        _lexicalOutput[_currentWordIndex]._value == "-") {

        SyntaxTreeNode* childNode = new SyntaxTreeNode(additive_operator, "");
        TreeNode->addChild(childNode);
        if (!_ADDITIVEOPERATOR(childNode)) {
            TreeNode->getChildren().pop_back();
            return false;
        }

        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(term, "");
        TreeNode->addChild(childNode2);
        if (!_TERM(childNode2)) {
            TreeNode->getChildren().pop_back();
            return false;
        }

        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(expression2, "");
        TreeNode->addChild(childNode3);
        if (!_EXPRESSION2(childNode3)) {
            TreeNode->getChildren().pop_back();
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
    SyntaxTreeNode* childNode = new SyntaxTreeNode(factor, "");
    TreeNode->addChild(childNode);
    if (!_FACTOR(childNode)) {
        TreeNode->getChildren().pop_back();
        return false;
    }

    SyntaxTreeNode* childNode2 = new SyntaxTreeNode(term2, "");
    TreeNode->addChild(childNode2);
    if (!_TERM2(childNode2)) {
        TreeNode->getChildren().pop_back();
        return false;
    }
    return true;
}

bool SyntaxAnalyzer::_TERM2(SyntaxTreeNode* TreeNode)
{
    if (_lexicalOutput[_currentWordIndex]._value == "*" ||
        _lexicalOutput[_currentWordIndex]._value == "/") {

        SyntaxTreeNode* childNode = new SyntaxTreeNode(multiplicative_operator, "");
        TreeNode->addChild(childNode);
        if (!_MULTIPLICATIVEOPERATOR(childNode)) {
            TreeNode->getChildren().pop_back();
            return false;
        }

        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(factor, "");
        TreeNode->addChild(childNode2);
        if (!_FACTOR(childNode2)) {
            TreeNode->getChildren().pop_back();
            return false;
        }

        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(term2, "");
        TreeNode->addChild(childNode3);
        if (!_TERM2(childNode3)) {
            TreeNode->getChildren().pop_back();
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
        SyntaxTreeNode* childNode = new SyntaxTreeNode(identifier, "");
        TreeNode->addChild(childNode);
        if (!_IDENTIFIER(childNode)) {
            TreeNode->getChildren().pop_back();
            return false;
        }
        return true;
    }
    else if (_lexicalOutput[_currentWordIndex]._type == "NUMBER") {
        SyntaxTreeNode* childNode = new SyntaxTreeNode(unsigned_integer, "");
        TreeNode->addChild(childNode);
        if (!_UNSIGNEDINTEGER(childNode)) {
            TreeNode->getChildren().pop_back();
            return false;
        }
        return true;
    }
    else if (_lexicalOutput[_currentWordIndex]._value == "(") {
        ++_currentWordIndex;       //读取下一个token

        SyntaxTreeNode* childNode = new SyntaxTreeNode(opening_parentheses, "");
        TreeNode->addChild(childNode);
        if (!_EXPRESSION(childNode)) {
            TreeNode->getChildren().pop_back();
            return false;
        }

        if (_lexicalOutput[_currentWordIndex]._value == ")") {
            ++_currentWordIndex;
            SyntaxTreeNode* childNode = new SyntaxTreeNode(closing_parentheses, "");
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
        SyntaxTreeNode* childNode = new SyntaxTreeNode(ADDITIVE_OPERATOR, _lexicalOutput[_currentWordIndex]._value);
        TreeNode->addChild(childNode);
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
        
        SyntaxTreeNode* childNode = new SyntaxTreeNode(MULTIPLICATIVE_OPERATOR, _lexicalOutput[_currentWordIndex]._value);
        TreeNode->addChild(childNode); 
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
        SyntaxTreeNode* childNode = new SyntaxTreeNode(IF, "IF");
        TreeNode->addChild(childNode);
    }
    else {
        return false;
    }

    SyntaxTreeNode* childNode2 = new SyntaxTreeNode(condition, "");
    TreeNode->addChild(childNode2);
    if (!_CONDITION(childNode2)) {
        TreeNode->getChildren().pop_back();
        return false;
    }
    //读取下一个标识符
    if (_lexicalOutput[_currentWordIndex]._value == "THEN") {
        _currentWordIndex++;  //读取下一个字符
        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(THEN, "THEN");
        TreeNode->addChild(childNode3);
    }
    else {
        return false;
    }

    SyntaxTreeNode* childNode4 = new SyntaxTreeNode(statement, "");
    TreeNode->addChild(childNode4);
    if (!_STATEMENT(childNode4)) {
        TreeNode->getChildren().pop_back();
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
        TreeNode->addChild(childNode);
    }
    else {
        return false;
    }

    SyntaxTreeNode* childNode2 = new SyntaxTreeNode(condition, "");
    TreeNode->addChild(childNode2);
    if (!_CONDITION(childNode2)) {
        TreeNode->getChildren().pop_back();
        return false;
    }

    //读取下一个标识符
    if (_lexicalOutput[_currentWordIndex]._value == "DO") {
        _currentWordIndex++;  //读取下一个字符
        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(DO, "DO");
        TreeNode->addChild(childNode3);
    }
    else {
        return false;
    }

    SyntaxTreeNode* childNode4 = new SyntaxTreeNode(statement, "");
    TreeNode->addChild(childNode4);
    if (!_STATEMENT(childNode4)) {
        TreeNode->getChildren().pop_back();
        return false;
    }
    return true;
}

//19 <条件>→<表达式> <关系运算符> <表达式>
bool SyntaxAnalyzer::_CONDITION(SyntaxTreeNode* TreeNode)
{
    SyntaxTreeNode* childNode = new SyntaxTreeNode(expression, "");
    TreeNode->addChild(childNode);
    if (!_EXPRESSION(childNode)) {
        TreeNode->getChildren().pop_back();
        return false;
    }

    SyntaxTreeNode* childNode2 = new SyntaxTreeNode(relation_operator, "");
    TreeNode->addChild(childNode2);
    if (!_RELATIONOPERATOR(childNode2)) {
        TreeNode->getChildren().pop_back();
        return false;
    }

    SyntaxTreeNode* childNode3 = new SyntaxTreeNode(expression, "");
    TreeNode->addChild(childNode3);
    if (!_EXPRESSION(childNode3)) {
        TreeNode->getChildren().pop_back();
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
    SyntaxTreeNode* childNode = new SyntaxTreeNode(EMPTY_STATEMENT,"empty");
    TreeNode->addChild(childNode);
    return true;
}