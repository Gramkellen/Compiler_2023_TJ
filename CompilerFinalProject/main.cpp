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



int main()
{
    //�ǹ̶����
    /*string filename;
    cout << "�������ļ�·��:" << endl;
    cin >> filename;*/
    string filename = ".\\program.txt";
    SyntaxAnalyzer SyntaxMachine(filename);
    SyntaxMachine.syntaxanalyze(SyntaxMachine.getSyntaxTreeRoot(), PROGRAMENTRY);
    SyntaxMachine.printcode();
    return 0;
}
