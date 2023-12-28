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

// 获取子节点列表
vector<SyntaxTreeNode*>& SyntaxTreeNode::getChildren(){
    return children_;
}
