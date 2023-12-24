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

void Test01()
{
    bool result = true;
    LexicalAnalyzer LexicalMachine;
    //这里可以自己设置传入文件路径:
    //string filename = ""
    //readFile(filename)
    string LexicalAnalysisInput = readFile();
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
}

int main()
{
   
    return 0;
}
