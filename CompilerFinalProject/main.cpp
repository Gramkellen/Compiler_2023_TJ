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



int main()
{
    //非固定情况
    /*string filename;
    cout << "请输入文件路径:" << endl;
    cin >> filename;*/
    string filename = ".\\program.txt";
    SyntaxAnalyzer SyntaxMachine(filename);
    SyntaxMachine.syntaxanalyze(SyntaxMachine.getSyntaxTreeRoot(), PROGRAMENTRY);
    SyntaxMachine.printcode();
    return 0;
}
