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
#include <queue>
#include "syntax.h"
#include "lexical.h"
#include "symboltable.h"
using namespace std;

// 每一条语法规则对应一个NodeType
enum NodeType {
    PROGRAMENTRY,
    PROGRAM,
    END,
    CONST,
    VAR,
    WHILE,
    DO,
    IF,
    THEN,
    BEGIN,
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
    EMPTY_STATEMENT,
    COMMA,
    SEMICOLON,
    ASSIGNMENT,
    // 非终结符
    program,
    program_header,
    sub_program,
    const_declaration,
    const_definition,
    unsigned_integer,
    var_declaration,
    identifier,
    compound_statement,
    statement,
    assignment_statement,
    expression,
    expression2,
    opening_parentheses,
    closing_parentheses,
    term,
    term2,
    factor,
    additive_operator,
    multiplicative_operator,
    conditional_statement,
    loop_statement,
    condition,
    relation_operator,
    letter,
    number,
    empty_statement
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
    vector<SyntaxTreeNode*>& getChildren();

    void setValue(string value);

    int getchildrennum();

    int getnotnullchildrennum();

private:
    NodeType type_;                      // 节点类型
    string value_;                       // 节点值（对于终结符节点）
    vector<SyntaxTreeNode*> children_;   // 子节点列表
  
};