#pragma once
#include <string>
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
#include "syntax.h"
#include "../lexical//lexical.h"
using namespace std;

// 每一条语法规则对应一个NodeType
enum NodeType {
    // 避免和关键字重复，这里枚举从9开始
    PROGRAM = 9,
    PROGRAM_HEADER,
    SUB_PROGRAM,
    CONST_DECLARATION,
    CONST_DEFINITION,
    UNSIGNED_INTEGER,
    VAR_DECLARATION,
    IDENTIFIER,
    COMPOUND_STATEMENT,
    STATEMENT,
    ASSIGNMENT_STATEMENT,
    EXPRESSION,
    TERM,
    FACTOR,
    ADDITIVE_OPERATOR,
    MULTIPLICATIVE_OPERATOR,
    CONDITIONAL_STATEMENT,
    LOOP_STATEMENT,
    CONDITION,
    RELATION_OPERATOR,
    LETTER,
    NUMBER,
    EMPTY_STATEMENT 
};

// 语法分析树节点类
class SyntaxTreeNode {
public:
    // 构造函数
    SyntaxTreeNode(NodeType type, const string& value);

    // 添加子节点
    void addChild(SyntaxTreeNode* child);

    // 获取节点类型
    NodeType getType() const;

    // 获取节点值
    const string& getValue() const;

    // 获取子节点列表
    const vector<SyntaxTreeNode*>& getChildren() const;

private:
    NodeType type_;                      // 节点类型
    string value_;                       // 节点值（对于终结符节点）
    vector<SyntaxTreeNode*> children_;   // 子节点列表
};