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

// ÿһ���﷨�����Ӧһ��NodeType
enum NodeType {
<<<<<<< Updated upstream:syntax/syntaxtree.h
    // ����͹ؼ����ظ�������ö�ٴ�9��ʼ
    PROGRAM = 9,
=======
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
>>>>>>> Stashed changes:Compiler/syntaxtree.h
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
    ASSIGNMENT,
    // ���ս��
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
    empty_statement,
};

// �﷨�������ڵ���
class SyntaxTreeNode {
public:
    // ���캯��
    SyntaxTreeNode(NodeType type, const string& value);

    // �����ӽڵ�
    void addChild(SyntaxTreeNode* child);

    // ��ȡ�ڵ�����
    NodeType getType() const;

    // ��ȡ�ڵ�ֵ
    const string& getValue() const;

    // ��ȡ�ӽڵ��б�
    const vector<SyntaxTreeNode*>& getChildren() const;

private:
    NodeType type_;                      // �ڵ�����
    string value_;                       // �ڵ�ֵ�������ս���ڵ㣩
    vector<SyntaxTreeNode*> children_;   // �ӽڵ��б�
};