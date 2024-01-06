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
#include <vector>
#include "syntax.h"
using namespace std;

const string ERROR = "ERROR";
int LABEL = 0;
int instuctioncount = 1;
int labelcount = 0;

int line = 0;
vector<int> labelValues;
vector<string> codeline;

void emit(string code) {
    codeline.push_back(code);
    instuctioncount++;
    line++;
}

int newLabel() {
    labelValues.push_back(NULL);  // 初始时标签位置未知
    return labelcount++;
}

void SyntaxAnalyzer::printcode() {
    ofstream outFile;
    outFile.open("ThreeMidCode.txt");
    if (!outFile.is_open()) {
        cerr << "Unable to open file";
        exit(1); // 调用系统来停止程序运行
    }
    for (int i = 0; i < codeline.size(); i++) {
        string line = codeline[i];
        // 替换所有的标签引用
        for (int j = 0; j < labelValues.size(); j++) {
            if (labelValues[j] != -1) {
                size_t found = line.find("$" + to_string(j));
                while (found != string::npos) {
                    line.replace(found, 2, to_string(labelValues[j]));
                    found = line.find("$" + to_string(j), found + 1);
                }
            }
        }
        cout << i << ": " << line << endl;
        outFile << i << ": " << line << "\n";
    }
    outFile.close();
    this->_stable.saveSymbol();
}

//过程是遍历词法分析的输出，第一个分析的节点默认分析为PROGRAM
string SyntaxAnalyzer::syntaxanalyze(SyntaxTreeNode* rootNode, NodeType nodetype) {

    this->LexicalMachine.readFile();
    this->LexicalMachine.Analyze();
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
	if ((_PROGRAM(rootNode->getChildren()[0]) != ERROR) && (_SUBPROG(rootNode->getChildren()[1]) != ERROR)) {
		return "";
	}
	return ERROR;
}

//<程序⾸部>→PROGRAM <标识符>
string SyntaxAnalyzer::_PROGRAM(SyntaxTreeNode* TreeNode)
{
    //因为产生式右部有两个符号，进行递归下降
    if (this->LexicalMachine.getToken()._value == "PROGRAM") {
        this->LexicalMachine.Analyze();
        // 添加终结符PROGRAM节点
        SyntaxTreeNode* childNode = new SyntaxTreeNode(PROGRAM, "PROGRAM");
        TreeNode->addChild(childNode);
    }
    else {
        cout << "第" << LexicalMachine.getLine() << "行出现语法错误，程序首部不是PROGRAM开头！" << endl;
        return ERROR;
    }

    // 新建子节点进行判断
    SyntaxTreeNode* childNode = new SyntaxTreeNode(identifier, "");
    TreeNode->addChild(childNode);
    // 判断标识符
    string identifier0 = _IDENTIFIER(childNode);
    if (identifier0 == ERROR)
    {
        // 如果不是，将子节点弹出
        TreeNode->getChildren().pop_back();
        return ERROR;
    }
    return "";
}

//<分程序>→[<常量说明>][<变量说明>]<语句>
string SyntaxAnalyzer::_SUBPROG(SyntaxTreeNode* TreeNode)
{
    if (this->LexicalMachine.getToken()._value == "CONST")
    {
        SyntaxTreeNode* childNode = new SyntaxTreeNode(const_declaration, "");
        TreeNode->addChild(childNode);
        string constdeclartion = _CONSTDECLARATION(childNode);
        if (constdeclartion == ERROR) {
            // 如果不是，将子节点弹出
            TreeNode->getChildren().pop_back();
            return ERROR;
        }
    }
    if (this->LexicalMachine.getToken()._value == "VAR")
    {
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(var_declaration, "");
        TreeNode->addChild(childNode2);
        string vardeclaration = _VARDECLARATION(childNode2);
        if (vardeclaration == ERROR) {
            TreeNode->getChildren().pop_back();
            return ERROR;
        }
    }

    // 将语句非标识符添加到父节点
    SyntaxTreeNode* childNode3 = new SyntaxTreeNode(statement, "");
    TreeNode->addChild(childNode3);
    string statement = _STATEMENT(childNode3);
    if (statement != ERROR) {
        return "";
    }
    else {
        TreeNode->getChildren().pop_back();
        return ERROR;
    }

}

// <常量说明>→CONST <常量定义>{，<常量定义>}; 
string SyntaxAnalyzer::_CONSTDECLARATION(SyntaxTreeNode* TreeNode)
{
    this->LexicalMachine.Analyze();
    //因为之前在分程序已经判断了常量说明，所以这里可以认定第一个token是CONST
    SyntaxTreeNode* childNode = new SyntaxTreeNode(CONST, "CONST");
    TreeNode->addChild(childNode);

    SyntaxTreeNode* childNode1 = new SyntaxTreeNode(const_definition, "");
    TreeNode->addChild(childNode1);
    string constdefinition = _CONSTDEFINITION(childNode1);
    if (constdefinition == ERROR) {
        // 若不符合语法规则，则将节点弹出
       
        TreeNode->getChildren().pop_back();
        return ERROR;
    }
    //S_stable.addEntry(this->LexicalMachine.getToken(0)._value, "CONST");
    while (this->LexicalMachine.getToken()._value == ",") {

        this->LexicalMachine.Analyze();    //读取下一个字符
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(COMMA, ",");
        TreeNode->addChild(childNode2);
        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(const_definition, "");
        TreeNode->addChild(childNode3);
        string constdefinition = _CONSTDEFINITION(childNode3);
        if (constdefinition == ERROR) {
            // 若不符合语法规则，则将节点弹出
            TreeNode->getChildren().pop_back();
            return ERROR;
        }
    }
    if (this->LexicalMachine.getToken()._value == ";") {
        this->LexicalMachine.Analyze();
        SyntaxTreeNode* childNode = new SyntaxTreeNode(SEMICOLON, ";");
        TreeNode->addChild(childNode);
    }
    else {
        cout << "第" << LexicalMachine.getLine() << "行出现语法错误，不符合常量说明规则！缺少分号';" << endl;
        return ERROR;
    }
    return "";
}

//<常量定义>→<标识符>: = <⽆符号整数>
string SyntaxAnalyzer::_CONSTDEFINITION(SyntaxTreeNode* TreeNode)
{
    SyntaxTreeNode* childNode = new SyntaxTreeNode(identifier, "");
    TreeNode->addChild(childNode);
    string _identifier = _IDENTIFIER(childNode);
    if (_identifier == ERROR) {
        // 若不符合语法规则，则将节点弹出
        TreeNode->getChildren().pop_back();
        return ERROR;
    }
    if (_stable.findEntry(this->LexicalMachine.getToken(0)._value) !=nullptr) {
        cout << "第" << LexicalMachine.getLine() << "行常量定义标识符重定义" << endl;
        return ERROR;
    }
    _stable.addEntry(this->LexicalMachine.getToken(0)._value, "CONST");
    string temp = " = ";
    if (this->LexicalMachine.getToken()._value == ":=")
    {
        this->LexicalMachine.Analyze();
        SyntaxTreeNode* childNode = new SyntaxTreeNode(ASSIGNMENT, ":=");
        TreeNode->addChild(childNode);
    }
    else
    {
        cout << "第" << LexicalMachine.getLine() << "行出现语法错误，不符合常量定义规则！缺少赋值号‘:=’" << endl;
        return ERROR;
    }

    // 新建unsigned_integer非标识符，插入父节点之后进行判断
    SyntaxTreeNode* childNode2 = new SyntaxTreeNode(unsigned_integer, "");
    TreeNode->addChild(childNode2);
    string unsignedinteger = _UNSIGNEDINTEGER(childNode2);
    if (unsignedinteger == ERROR)
    {
        // 若不符合语法规则，则将节点弹出
        TreeNode->getChildren().pop_back();
        cout << "第" << LexicalMachine.getLine() << "行出现语法错误，不符合常量定义规则！不是无符号整数’" << endl;
        return ERROR;
    }
    emit(_identifier + temp + unsignedinteger);
    return "";

}

//<⽆符号整数>→<数字>{ <数字> }
//这里有个坑是第一位为0的情况
string SyntaxAnalyzer::_UNSIGNEDINTEGER(SyntaxTreeNode* TreeNode)
{
    SyntaxTreeNode* childNode = new SyntaxTreeNode(NUMBER, TreeNode->getValue());
    TreeNode->addChild(childNode);
    string temp = this->LexicalMachine.getToken()._value;
    if (_NUMBER(childNode) == ERROR) {
        // 若不符合语法规则，则将节点弹出
        TreeNode->getChildren().pop_back();
        return ERROR;
    }
    //除去首部为0的多数字情形，此时默认为错误的
    if (this->LexicalMachine.getToken(0)._value == "0" &&
        this->LexicalMachine.getToken()._type == "NUMBER")
    {
        return ERROR;
    }
    while (this->LexicalMachine.getToken()._type == "NUMBER")
    {
        this->LexicalMachine.Analyze();  //读取下一个字符
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(NUMBER, TreeNode->getValue());
        TreeNode->addChild(childNode2);
        if (_NUMBER(childNode2) == ERROR) {
            // 若不符合语法规则，则将节点弹出
            TreeNode->getChildren().pop_back();
            return ERROR;
        }
    }
    return temp;
}

//<变量说明>→VAR<标识符>{，<标识符>};
string SyntaxAnalyzer::_VARDECLARATION(SyntaxTreeNode* TreeNode)
{
    this->LexicalMachine.Analyze();
    //因为之前在分程序已经判断了变量说明，所以这里可以认定第一个token是VAR
    SyntaxTreeNode* childNode = new SyntaxTreeNode(VAR_DECLARATION, "VAR");
    TreeNode->addChild(childNode);


    SyntaxTreeNode* childNode1 = new SyntaxTreeNode(identifier, "");
    TreeNode->addChild(childNode1);
    string _identifier = _IDENTIFIER(childNode1);
    if (_identifier == ERROR) {
        // 若不符合语法规则，则将节点弹出
        TreeNode->getChildren().pop_back();
        return ERROR;
    }
    if (_stable.findEntry(this->LexicalMachine.getToken(0)._value) != nullptr) {
        cout << "第" << LexicalMachine.getLine() << "行变量说明标识符重定义，与常量定义重名" << endl;
        return ERROR;
    }
    _stable.addEntry(this->LexicalMachine.getToken(0)._value, "VAR");
    while (this->LexicalMachine.getToken()._value == ",") {
        this->LexicalMachine.Analyze();
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(COMMA, ",");
        TreeNode->addChild(childNode2);

        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(identifier, "");
        TreeNode->addChild(childNode3);
        string _identifier1 = _IDENTIFIER(childNode1);
        if (_identifier1 == ERROR)
        {
            // 若不符合语法规则，则将节点弹出
            TreeNode->getChildren().pop_back();
            return ERROR;
        }
        if (_stable.findEntry(this->LexicalMachine.getToken(0)._value) != nullptr) {
            cout << "第" << LexicalMachine.getLine() << "行变量说明标识符重定义" << endl;
            return ERROR;
        }
        _stable.addEntry(this->LexicalMachine.getToken(0)._value, "VAR");
    }


    if (this->LexicalMachine.getToken()._value == ";") {
        this->LexicalMachine.Analyze();
        SyntaxTreeNode* childNode = new SyntaxTreeNode(SEMICOLON, ";");
        TreeNode->addChild(childNode);
    }
    else {
        cout << "第" << LexicalMachine.getLine() << "行出现语法错误，不符合变量说明规则！尾部缺少‘;’结束符" << endl;
        return ERROR;
    }
    return "";
}

//<标识符>→<字⺟>{<字⺟> | <数字>}
string SyntaxAnalyzer::_IDENTIFIER(SyntaxTreeNode* TreeNode)
{
    string temp = this->LexicalMachine.getToken()._value;
    if (this->LexicalMachine.getToken()._type == "IDENTIFIER") {
        // 将标识符作为节点添加到父节点当中
        SyntaxTreeNode* childNode = new SyntaxTreeNode(IDENTIFIER, this->LexicalMachine.getToken()._value);
        TreeNode->addChild(childNode);
        // 读取下一个字符
        this->LexicalMachine.Analyze();
        return temp;
    }
    cout << "第" << LexicalMachine.getLine() << "行出现语法错误，不是标识符！" << endl;
    return ERROR;
}

//<复合语句>→BEGIN <语句>{; <语句>} END
// 这个函数有问题
string SyntaxAnalyzer::_COMPOUNDSTATEMENT(SyntaxTreeNode* TreeNode)
{
    if (this->LexicalMachine.getToken()._value == "BEGIN") {
        this->LexicalMachine.Analyze();  //读取下一个字符
        SyntaxTreeNode* childNode = new SyntaxTreeNode(BEGIN, "BEGIN");
        TreeNode->addChild(childNode);
    }

    SyntaxTreeNode* childNode2 = new SyntaxTreeNode(statement, "");
    TreeNode->addChild(childNode2);
    //判断语句

    string _statement = _STATEMENT(childNode2);
    if (_statement == ERROR)
    {
        // 若不符合语法规则，则将节点弹出
        TreeNode->getChildren().pop_back();
        return ERROR;
    }

    while (this->LexicalMachine.getToken()._value == ";") {
        SyntaxTreeNode* childNode = new SyntaxTreeNode(SEMICOLON, ";");
        TreeNode->addChild(childNode);
        this->LexicalMachine.Analyze();  //读取下一个字符

        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(statement, "");
        TreeNode->addChild(childNode3);
        string _statement1 = _STATEMENT(childNode3);
        if (_statement1 == ERROR)
        {
            // 若不符合语法规则，则将节点弹出
            TreeNode->getChildren().pop_back();
            return ERROR;
        }
    }
    if (this->LexicalMachine.getToken()._value == "END") {
        this->LexicalMachine.Analyze();  //读取下一个字符
        SyntaxTreeNode* childNode4 = new SyntaxTreeNode(END, "END");
        TreeNode->addChild(childNode4);
        return "";
    }
    return ERROR;
}

//<语句>→<赋值语句> | <条件语句 > | <循环语句> | <复合语句> | <空语句>
string SyntaxAnalyzer::_STATEMENT(SyntaxTreeNode* TreeNode)
{
    if (this->LexicalMachine.getToken()._type == "IDENTIFIER") { //赋值语句
        SyntaxTreeNode* childNode1 = new SyntaxTreeNode(assignment_statement, "assignment_statement");
        TreeNode->addChild(childNode1);
        string assignmentstatement = _ASSIGNMENTSTATEMENT(childNode1);
        if (assignmentstatement == ERROR) {
            TreeNode->getChildren().pop_back();
            return ERROR;
        }
        return "";
    }
    else if (this->LexicalMachine.getToken()._value == "IF") {  //条件语句
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(conditional_statement, "conditional_statement");
        TreeNode->addChild(childNode2);
        string conditionalstatement = _CONDITIONALSTATEMENT(childNode2);
        if (conditionalstatement == ERROR) {
            TreeNode->getChildren().pop_back();
            return ERROR;
        }
        return "";
    }
    else if (this->LexicalMachine.getToken()._value == "WHILE") { //循环语句
        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(loop_statement, "loop_statement");
        TreeNode->addChild(childNode3);

        string loopstatement = _LOOPSTATEMENT(childNode3);
        if (loopstatement == ERROR) {
            TreeNode->getChildren().pop_back();
            return ERROR;
        }
        return "";
    }
    else if (this->LexicalMachine.getToken()._value == "BEGIN") {  //复合语句
        SyntaxTreeNode* childNode4 = new SyntaxTreeNode(compound_statement, "compound_statement");
        TreeNode->addChild(childNode4);
        string compoundstatement = _COMPOUNDSTATEMENT(childNode4);
        if (compoundstatement == ERROR) {
            TreeNode->getChildren().pop_back();
            return ERROR;
        }
        return "";
    }
    else {
        return _EMPTYSTATEMENT(TreeNode);   //空语句
    }
}

//<赋值语句>→<标识符>: = <表达式>
string SyntaxAnalyzer::_ASSIGNMENTSTATEMENT(SyntaxTreeNode* TreeNode)
{
    SyntaxTreeNode* childNode = new SyntaxTreeNode(IDENTIFIER, "");
    string temp = this->LexicalMachine.getToken()._value;
    if (this->LexicalMachine.getToken()._type == "IDENTIFIER") {

        // 获取标识符的值
        string identifierValue = this->LexicalMachine.getToken()._value;

        // 在符号表中查询标识符是否存在
        SymbolTableEntry* entry = _stable.findEntry(identifierValue);

        // 如果标识符不存在于符号表中，输出错误信息或采取相应的处理
        if (entry == nullptr) {
            cerr << "发生错误:变量'" << identifierValue << "'不在符号表中" << endl;
            return ERROR;
        }
        childNode->setValue(this->LexicalMachine.getToken()._value);
        TreeNode->addChild(childNode);
        this->LexicalMachine.Analyze();
    }
    else {
        return ERROR;
    }
    SyntaxTreeNode* childNode2 = new SyntaxTreeNode(ASSIGNMENT, "");
    if (this->LexicalMachine.getToken()._value == ":=") {
        this->LexicalMachine.Analyze();
        childNode2->setValue(":=");
        TreeNode->addChild(childNode2);
    }
    else {
        return ERROR;
    }

    SyntaxTreeNode* childNode3 = new SyntaxTreeNode(expression, "");
    TreeNode->addChild(childNode3);
    string expression2 = _EXPRESSION(childNode3);
    if (expression2 == ERROR) {
        TreeNode->getChildren().pop_back();
        return ERROR;
    }
    emit(temp + " = " + expression2);
    return expression2;
}

/*-----------这里需要消除左递归---------------*/
//<表达式>→[+| -]<项> | <表达式> <加法运算符> <项>


//<表达式>→[+| -]<项><表达式'>   
//<表达式'>→<加法运算符> <项><表达式'> | <空语句>
//<表达式>:
string SyntaxAnalyzer::_EXPRESSION(SyntaxTreeNode* TreeNode)
{
    bool isaddorsub = false;
    string op1;
    if (this->LexicalMachine.getToken()._value == "+" ||
        this->LexicalMachine.getToken()._value == "-") {
        op1 = this->LexicalMachine.getToken()._value;
        isaddorsub = true;
        SyntaxTreeNode* childNode = new SyntaxTreeNode(ADDITIVE_OPERATOR, this->LexicalMachine.getToken()._value);
        TreeNode->addChild(childNode);
        this->LexicalMachine.Analyze();  //读取下一个字符
    }

    SyntaxTreeNode* childNode2 = new SyntaxTreeNode(term, "");
    TreeNode->addChild(childNode2);

    string term = _TERM(childNode2);
    if (term == ERROR) {
        TreeNode->getChildren().pop_back();
        return ERROR;
    }
    string temp1 = term;
    if (isaddorsub) {
        temp1 = allocateTemporary();
        _stable.addEntry(lastTemporary(), "MID");
        emit(temp1 + " = " + op1 + term);
    }
    SyntaxTreeNode* childNode3 = new SyntaxTreeNode(expression2, "");
    TreeNode->addChild(childNode3);

    string expression2 = _EXPRESSION2(childNode3);
    //string temp2 = allocateTemporary();
    if (expression2 == ERROR) {
        // 若不符合语法规则，则将子节点弹出，并返回错误
        TreeNode->getChildren().pop_back();
        return ERROR;
    }
    else if (expression2 == "") {
        return temp1;
    }
    else {
        //emit(temp2 + "=" + temp1 + op1 + expression2);
        return expression2;
    }
}
//<表达式'>:
string SyntaxAnalyzer::_EXPRESSION2(SyntaxTreeNode* TreeNode) {

    string temp = this->LexicalMachine.getToken(0)._value;
    if (this->LexicalMachine.getToken()._value == "+" ||
        this->LexicalMachine.getToken()._value == "-") {

        SyntaxTreeNode* childNode = new SyntaxTreeNode(additive_operator, "");
        TreeNode->addChild(childNode);
        string additiveoperator = _ADDITIVEOPERATOR(childNode);
        if (additiveoperator == ERROR) {
            TreeNode->getChildren().pop_back();
            return ERROR;
        }
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(term, "");
        TreeNode->addChild(childNode2);
        string term = _TERM(childNode2);
        if (term == ERROR) {
            TreeNode->getChildren().pop_back();
            return ERROR;
        }
        string temp1 = allocateTemporary();
        _stable.addEntry(lastTemporary(), "MID");
        string currenttemp = lastTemporary();
        emit(temp1 + " = " + temp + additiveoperator + term);
        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(expression2, "");
        TreeNode->addChild(childNode3);
        string expression2 = _EXPRESSION2(childNode3);
        if (expression2 == ERROR) {
            TreeNode->getChildren().pop_back();
            return ERROR;
        }
        return temp1;
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
string SyntaxAnalyzer::_TERM(SyntaxTreeNode* TreeNode)
{
    SyntaxTreeNode* childNode = new SyntaxTreeNode(factor, "");
    TreeNode->addChild(childNode);
    string factor = _FACTOR(childNode);
    if (factor == ERROR) {
        TreeNode->getChildren().pop_back();
        return ERROR;
    }
    string temp1 = factor;
    SyntaxTreeNode* childNode2 = new SyntaxTreeNode(term2, "");
    TreeNode->addChild(childNode2);
    string term2 = _TERM2(childNode2);
    if (term2 == ERROR) {
        TreeNode->getChildren().pop_back();
        return ERROR;
    }
    else if (term2 == "") {
        return temp1;
    }
    else {
        return term2;
    }
}

string SyntaxAnalyzer::_TERM2(SyntaxTreeNode* TreeNode)
{
    string temp = this->LexicalMachine.getToken(0)._value;
    if (this->LexicalMachine.getToken()._value == "*" ||
        this->LexicalMachine.getToken()._value == "/") {

        SyntaxTreeNode* childNode = new SyntaxTreeNode(multiplicative_operator, "");
        TreeNode->addChild(childNode);
        string multiplicativeoperator = _MULTIPLICATIVEOPERATOR(childNode);
        if (multiplicativeoperator == ERROR) {
            TreeNode->getChildren().pop_back();
            return ERROR;
        }
        SyntaxTreeNode* childNode2 = new SyntaxTreeNode(factor, "");
        TreeNode->addChild(childNode2);
        string factor = _FACTOR(childNode2);
        if (factor == ERROR) {
            TreeNode->getChildren().pop_back();
            return ERROR;
        }

        string temp1 = allocateTemporary();
        _stable.addEntry(lastTemporary(), "MID");
        string currenttemp = lastTemporary();
        emit(temp1 + " = " + temp + multiplicativeoperator + factor);
        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(term2, "");
        TreeNode->addChild(childNode3);
        string term2 = _TERM2(childNode3);
        if (term2 == ERROR) {
            TreeNode->getChildren().pop_back();
            return ERROR;
        }
        return temp1;
    }
    else
    {
        return _EMPTYSTATEMENT(TreeNode);
    }

}


//<因⼦>→<标识符> | <⽆符号整数> | (<表达式>)
string SyntaxAnalyzer::_FACTOR(SyntaxTreeNode* TreeNode)
{
    if (this->LexicalMachine.getToken()._type == "IDENTIFIER") {

        // 获取标识符的值
        string identifierValue = this->LexicalMachine.getToken()._value;

        // 在符号表中查询标识符是否存在
        SymbolTableEntry* entry = _stable.findEntry(identifierValue);

        // 如果标识符不存在于符号表中，输出错误信息或采取相应的处理
        if (entry == nullptr) {
            cerr << "发生错误:变量'" << identifierValue << "'不在符号表中" << endl;
            return ERROR;
        }

        SyntaxTreeNode* childNode = new SyntaxTreeNode(identifier, "");
        TreeNode->addChild(childNode);
        string identifier = _IDENTIFIER(childNode);
        if (identifier == ERROR) {
            TreeNode->getChildren().pop_back();
            return ERROR;
        }
        return identifier;
    }
    else if (this->LexicalMachine.getToken()._type == "NUMBER") {
        SyntaxTreeNode* childNode = new SyntaxTreeNode(unsigned_integer, "");
        TreeNode->addChild(childNode);
        string unsignedinteger = _UNSIGNEDINTEGER(childNode);
        if (unsignedinteger == ERROR) {
            TreeNode->getChildren().pop_back();
            return ERROR;
        }
        return unsignedinteger;
    }
    else if (this->LexicalMachine.getToken()._value == "(") {
        this->LexicalMachine.Analyze();       //读取下一个token

        SyntaxTreeNode* childNode = new SyntaxTreeNode(opening_parentheses, "");
        TreeNode->addChild(childNode);
        string expression = _EXPRESSION(childNode);
        if (expression == ERROR) {
            TreeNode->getChildren().pop_back();
            return ERROR;
        }
        if (this->LexicalMachine.getToken()._value == ")") {
            this->LexicalMachine.Analyze();
            SyntaxTreeNode* childNode = new SyntaxTreeNode(closing_parentheses, "");
            TreeNode->addChild(childNode);
        }
        else {
            return ERROR;
        }
        return expression;
    }
    return ERROR;
}

// 15 <加法运算符> → + | -
string SyntaxAnalyzer::_ADDITIVEOPERATOR(SyntaxTreeNode* TreeNode)
{
    string temp = this->LexicalMachine.getToken()._value;
    if (this->LexicalMachine.getToken()._value == "+" ||
        this->LexicalMachine.getToken()._value == "-") {
        SyntaxTreeNode* childNode = new SyntaxTreeNode(ADDITIVE_OPERATOR, this->LexicalMachine.getToken()._value);
        TreeNode->addChild(childNode);
        this->LexicalMachine.Analyze();
        return temp;
    }
    return ERROR;
}

//16 <乘法运算符>→ * | /
string SyntaxAnalyzer::_MULTIPLICATIVEOPERATOR(SyntaxTreeNode* TreeNode)
{
    string temp = this->LexicalMachine.getToken()._value;
    if (this->LexicalMachine.getToken()._value == "*" ||
        this->LexicalMachine.getToken()._value == "/") {
        SyntaxTreeNode* childNode = new SyntaxTreeNode(MULTIPLICATIVE_OPERATOR, this->LexicalMachine.getToken()._value);
        TreeNode->addChild(childNode);
        this->LexicalMachine.Analyze();
        return temp;
    }
    return ERROR;
}

//17 <条件语句>→IF <条件> THEN <语句>
string SyntaxAnalyzer::_CONDITIONALSTATEMENT(SyntaxTreeNode* TreeNode)
{
    int label1 = newLabel();
    int label2 = newLabel();
    if (this->LexicalMachine.getToken()._value == "IF") {
        this->LexicalMachine.Analyze();  //读取下一个字符
        SyntaxTreeNode* childNode = new SyntaxTreeNode(IF, "IF");
        TreeNode->addChild(childNode);
    }
    else {
        return ERROR;
    }

    SyntaxTreeNode* childNode2 = new SyntaxTreeNode(condition, "");
    TreeNode->addChild(childNode2);
    string condition = _CONDITION(childNode2);
    if (condition == ERROR) {
        TreeNode->getChildren().pop_back();
        return ERROR;
    }

    emit("if " + condition + " goto $" + to_string(label1));
    emit("goto $" + to_string(label2));

    labelValues[label1] = instuctioncount - 1;
    //读取下一个标识符
    if (this->LexicalMachine.getToken()._value == "THEN") {
        this->LexicalMachine.Analyze();  //读取下一个字符
        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(THEN, "THEN");
        TreeNode->addChild(childNode3);
    }
    else {
        return ERROR;
    }

    SyntaxTreeNode* childNode4 = new SyntaxTreeNode(statement, "");
    TreeNode->addChild(childNode4);
    string statement = _STATEMENT(childNode4);
    if (statement == ERROR) {
        TreeNode->getChildren().pop_back();
        return ERROR;
    }
    labelValues[label2] = instuctioncount - 1;
    return "";
}

//18 <循环语句>→WHILE <条件> DO <语句>
string SyntaxAnalyzer::_LOOPSTATEMENT(SyntaxTreeNode* TreeNode)
{
    int labelBegin = newLabel();
    int labelEnd = newLabel();
    int labelStatement = newLabel();

    labelBegin = instuctioncount - 1;
    if (this->LexicalMachine.getToken()._value == "WHILE") {
        this->LexicalMachine.Analyze();  //读取下一个字符
        SyntaxTreeNode* childNode = new SyntaxTreeNode(WHILE, "WHILE");
        TreeNode->addChild(childNode);
    }
    else {
        return ERROR;
    }

    SyntaxTreeNode* childNode2 = new SyntaxTreeNode(condition, "");
    TreeNode->addChild(childNode2);


    string condition = _CONDITION(childNode2);
    if (condition == ERROR) {
        TreeNode->getChildren().pop_back();
        return ERROR;
    }
    emit("if " + condition + " goto $" + to_string(labelStatement));
    emit("goto $" + to_string(labelEnd));
    labelValues[labelStatement] = instuctioncount - 1;
    //读取下一个标识符
    if (this->LexicalMachine.getToken()._value == "DO") {
        this->LexicalMachine.Analyze();  //读取下一个字符
        SyntaxTreeNode* childNode3 = new SyntaxTreeNode(DO, "DO");
        TreeNode->addChild(childNode3);
    }
    else {
        return ERROR;
    }

    SyntaxTreeNode* childNode4 = new SyntaxTreeNode(statement, "");
    TreeNode->addChild(childNode4);
    string statement = _STATEMENT(childNode4);
    if (statement == ERROR) {
        TreeNode->getChildren().pop_back();
        return ERROR;
    }

    emit("goto " + to_string(labelBegin));
    labelValues[labelEnd] = int(codeline.size());
    return "";
}

//19 <条件>→<表达式> <关系运算符> <表达式>
string SyntaxAnalyzer::_CONDITION(SyntaxTreeNode* TreeNode)
{
    SyntaxTreeNode* childNode = new SyntaxTreeNode(expression, "");
    TreeNode->addChild(childNode);
    string expression1 = _EXPRESSION(childNode);
    if (expression1 == ERROR) {
        TreeNode->getChildren().pop_back();
        return ERROR;
    }

    SyntaxTreeNode* childNode2 = new SyntaxTreeNode(relation_operator, "");
    TreeNode->addChild(childNode2);
    string op = _RELATIONOPERATOR(childNode2);
    if (op == ERROR) {
        TreeNode->getChildren().pop_back();
        cout << "第" << LexicalMachine.getLine() << "行条件判断缺少关系运算符" << endl;
        return ERROR;
    }

    SyntaxTreeNode* childNode3 = new SyntaxTreeNode(expression, "");
    TreeNode->addChild(childNode3);
    string expression2 = _EXPRESSION(childNode3);
    if (expression2 == ERROR) {
        TreeNode->getChildren().pop_back();
        cout << "第" << LexicalMachine.getLine() << "行条件运算出错" << endl;
        return ERROR;
    }
    return expression1 + op + expression2;
}

//20 <关系运算符>→ = | <> | < | <= | > | >=
string SyntaxAnalyzer::_RELATIONOPERATOR(SyntaxTreeNode* TreeNode)
{
    string temp = this->LexicalMachine.getToken()._value;
    if (temp == "=" || temp == "<>" ||  temp == "<" || temp == "<=" || temp == ">" || temp == ">=") {

        SyntaxTreeNode* childNode = new SyntaxTreeNode(RELATION_OPERATOR, temp);
        this->LexicalMachine.Analyze();
        TreeNode->addChild(childNode);
    }
    else {
        cout << "第" << LexicalMachine.getLine() << "行出现语法错误，不符合关系运算符定义规则！" << endl;
        return ERROR;
    }
    return temp;
}

//21 <字⺟>→a | b … | x | y | z
string SyntaxAnalyzer::_LETTER(SyntaxTreeNode* TreeNode)
{
    string temp = this->LexicalMachine.getToken()._value;
    if (temp >= "a" && temp <= "z") {
        this->LexicalMachine.Analyze();
        SyntaxTreeNode* childNode = new SyntaxTreeNode(letter, "");
        TreeNode->addChild(childNode);
    }
    else {
        cout << "第" << LexicalMachine.getLine() << "行出现语法错误，不符合字母定义规则！" << endl;
        return ERROR;
    }
    return temp;
}

//22 <数字>→0 | 1 | … | 8 | 9
string SyntaxAnalyzer::_NUMBER(SyntaxTreeNode* TreeNode)
{
    string temp = this->LexicalMachine.getToken()._value;
    if (temp >= "0" &&  temp <= "9") {
        SyntaxTreeNode* childNode = new SyntaxTreeNode(NUMBER, temp);
        this->LexicalMachine.Analyze();
        TreeNode->addChild(childNode);
    }
    else {
        cout << "第" << LexicalMachine.getLine() << "行出现语法错误，不符合数字规则！" << endl;
        return ERROR;
    }
    return temp;
}

//<空语句>
string SyntaxAnalyzer::_EMPTYSTATEMENT(SyntaxTreeNode* TreeNode)
{
    SyntaxTreeNode* childNode = new SyntaxTreeNode(EMPTY_STATEMENT, "empty");
    TreeNode->addChild(childNode);
    return "";
}