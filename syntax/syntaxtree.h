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
#include "lexical.h"
using namespace std;

enum NodeType {
    PROGRAM,
    PROGRAM_HEADER,
    SUB_PROGRAM,
    CONST_DECLARATION,
    CONST_DEFINITION,
    VAR_DECLARATION,
    IDENTIFIER,
    COMPOUND_STATEMENT,
    STATEMENT,
    ASSIGNMENT_STATEMENT,
    EXPRESSION,
    TERM,
    FACTOR,
    RELATION_EXPRESSION,
    RELATION_OPERATOR,
    LETTER,
    NUMBER
};

// �﷨�������ڵ���
class SyntaxTreeNode {
public:
    // ���캯��
    SyntaxTreeNode(NodeType type, const string& value);

    // ����ӽڵ�
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