#include "syntax.h"

// ���캯��
SyntaxTreeNode::SyntaxTreeNode(NodeType type, const string& value) : type_(type), value_(value) {}

// ����ӽڵ�
void SyntaxTreeNode::addChild(SyntaxTreeNode* child) {
    children_.push_back(child);
}

// ��ȡ�ڵ�����
NodeType SyntaxTreeNode::getType() const {
    return type_;
}

// ��ȡ�ڵ�ֵ
const string& SyntaxTreeNode::getValue() const {
    return value_;
}

// ��ȡ�ӽڵ��б�
const vector<SyntaxTreeNode*>& SyntaxTreeNode::getChildren() const {
    return children_;
}
