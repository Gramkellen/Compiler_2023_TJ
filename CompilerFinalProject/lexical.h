#pragma once
#include<iostream>
#include<string>
#include<map>
#include<unordered_map>
#include<unordered_set>
#include<vector>
#include<fstream>
using namespace std;

void Test01();
string readFile(string filename = "D:\\Files\\SoftWare_5\\����ԭ��\\��ĩ\\lexical\\Compiler_2023_TJ\\lexical\\program.txt");
//ʹ��hash�ײ�������KEYWORDS����


const unordered_set<string> KEYWORDS = {
    "PROGRAM",
    "BEGIN",
    "END",
    "CONST",
    "VAR",
    "WHILE",
    "DO",
    "IF",
    "THEN"
};

//����Լ��������ͽ��޷������ǵ�������������ȼ�����������ʱʹ��map
const unordered_map<string, int> OPERATORWORD = {
        {"+",           1},
        {"-",           2},
        {"*",           3},
        {"/",           4},
        {":=",          5},

        {"=",           6},
        {"<>",          7},
        {">",           8},
        {">=",          9},
        {"<",           10},
        {"<=",          11},
        {"(",           12},
        {")",           13},
        {";",           14},
        {",",           15},
};

//���ʱ��
struct Token {
    Token(string type = "", string value = "") :_type(type), _value(value) {}
    string _type;      //��������
    string _value;    //���ʵ�ֵ
};

//-----------------�ʷ�����������-----------------//
/*�﷨��������
* function : �����봮���дʷ�������ʶ�������token
* ������ : ��ⵥ��ƴд�Ƿ����ͷǷ��ַ�
*/

class LexicalAnalyzer {
public:
    std::ifstream file;                    //��ȡ�ļ�����
    LexicalAnalyzer(string filename):file(filename),filerow(0),filecol(0) {};
    bool findKeyords(const string& str);       //���ҹؼ��ֺ���
    int findOperatorWords(const string& str);  //������������
    void letterAnalysis(string& str, int& i);  //��ʶ��ʶ��
    void digitAnalysis(string& str, int& i);   //����ʶ��
    void operatorAnalysis(string& str, int& i); //������
    void Analyze();                  //�ʷ�����
    void readFile();
    Token getToken(int choice = 1);
    int getLine();
private:
    Token beforeToken;                    //��һ��Token
    Token currentToken;                  //�������ͣ�����ֵ
    int filerow;                         //��һ��
    int filecol;                         //��һ��
    string currentline;                  //�洢��ǰ��

};
