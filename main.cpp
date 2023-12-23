#include <iostream>
#include <fstream>
#include <vector>
#include "lexical.h"
#include "syntax.h"
//#include "IntermediateCodeGenerator.h"
using namespace std;

//-----------------�ļ���ȡ����ʵ��-----------------//
/*
* ע�⣺
* �����Ƕ��������ж�ȡ���ԣ�ʵ����Ҫ����﷨�������м��������ʵ�֣�
  �߶��߽��дʷ����﷨������Ȼ������м����
*/
string readFile(string filename) {
    ifstream file(filename); // ���� txt ���ļ�
    string LexicalAnalysisInput;
    if (file.is_open()) { // ����ļ��Ƿ�ɹ���
        string line;
        while (getline(file, line)) { // ���ж�ȡ�ļ�����,���з��ᱻ��ȡȻ����
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
        cerr << "Unable to open file" << endl;
        return "";
    }
}


int main() {
    cout << "������Ҫ����ĳ����ļ�·����";
    string inputFilePath;
    getline(cin, inputFilePath);

    // ִ�дʷ�����
    string LexicalAnalysisInput = readFile(inputFilePath);
    // �����ʷ�������
    LexicalAnalyzer lexicalanalyzer;
    if (lexicalanalyzer.Analyze(LexicalAnalysisInput)) {
        lexicalanalyzer.Output();
    }
    else {//���ִʷ�����
        cout << "�ʷ�����" << endl;
        return 1;
    }

    // ִ���﷨����
    vector<Word> LexicalResult = lexicalanalyzer.getlexicalresult();
    // �����﷨������
    SyntaxAnalyzer syntaxanalyzer(LexicalResult);


    // �����м����

    return 0;
}