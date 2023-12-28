#include<iostream>
#include"lexical.h"
#include"syntax.h"
#include"symboltable.h"
using namespace std;

/* ����һ��ɨ��ķ�ʽ�����﷨������ͬʱ��������ֵ
* 1.���������﷨����AST
*	mknode(op,left,right);
*	mkleaf(id ,entry) 
*	mkleaf(num,entry)
* 
* 2.��ô���������﷨��
*/

void bfs(SyntaxTreeNode* root) {
    if (root == nullptr) {
        cout << "nullptr" << endl;
        return;
    }

    std::queue<SyntaxTreeNode*> queue;
    queue.push(root);

    while (!queue.empty()) {
        SyntaxTreeNode* node = queue.front();
        queue.pop();

        const vector<SyntaxTreeNode*>& children = node->getChildren();

        // ����ڵ�û���ӽڵ㣬��ô������Ҷ�ӽڵ�
        if (children.empty()) {
            cout << node->getType() << " " << node->getValue() << endl;
        }

        for (SyntaxTreeNode* child : children) {
            queue.push(child);
        }
    }
}

void dfs(SyntaxTreeNode* node) {
    if (node == nullptr) {
        return;
    }

    // Recurse on the children
    vector<SyntaxTreeNode*>& children = node->getChildren();
    if (children.empty() && node->getValue() != "empty") {
        // If the node has no children, it's a leaf node
        cout << node->getType() << " " << node->getValue() << endl;
    }
    else {
        for (SyntaxTreeNode* child : children) {
            dfs(child);
        }
    }
}

int main()
{
    bool result = true;
    LexicalAnalyzer LexicalMachine;
    //��������Լ����ô����ļ�·��:
    //string filename = ""
    //readFile(filename)
    string filename = "program.txt";
    string LexicalAnalysisInput = readFile(filename);
    if (LexicalMachine.Analyze(LexicalAnalysisInput)) {
        result = true;
    }
    else {
        //���ִʷ�����
        result = false;
        cout << "Lexical Error" << endl;
    }

    SyntaxAnalyzer s(LexicalMachine.getInput());
    result = s.syntaxanalyze(s.getSyntaxTreeRoot(), PROGRAMENTRY);
    if (result) {
        cout << "�﷨��������" << endl;
    }
    else
    {
        cout << "�﷨�����д�" << endl;
    }
    dfs(s.getSyntaxTreeRoot());

    return 0;
}
