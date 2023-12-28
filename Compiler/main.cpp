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

void Test1()
{
    bool result = true;
    LexicalAnalyzer LexicalMachine;
    //��������Լ����ô����ļ�·��:
    //string filename = ""
    //readFile(filename)
    string LexicalAnalysisInput = readFile();
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
}

int main()
{
    Test1();
    return 0;
}
