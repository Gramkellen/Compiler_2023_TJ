#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <string>
#include <climits>
#include <algorithm>
#include <fstream>
#include "./lexical/lexical.h"
#include "./syntax/syntax.h"
//#include "IntermediateCodeGenerator.h"
using namespace std;

//-----------------文件读取函数实现-----------------//
/*
* 注解：
* 这里是对整个进行读取测试，实际需要结合语法分析和中间代码生成实现：
  边读边进行词法和语法分析，然后翻译成中间代码
*/
string readFile(string filename) {
    ifstream file(filename); // 打开名 txt 的文件
    string LexicalAnalysisInput;
    if (file.is_open()) { // 检查文件是否成功打开
        string line;
        while (getline(file, line)) { // 逐行读取文件内容,换行符会被读取然后丢弃
            if (line.length() != 0) {
                cout << line << endl;
                LexicalAnalysisInput += line;
                LexicalAnalysisInput += " ";
            }
        }
        file.close(); // 关闭文件流
        return LexicalAnalysisInput;
    }
    else {
        cerr << "Unable to open file" << endl;
        return "";
    }
}


int main() {
    cout << "请输入要编译的程序文件路径：";
    string inputFilePath;
    getline(cin, inputFilePath);

    // 执行词法分析
    string LexicalAnalysisInput = readFile(inputFilePath);
    // 创建词法分析器
    LexicalAnalyzer lexicalanalyzer;
    cout << "开始词法分析" << endl;
    if (lexicalanalyzer.Analyze(LexicalAnalysisInput)) {
        cout << "词法分析结果：" << endl;
        lexicalanalyzer.Output();
    }
    else {//出现词法错误
        cout << "词法错误" << endl;
        return 1;
    }

    //// 执行语法分析
    vector<Word> LexicalResult = lexicalanalyzer.getlexicalresult();
    cout << "开始语法分析：" << endl;
    //// 创建语法分析器
    SyntaxAnalyzer syntaxanalyzer(LexicalResult);


    // 生成中间代码

    return 0;
}