#include<iostream>
#include"lexical.h"
#include"syntax.h"
#include"symboltable.h"
using namespace std;

/* 采用一遍扫描的方式，在语法分析的同时计算属性值
* 1.建立抽象语法树：AST
*	mknode(op,left,right);
*	mkleaf(id ,entry) 
*	mkleaf(num,entry)
* 
* 2.怎么建立抽象语法树
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

        // 如果节点没有子节点，那么它就是叶子节点
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
    //这里可以自己设置传入文件路径:
    //string filename = ""
    //readFile(filename)
    string filename = "program.txt";
    string LexicalAnalysisInput = readFile(filename);
    if (LexicalMachine.Analyze(LexicalAnalysisInput)) {
        result = true;
    }
    else {
        //出现词法错误
        result = false;
        cout << "Lexical Error" << endl;
    }

    SyntaxAnalyzer s(LexicalMachine.getInput());
    result = s.syntaxanalyze(s.getSyntaxTreeRoot(), PROGRAMENTRY);
    if (result) {
        cout << "语法分析无误！" << endl;
    }
    else
    {
        cout << "语法分析有错！" << endl;
    }
    dfs(s.getSyntaxTreeRoot());

    return 0;
}
