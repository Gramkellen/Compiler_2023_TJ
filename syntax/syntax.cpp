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


    case RELATION_OPERATOR:
        // 检查节点类型是否为关系运算符:<关系运算符>→ = | <> | < | <= | > | >=
        if (rootNode->getType() == RELATION_OPERATOR) {
            // 检查值是否在规定的运算符集合中
            const vector<string> validOperators = { "=", "<>", "<", "<=", ">", ">=" };
            const string nodeValue = rootNode->getValue();

            for (const auto& validOperator : validOperators) {
                if (nodeValue == validOperator) {
                    return true;  // 满足规则
                }
            }
        }

        // 递归检查子节点
        for (const auto& child : rootNode->getChildren()) {
            if (syntaxanalyze(child, RELATION_OPERATOR)) {
                return true;  // 子节点中有满足规则的
            }
        }

        return false;  // 没有找到满足规则的节点

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