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
    while (((str[i] >= 'A' && str[i] <= 'Z')  || (str[i] >= 'a' && str[i] <= 'z')) || (str[i] >= '0' && str[i] <= '9')) {
        tempWord += str[i];
        i++;
    }
    i--;
    if (findKeyords(tempWord)) {
        beforeToken = currentToken;
        currentToken = Token("KEYWORDS", tempWord); //�ؼ���
    }
    else {
        beforeToken = currentToken;
        currentToken = Token("IDENTIFIER", tempWord); //��ʶ��
    }
}

//����ʶ��
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
    //���ֺ�������ַ���������
    if ((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z')) {
        beforeToken = currentToken;
        cout << "��" << filerow << "�У���" << filecol << "�г��ִʷ����󣡱�ʶ����λΪ���֣���������ƴд����" << endl;
        currentToken = Token("Error","Error");
        return;
    }
    i--;
    beforeToken = currentToken;
    currentToken = Token("NUMBER", tempWord);
}

//��������޷����
void LexicalAnalyzer::operatorAnalysis(string& str, int& i) {
    string tempWord;
    tempWord += str[i];
    //�ж��ǲ������/���
    int tempid = findOperatorWords(tempWord);
    if (tempid) {
        tempWord += str[i + 1];
        int tempid2 = findOperatorWords(tempWord);
        //˵���Ƕ��������
        if (tempid2) { 
            i++;
            beforeToken = currentToken;
            currentToken = Token("TwoOP", tempWord);
        }
        //ֻ����һԪ��
        else {
            tempWord.clear();
            tempWord += str[i];
            beforeToken = currentToken;
            currentToken = Token("OneOP", tempWord);
        }
    }
    //�Ǳ�ʶ�������ֺ����/����������Ϊ���ִʷ�ƴд���󣬷���false
    else {
        tempWord += str[i + 1];
        int tempid2 = findOperatorWords(tempWord);
        //����ֱ���Ƕ�Ԫ�ģ�Ȼ��һԪû�ж�Ӧ
        if (tempid2) {
            //�жϹ�ϵ���㷨
            beforeToken = currentToken;
            i++;
            currentToken = Token("TwoOP", tempWord);;
        }
        else
        {
            beforeToken = currentToken;
            cout << "��" << filerow << "�У���" << filecol << "�г���ƴд����" << endl;
            currentToken = Token("Error", "Error");
            
        }
    }
}

//�ʷ�����
void LexicalAnalyzer::Analyze() {
    int i = filecol;
    for (; i < currentline.length(); i++) {
        if (currentline[i] == ' ' || currentline[i] == 9) {//�ո��tab
            continue; 
        }
        //ʶ��ؼ��� or  ��ʶ��
        else if ((currentline[i] >= 'A' && currentline[i] <= 'Z') ||
            (currentline[i] >= 'a' && currentline[i] <= 'z')) {
            letterAnalysis(currentline, i);
            break;
        }
        //ʶ������
        else if (currentline[i] >= '0' && currentline[i] <= '9') {
             digitAnalysis(currentline, i);
             break;
        }
        else {
            // �ж������
             operatorAnalysis(currentline, i);
             break;
        }
    }
    //��һ���жϽ�����
    //Panic ģʽ���� Error �� Panic ����
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

//��ȡ�ļ�
void LexicalAnalyzer::readFile() {
    if (file.is_open()) {
        string line;
        bool isClose = true;
        while(std::getline(file, line)) { // ���ж�ȡ�ļ�����,���з��ᱻ��ȡȻ����
            if (line.length() != 0) {
                currentline = line;
                filerow++;
                isClose = false;
                break;
            }
        }
        if (isClose) {
            file.close(); // �ر��ļ���
        }  
    }
    else
    {
        cout << "���ļ�����" << endl;
        exit(EXIT_FAILURE);              //ֱ���˳��ļ�����
    }
}

//�����к�
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


//-----------------�ļ���ȡ����ʵ��-----------------//
/*
* ע�⣺
* �����Ƕ��������ж�ȡ���ԣ�ʵ����Ҫ����﷨�������м��������ʵ�֣�
  �߶��߽��дʷ����﷨������Ȼ������м����
*/
//string readFile(string filename) {
//    std::ifstream file(filename); // ���� txt ���ļ�
//    string LexicalAnalysisInput;
//    if (file.is_open()) { // ����ļ��Ƿ�ɹ���
//        std::string line;
//        while (std::getline(file, line)) { // ���ж�ȡ�ļ�����,���з��ᱻ��ȡȻ����
//            if (line.length() != 0) {
//                LexicalAnalysisInput += line;
//                LexicalAnalysisInput += " ";
//            }
//        }
//        file.close(); // �ر��ļ���
//        return LexicalAnalysisInput;
//    }
//    else {
//        std::cerr << "Unable to open file" << std::endl;
//        return "";
//    }
//}


