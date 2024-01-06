#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <string>
#include <climits>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include"lexical.h"
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
    while (((str[i] >= 'A' && str[i] <= 'Z')  || (str[i] >= 'a' && str[i] <= 'z')) || (str[i] >= '0' && str[i] <= '9')) {
        tempWord += str[i];
        i++;
    }
    i--;
    if (findKeyords(tempWord)) {
        beforeToken = currentToken;
        currentToken = Token("KEYWORDS", tempWord); //关键字
    }
    else {
        beforeToken = currentToken;
        currentToken = Token("IDENTIFIER", tempWord); //标识符
    }
}

//数字识别
void LexicalAnalyzer::digitAnalysis(string& str, int& i) {
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
    if ((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z')) {
        beforeToken = currentToken;
        cout << "第" << filerow << "行，第" << filecol << "列出现词法错误！标识符首位为数字，或者数字拼写错误！" << endl;
        currentToken = Token("Error","Error");
        return;
    }
    i--;
    beforeToken = currentToken;
    currentToken = Token("NUMBER", tempWord);
}

//算符、界限符检测
void LexicalAnalyzer::operatorAnalysis(string& str, int& i) {
    string tempWord;
    tempWord += str[i];
    //判断是不是算符/界符
    int tempid = findOperatorWords(tempWord);
    if (tempid) {
        tempWord += str[i + 1];
        int tempid2 = findOperatorWords(tempWord);
        //说明是二个运算符
        if (tempid2) { 
            i++;
            beforeToken = currentToken;
            currentToken = Token("TwoOP", tempWord);
        }
        //只存在一元的
        else {
            tempWord.clear();
            tempWord += str[i];
            beforeToken = currentToken;
            currentToken = Token("OneOP", tempWord);
        }
    }
    //非标识符、数字和算符/操作符，认为出现词法拼写错误，返回false
    else {
        tempWord += str[i + 1];
        int tempid2 = findOperatorWords(tempWord);
        //可能直接是二元的，然后一元没有对应
        if (tempid2) {
            //判断关系运算法
            beforeToken = currentToken;
            i++;
            currentToken = Token("TwoOP", tempWord);;
        }
        else
        {
            beforeToken = currentToken;
            cout << "第" << filerow << "行，第" << filecol << "列出现拼写错误！" << endl;
            currentToken = Token("Error", "Error");
            
        }
    }
}

//词法分析
void LexicalAnalyzer::Analyze() {
    int i = filecol;
    for (; i < currentline.length(); i++) {
        if (currentline[i] == ' ' || currentline[i] == 9) {//空格或tab
            continue; 
        }
        //识别关键字 or  标识符
        else if ((currentline[i] >= 'A' && currentline[i] <= 'Z') ||
            (currentline[i] >= 'a' && currentline[i] <= 'z')) {
            letterAnalysis(currentline, i);
            break;
        }
        //识别整数
        else if (currentline[i] >= '0' && currentline[i] <= '9') {
             digitAnalysis(currentline, i);
             break;
        }
        else {
            // 判断运算符
             operatorAnalysis(currentline, i);
             break;
        }
    }
    //这一行判断结束了
    //Panic 模式，有 Error 用 Panic 跳过
    if (currentToken._value != "Error" && i < currentline.length()) {
        filecol = i + 1;
    }
    else
    {
        readFile();
        filecol = 0;
        Analyze();
    }
  
}

//读取文件
void LexicalAnalyzer::readFile() {
    if (file.is_open()) {
        string line;
        bool isClose = true;
        while(std::getline(file, line)) { // 逐行读取文件内容,换行符会被读取然后丢弃
            if (line.length() != 0) {
                currentline = line;
                filerow++;
                isClose = false;
                break;
            }
        }
        if (isClose) {
            file.close(); // 关闭文件流
        }  
    }
    else
    {
        cout << "打开文件错误！" << endl;
        exit(EXIT_FAILURE);              //直接退出文件运行
    }
}

//返回行号
int LexicalAnalyzer::getLine() {
    return this->filerow;
}

Token LexicalAnalyzer::getToken(int choice)
{
	if (choice == 1) {
		return this->currentToken;
	}
	else
	{
		return  this->beforeToken;
	}
}


//-----------------文件读取函数实现-----------------//
/*
* 注解：
* 这里是对整个进行读取测试，实际需要结合语法分析和中间代码生成实现：
  边读边进行词法和语法分析，然后翻译成中间代码
*/
//string readFile(string filename) {
//    std::ifstream file(filename); // 打开名 txt 的文件
//    string LexicalAnalysisInput;
//    if (file.is_open()) { // 检查文件是否成功打开
//        std::string line;
//        while (std::getline(file, line)) { // 逐行读取文件内容,换行符会被读取然后丢弃
//            if (line.length() != 0) {
//                LexicalAnalysisInput += line;
//                LexicalAnalysisInput += " ";
//            }
//        }
//        file.close(); // 关闭文件流
//        return LexicalAnalysisInput;
//    }
//    else {
//        std::cerr << "Unable to open file" << std::endl;
//        return "";
//    }
//}


