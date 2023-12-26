#pragma once
#include<iostream>
#include<string>
#include<map>
#include<unordered_map>
#include<unordered_set>
#include<vector>
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
        {"(",           14},
        {")",           15},
        {";",           16},
        {",",           17},
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
    LexicalAnalyzer() {};
    bool findKeyords(const string& str);       //���ҹؼ��ֺ���
    int findOperatorWords(const string& str);  //������������
    void letterAnalysis(string& str, int& i);  //��ʶ��ʶ��
    bool digitAnalysis(string& str, int& i);   //����ʶ��
    bool operatorAnalysis(string& str, int& i); //������
    bool Analyze(string str);                  //�ʷ�����
    void Output();                             //�������
    const vector<Token>& getInput() {
        return LexicalAnalysisOutput;
    }
private:
    vector<Token> LexicalAnalysisOutput;//�������ͣ�����ֵ
};
