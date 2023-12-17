#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <string>
#include <climits>
#include <algorithm>
#include <fstream>
#include"lexical.h"
using namespace std;


//-----------------�ʷ�������ʵ��-----------------//
//���ҹؼ���
bool LexicalAnalyzer::findKeyords(const string& str) {
    auto cit = KEYWORDS.find(str);
    if (cit != KEYWORDS.end()) return true;
    else  return false;
}

//�������
int LexicalAnalyzer::findOperatorWords(const string& str) {
    auto cit = OPERATORWORD.find(str);
    if (cit != OPERATORWORD.end()) return (*cit).second;
    else  return 0;
}

//��ʶ��ʶ��
void LexicalAnalyzer::letterAnalysis(string& str, int& i)
{
    string tempWord;
    while ((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9')) {
        tempWord += str[i];
        i++;
    }
    i--;
    if (findKeyords(tempWord)) {
        LexicalAnalysisOutput.emplace_back("KEYWORDS", tempWord); //�ؼ���
    }
    else {
        LexicalAnalysisOutput.emplace_back("IDENTIFIER", tempWord); //��ʶ��
    }
}

//����ʶ��
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
    //���ֺ�������ַ���������
    if (str[i] >= 'A' && str[i] <= 'Z') {
        return false;
    }
    LexicalAnalysisOutput.emplace_back("NUMBER", tempWord); //����
    i--;
    return true;
}

//��ʶ�����
bool LexicalAnalyzer::operatorAnalysis(string& str, int& i) {
    string tempWord;
    tempWord += str[i];
    //�ж��ǲ������/���
    int tempid = findOperatorWords(tempWord);
    if (tempid) {
        tempWord += str[i + 1];
        int tempid2 = findOperatorWords(tempWord);
        //˵���Ƕ�Ԫ�����
        if (tempid2) {
            LexicalAnalysisOutput.emplace_back("SecondOperator", tempWord);
            i++;
        }
        //ֻ����һԪ��
        else {
            tempWord.clear();
            tempWord += str[i];
            LexicalAnalysisOutput.emplace_back("OneOperator", tempWord);
        }
        return true;
    }
    //�Ǳ�ʶ�������ֺ����/����������Ϊ���ִʷ�ƴд���󣬷���false
    else {
        tempWord += str[i + 1];
        int tempid2 = findOperatorWords(tempWord);
        //����ֱ���Ƕ�Ԫ�ģ�Ȼ��һԪû�ж�Ӧ
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

//�ʷ�����
bool LexicalAnalyzer::Analyze(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ' ' || str[i] == 9) {//�ո��tab
            continue;
        }
        //ʶ���ʶ��
        else if (str[i] >= 'A' && str[i] <= 'Z') {
            letterAnalysis(str, i);
        }
        //ʶ������
        else if (str[i] >= '0' && str[i] <= '9') {
            if (!digitAnalysis(str, i)) { 
                //���������Ļ�
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

//���
void LexicalAnalyzer::Output() {
    for (auto i : LexicalAnalysisOutput) {
        cout << i._type << "  :  " << i._value << endl << endl;
    }
}



//-----------------�ļ���ȡ����ʵ��-----------------//
/*
* ע�⣺
* �����Ƕ��������ж�ȡ���ԣ�ʵ����Ҫ����﷨�������м��������ʵ�֣�
  �߶��߽��дʷ����﷨������Ȼ������м����
*/
string readFile(string filename) {
    std::ifstream file(filename); // ���� txt ���ļ�
    string LexicalAnalysisInput;
    if (file.is_open()) { // ����ļ��Ƿ�ɹ���
        std::string line;
        while (std::getline(file, line)) { // ���ж�ȡ�ļ�����,���з��ᱻ��ȡȻ����
            if (line.length() != 0) {
                cout << line << endl;
                LexicalAnalysisInput += line;
                LexicalAnalysisInput += " ";
            }
        }
        file.close(); // �ر��ļ���
        return LexicalAnalysisInput;
    }
    else {
        std::cerr << "Unable to open file" << std::endl;
        return "";
    }
}

void Test01()
{
    LexicalAnalyzer LexicalMachine;
    string filename;
    cout << "�������ļ����ƣ�" << endl;
    cin >> filename;
    string LexicalAnalysisInput = readFile(filename);
    if (LexicalMachine.Analyze(LexicalAnalysisInput)) {
        LexicalMachine.Output();
        //�����﷨����
    }
    else {//���ִʷ�����
        cout << "Lexical Error" << endl;
    }
}


int main() {
   
    Test01();
    return 0;
}