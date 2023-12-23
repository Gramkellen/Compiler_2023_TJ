#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include <string>
#include <vector>


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
    string value_;                  // 节点值（对于终结符节点）
    vector<SyntaxTreeNode*> children_;  // 子节点列表
};

#endif  // SYNTAX_TREE_H