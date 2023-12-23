#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include <string>
#include <vector>


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
    string value_;                  // �ڵ�ֵ�������ս���ڵ㣩
    vector<SyntaxTreeNode*> children_;  // �ӽڵ��б�
};

#endif  // SYNTAX_TREE_H