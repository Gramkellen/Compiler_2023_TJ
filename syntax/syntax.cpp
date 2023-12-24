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

    switch (nodetype) {
    //to do
        //这里添加更多的语法规则

    case PROGRAM: 
        // <程序>→<程序⾸部> <分程序>
        if (rootNode->getType() == PROGRAM) {
            vector<SyntaxTreeNode*> children = rootNode->getChildren();

            // 检查子节点数量是否为2（程序首部和分程序）
            if (children.size() == 2) {
                // 检查第一个子节点是否满足程序首部的规则
                if (children[0]->getType() != PROGRAM_HEADER) {
                    return false;  
                }

                // 检查第二个子节点是否满足分程序的规则
                if (children[1]->getType() != SUB_PROGRAM) {
                    return false;  
                }

                return true;  
            }
            return false;
        }
        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child, PROGRAM)) {
                return true;  
            }
        }

        return false; 


    case PROGRAM_HEADER: 
        // <程序⾸部>→PROGRAM <标识符>
        if (rootNode->getType() == PROGRAM_HEADER) {
            vector<SyntaxTreeNode*> children = rootNode->getChildren();

            // 检查子节点数量是否为2（PROGRAM和标识符）
            if (children.size() == 2) {
                if (children[0]->getType() != KW_PROGRAM) {
                    return false; 
                }

                // 检查第二个子节点是否为标识符
                if (children[1]->getType() != IDENTIFIER) {
                    return false; 
                }

                return true;
            }
            return false;
        }
        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child, PROGRAM_HEADER)) {
                return true; 
            }
        }

        return false; 

    case SUB_PROGRAM:
        // <分程序>→[<常量说明>][<变量说明>]<语句>
        if (rootNode->getType() == SUB_PROGRAM) {
            vector<SyntaxTreeNode*> children = rootNode->getChildren();

            // 检查子节点数量是否在1~3之间
            if (children.size() >= 1 && children.size() <= 3) {
                int index = 0;

                // 检查第一个子节点是否为常量说明
                if (children.size() > index && children[index]->getType() == CONST_DECLARATION) {
                    index++;
                }

                // 检查下一个子节点是否为变量说明
                if (children.size() > index && children[index]->getType() == VAR_DECLARATION) {
                    index++;
                }

                // 检查最后一个子节点是否为语句
                if (children.size() > index && children[index]->getType() != STATEMENT) {
                    return false;  
                }

                return true; 
            }
            return false;
        }
        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child, SUB_PROGRAM)) {
                return true;  
            }
        }

        return false; 

    case CONST_DECLARATION:
        // <常量说明>→CONST <常量定义>{，<常量定义>}
        if (rootNode->getType() == CONST_DECLARATION) {
            // 获取子节点
            vector<SyntaxTreeNode*> children = rootNode->getChildren();

            // 检查子节点数量是否至少为2（CONST和至少一个常量定义）
            if (children.size() >= 2) {
                // 检查第一个子节点是否为CONST
                if (children[0]->getType() != KW_CONST) {
                    return false;  
                }

                // 检查剩余的子节点是否都为常量定义
                for (int i = 1; i < children.size(); i++) {
                    if (children[i]->getType() != CONST_DEFINITION) {
                        return false;  
                    }
                }

                return true;  
            }
            return false;
        }
        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child, CONST_DECLARATION)) {
                return true; 
            }
        }

        return false;  

    case CONST_DEFINITION:
        if (rootNode->getType() == CONST_DECLARATION) {
            // <常量定义>→<标识符>:=<⽆符号整数>
            vector<SyntaxTreeNode*> children = rootNode->getChildren();

            // 检查子节点数量是否为3（标识符，:=，无符号整数）
            if (children.size() == 3) {
                if (children[0]->getType() != IDENTIFIER) {
                    return false;
                }

                if (children[1]->getValue() != ":=") {
                    return false;
                }

                if (children[2]->getType() != UNSIGNED_INTEGER) {
                    return false;
                }

                return true;
            }
        }
        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child, CONST_DEFINITION)) {
                return true; 
            }
        }

        return false; 

    case UNSIGNED_INTEGER:
        // <无符号整数>→<数字>{<数字>}
        if (rootNode->getType() == UNSIGNED_INTEGER) {
            const string nodeValue = rootNode->getValue();

            // 检查值是否为无符号整数
            for (const auto& character : nodeValue) {
                if (!isdigit(character)) {
                    return false; 
                }
            }

            return true; 
        }

        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child, UNSIGNED_INTEGER)) {
                return true;  
            }
        }

        return false; 

    case VAR_DECLARATION:
        // <变量说明>→VAR<标识符>{，<标识符>};
        if (rootNode->getType() == VAR_DECLARATION) {
            vector<SyntaxTreeNode*> children = rootNode->getChildren();

            // 检查子节点数量是否至少为2（VAR，标识符，;）
            if (children.size() < 3) {
                return false; 
            }

            // 检查第一个子节点是否为VAR
            if (children[0]->getType() != KW_VAR) {
                return false;  
            }

            // 检查最后一个子节点是否为;
            if (children.back()->getValue() != ";") {
                return false; 
            }

            // 检查中间的子节点是否为标识符，或者为，后跟标识符
            for (int i = 1; i < children.size() - 1; ++i) {
                if (children[i]->getType() != IDENTIFIER) {
                    if (children[i]->getValue() != "," || i + 1 >= children.size() - 1 || children[i + 1]->getType() != IDENTIFIER) {
                        return false; 
                    }
                    ++i;  // 跳过后面的标识符
                }
            }

            return true;
        }

        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child, VAR_DECLARATION)) {
                return true;
            }
        }

        return false;

    case STATEMENT:
        // <语句>→<赋值语句> | <条件语句> | <循环语句> | <复合语句> | <空语句>
        if (rootNode->getType() == STATEMENT) {
            // 获取子节点
            vector<SyntaxTreeNode*> children = rootNode->getChildren();

            // 检查子节点数量是否为1
            if (children.size() != 1) {
                return false;  
            }

            // 检查子节点类型是否为赋值语句，条件语句，循环语句，复合语句，或空语句
            SyntaxTreeNode* child = children[0];
            if (child->getType() == ASSIGNMENT_STATEMENT || child->getType() == CONDITIONAL_STATEMENT ||
                child->getType() == LOOP_STATEMENT || child->getType() == COMPOUND_STATEMENT || child->getType() == EMPTY_STATEMENT) {
                return true; 
            }
        }

        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child, STATEMENT)) {
                return true; 
            }
        }

        return false; 

    case ASSIGNMENT_STATEMENT:
        // <赋值语句>→<标识符>:=<表达式>
        if (rootNode->getType() == ASSIGNMENT_STATEMENT) {
            // 获取子节点
            vector<SyntaxTreeNode*> children = rootNode->getChildren();

            // 检查子节点数量是否为3（标识符，:=，表达式）
            if (children.size() != 3) {
                return false; 
            }

            // 检查第一个子节点是否为标识符
            if (children[0]->getType() != IDENTIFIER) {
                return false; 
            }

            // 检查第二个子节点是否为:=
            if (children[1]->getValue() != ":=") {
                return false; 
            }

            // 检查第三个子节点是否为表达式
            if (children[2]->getType() != EXPRESSION) {
                return false;  
            }

            return true; 
        }

        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child, ASSIGNMENT_STATEMENT)) {
                return true; 
            }
        }

        return false; 

    case EXPRESSION:
        // <表达式>→[+|-]项 | <表达式> <加法运算符> <项>
        if (rootNode->getType() == EXPRESSION) {
            // 获取子节点
            vector<SyntaxTreeNode*> children = rootNode->getChildren();

            // 检查子节点数量是否为1、2或3
            if (children.size() < 1 || children.size() > 3) {
                return false;  // 不满足规则
            }

            // 如果子节点数量为1，检查子节点是否为项
            if (children.size() == 1) {
                if (children[0]->getType() != TERM) {
                    return false;  
                }
            }

            // 如果子节点数量为2，检查子节点是否为[+|-]项
            if (children.size() == 2) {
                if (children[0]->getValue() != "+" && children[0]->getValue() != "-" || children[1]->getType() != TERM) {
                    return false;  
                }
            }

            // 如果子节点数量为3，检查子节点是否为<表达式> <加法运算符> <项>
            if (children.size() == 3) {
                if (children[0]->getType() != EXPRESSION || children[1]->getType() != ADDITIVE_OPERATOR || children[2]->getType() != TERM) {
                    return false;  
                }
            }

            return true;  
        }

        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child, EXPRESSION)) {
                return true;  
            }
        }

        return false;  

    case TERM:
        // <项>→<因子> | <项> <乘法运算符> <因子>
        if (rootNode->getType() == TERM) {
            vector<SyntaxTreeNode*> children = rootNode->getChildren();

            // 检查子节点数量是否为1或3
            if (children.size() != 1 && children.size() != 3) {
                return false; 
            }

            // 如果子节点数量为1，检查子节点是否为因子
            if (children.size() == 1) {
                if (children[0]->getType() != FACTOR) {
                    return false;  
                }
            }

            // 如果子节点数量为3，检查子节点是否为<TERM> <乘法运算符> <因子>
            if (children.size() == 3) {
                if (children[0]->getType() != TERM || children[1]->getType() != MULTIPLICATIVE_OPERATOR || children[2]->getType() != FACTOR) {
                    return false;  
                }
            }

            return true;  
        }

        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child, TERM)) {
                return true; 
            }
        }

        return false; 

    case FACTOR:
        // <因子>→<标识符> |<无符号整数> | (<表达式>)
        if (rootNode->getType() == FACTOR) {
            // 获取子节点
            vector<SyntaxTreeNode*> children = rootNode->getChildren();

            // 检查子节点数量是否为1或3
            if (children.size() != 1 && children.size() != 3) {
                return false;
            }

            // 如果子节点数量为1，检查子节点是否为标识符或无符号整数
            if (children.size() == 1) {
                if (children[0]->getType() != IDENTIFIER && children[0]->getType() != UNSIGNED_INTEGER) {
                    return false; 
                }
            }

            // 如果子节点数量为3，检查子节点是否为(<表达式>)
            if (children.size() == 3) {
                if (children[0]->getValue() != "(" || children[1]->getType() != EXPRESSION || children[2]->getValue() != ")") {
                    return false;  
                }
            }

            return true; 
        }

        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child, FACTOR)) {
                return true;  
            }
        }

        return false;  

    case ADDITIVE_OPERATOR:
        // <加法运算符>→ + | -
        if (rootNode->getType() == ADDITIVE_OPERATOR) {
            string value = rootNode->getValue();

            // 检查节点的值是否为"+"或"-"
            if (value != "+" && value != "-") {
                return false; 
            }

            return true;
        }

        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child, ADDITIVE_OPERATOR)) {
                return true; 
            }
        }

        return false; 

    case MULTIPLICATIVE_OPERATOR:
        // <乘法运算符>→ * | /
        if (rootNode->getType() == MULTIPLICATIVE_OPERATOR) {
            // 获取节点的值
            string value = rootNode->getValue();

            // 检查节点的值是否为"*"或"/"
            if (value != "*" && value != "/") {
                return false; 
            }

            return true;
        }

        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child, MULTIPLICATIVE_OPERATOR)) {
                return true;
            }
        }

        return false;

    case CONDITIONAL_STATEMENT:
        // <条件语句>→IF <条件> THEN <语句>
        if (rootNode->getType() == CONDITIONAL_STATEMENT) {
            // 获取子节点
            vector<SyntaxTreeNode*> children = rootNode->getChildren();

            // 检查子节点数量是否为4
            if (children.size() != 4) {
                return false; 
            }

            // 检查子节点是否为IF <条件> THEN <语句>
            if (children[0]->getValue() != "IF" || children[1]->getType() != CONDITION || children[2]->getValue() != "THEN" || children[3]->getType() != STATEMENT) {
                return false; 
            }

            return true; 
        }

        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child, CONDITIONAL_STATEMENT)) {
                return true;  // 子节点中有满足规则的
            }
        }

        return false;  // 没有找到满足规则的节点

    case LOOP_STATEMENT:
        // 检查节点类型是否为循环语句:<循环语句>→WHILE <条件> DO <语句>
        if (rootNode->getType() == LOOP_STATEMENT) {
            // 获取子节点
            vector<SyntaxTreeNode*> children = rootNode->getChildren();

            // 检查子节点数量是否为4
            if (children.size() != 4) {
                return false;  
            }

            // 检查子节点是否为WHILE <条件> DO <语句>
            if (children[0]->getValue() != "WHILE" || children[1]->getType() != CONDITION || children[2]->getValue() != "DO" || children[3]->getType() != STATEMENT) {
                return false;  
            }

            return true;  
        }

        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child, LOOP_STATEMENT)) {
                return true;  
            }
        }

        return false; 

    case CONDITION:
        // 检查节点类型是否为条件:<条件>→<表达式> <关系运算符> <表达式>
        if (rootNode->getType() == CONDITION) {
            // 获取子节点
            vector<SyntaxTreeNode*> children = rootNode->getChildren();

            // 检查子节点数量是否为3
            if (children.size() != 3) {
                return false;  // 不满足规则
            }

            // 检查子节点是否为<表达式> <关系运算符> <表达式>
            if (children[0]->getType() != EXPRESSION || children[1]->getType() != RELATION_OPERATOR || children[2]->getType() != EXPRESSION) {
                return false; 
            }

            return true;
        }

        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child, CONDITION)) {
                return true;  
            }
        }

        return false; 



    case RELATION_OPERATOR:
        // 检查节点类型是否为关系运算符:<关系运算符>→ = | <> | < | <= | > | >=
        if (rootNode->getType() == RELATION_OPERATOR) {
            // 检查值是否在规定的运算符集合中
            const vector<string> validOperators = { "=", "<>", "<", "<=", ">", ">=" };
            const string nodeValue = rootNode->getValue();

            for (const auto& validOperator : validOperators) {
                if (nodeValue == validOperator) {
                    return true;  
                }
            }
        }

        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child, RELATION_OPERATOR)) {
                return true;  
            }
        }

        return false; 

   

    case LETTER:
        // 检查节点类型是否为字母:<字⺟>→a | b … | x | y | z
        if (rootNode->getType() == LETTER) {
            // 检查值是否是单个字母
            const string& nodeValue = rootNode->getValue();
            if (nodeValue.size() == 1 && isalpha(nodeValue[0])) {
                return true;  // 满足规则
            }
        }

        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child,LETTER)) {
                return true;  // 子节点中有满足规则的
            }
        }

        return false;  // 没有找到满足规则的节点

    case NUMBER:
        // 检查节点类型是否为数字:<数字>→0 | 1 | … | 8 | 9
        if (rootNode->getType() == NUMBER) {
            // 检查值是否是单个数字字符
            const string& nodeValue = rootNode->getValue();
            if (nodeValue.size() == 1 && isdigit(nodeValue[0])) {
                return true;  // 满足规则
            }
        }

        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child,NUMBER)) {
                return true;  // 子节点中有满足规则的
            }
        }

        return false;  // 没有找到满足规则的节点

    default:
        cout << "语法错误" << endl;
        return false;
    }
    return false;
}