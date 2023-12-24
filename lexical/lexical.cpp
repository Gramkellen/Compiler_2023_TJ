#pragma once
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <string>
#include <climits>
#include <algorithm>
#include <fstream>
#include "lexical.h"
using namespace std;


//-----------------词法分析器实现-----------------//
//查找关键字
bool LexicalAnalyzer::findKeyords(const string& str) {
    auto cit = KEYWORDS.find(str);
    if (cit != KEYWORDS.end()) return true;
    else  return false;
}

//查找算符
int LexicalAnalyzer::findOperatorWords(const string& str) {
    auto cit = OPERATORWORD.find(str);
    if (cit != OPERATORWORD.end()) return (*cit).second;
    else  return 0;
}

//标识符识别
void LexicalAnalyzer::letterAnalysis(string& str, int& i)
{
    string tempWord;
    while ((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9')) {
        tempWord += str[i];
        i++;
    }
    i--;
    if (findKeyords(tempWord)) {
        LexicalAnalysisOutput.emplace_back("KEYWORDS", tempWord); //关键字
    }
    else {
        LexicalAnalysisOutput.emplace_back("IDENTIFIER", tempWord); //标识符
    }
}

//数字识别
bool LexicalAnalyzer::digitAnalysis(string& str, int& i) {
    string tempWord;
    if (str[i] == '0') {
        tempWord += "0";
        i++;
    }
    else {
        while (str[i] >= '0' && str[i] <= '9')
        {
            tempWord += str[i];
            i++;
        }
    }
    //数字后面紧跟字符，检测错误
    if (str[i] >= 'A' && str[i] <= 'Z') {
        return false;
    }
    LexicalAnalysisOutput.emplace_back("NUMBER", tempWord); //数字
    i--;
    return true;
}

//标识符检测
bool LexicalAnalyzer::operatorAnalysis(string& str, int& i) {
    string tempWord;
    tempWord += str[i];
    //判断是不是算符/界符
    int tempid = findOperatorWords(tempWord);
    if (tempid) {
        tempWord += str[i + 1];
        int tempid2 = findOperatorWords(tempWord);
        //说明是二元运算符
        if (tempid2) {
            LexicalAnalysisOutput.emplace_back("SecondOperator", tempWord);
            i++;
        }
        //只存在一元的
        else {
            tempWord.clear();
            tempWord += str[i];
            LexicalAnalysisOutput.emplace_back("OneOperator", tempWord);
        }
        return true;
    }
    //非标识符、数字和算符/操作符，认为出现词法拼写错误，返回false
    else {
        tempWord += str[i + 1];
        int tempid2 = findOperatorWords(tempWord);
        //可能直接是二元的，然后一元没有对应
        if (tempid2) {
            LexicalAnalysisOutput.emplace_back("SecondOperator", tempWord);
            i++;
        }
        else
        {
            return false;
        }
    }
}

//词法分析
bool LexicalAnalyzer::Analyze(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ' ' || str[i] == 9) {//空格或tab
            continue;
        }
        //识别标识符
        else if (str[i] >= 'A' && str[i] <= 'Z') {
            letterAnalysis(str, i);
        }
        //识别整数
        else if (str[i] >= '0' && str[i] <= '9') {
            if (!digitAnalysis(str, i)) { 
                //如果检测错误的话
                return false;
           }
        }
        else {
            if (!operatorAnalysis(str, i)) {
                return false;
            }
        }
    }
    return true;
}

//输出
void LexicalAnalyzer::Output() {
    for (auto i : LexicalAnalysisOutput) {
        cout << i._type << "  :  " << i._value << endl << endl;
    }
}