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

// ���ýڵ�ֵ
void SyntaxTreeNode::setValue(string value){
    value_ = value;
}

// ��ȡ�ӽڵ��б�
vector<SyntaxTreeNode*>& SyntaxTreeNode::getChildren(){
    return children_;
}

// ��ȡ�ڵ��ӽڵ���
int SyntaxTreeNode::getchildrennum() {
    return children_.size();
}

// ��ȡ�ǿսڵ��ӽڵ���
int SyntaxTreeNode::getnotnullchildrennum() {
    int num = 0;
    for (SyntaxTreeNode* node : children_) {
        if (node->getValue() != "")
            num++;
    }
    return num;
}