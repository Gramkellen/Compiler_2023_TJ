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
            for (const SyntaxTreeNode* child : children) {
                std::cout << child->getType() <<" "<< child->getValue() << std::endl;
            }
        }

        for (SyntaxTreeNode* child : children) {
            queue.push(child);
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
    bfs(s.getSyntaxTreeRoot());

    return 0;
}
