#include "syntax.h"

// 构造函数
SyntaxTreeNode::SyntaxTreeNode(NodeType type, const string& value) : type_(type), value_(value) {}

// 添加子节点
void SyntaxTreeNode::addChild(SyntaxTreeNode* child) {
    children_.push_back(child);
}

// 获取节点类型
NodeType SyntaxTreeNode::getType() const {
    return type_;
}

// 获取节点值
const string& SyntaxTreeNode::getValue() const {
    return value_;
}

// 设置节点值
void SyntaxTreeNode::setValue(string value){
    value_ = value;
}

// 获取子节点列表
vector<SyntaxTreeNode*>& SyntaxTreeNode::getChildren(){
    return children_;
}

// 获取节点子节点数
int SyntaxTreeNode::getchildrennum() {
    return children_.size();
}

// 获取非空节点子节点数
int SyntaxTreeNode::getnotnullchildrennum() {
    int num = 0;
    for (SyntaxTreeNode* node : children_) {
        if (node->getValue() != "")
            num++;
    }
    return num;
}