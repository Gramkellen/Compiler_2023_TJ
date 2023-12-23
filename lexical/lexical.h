#pragma once
#include<iostream>
#include<string>
#include<map>
#include<unordered_map>
#include<unordered_set>
#include<vector>
using namespace std;


//�ؼ��ֱ�ʶ
enum KeyWordTags
{
    KW_PROGRAM,
    KW_BEGIN,
    KW_END,
    KW_CONST,
    KW_VAR,
    KW_WHILE,
    KW_DO,
    KW_IF,
    KW_THEN
};
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
        {"=",           1},
        {":=",          2},
        {"+",           3},
        {"-",           4},
        {"*",           5},
        {"/",           6},
        {"#",           7},
        {"<",           8},
        {"<=",          9},
        {">",           10},
        {">=",          11},
        {";",           12},
        {",",           13},
        {"(",           14},
        {")",           15}
};

//���ʱ��
struct Word {
    Word(string type = "", string value = "") :_type(type), _value(value) {}
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
    vector<Word> getlexicalresult() { return LexicalAnalysisOutput; }
private:
    vector<Word> LexicalAnalysisOutput;//�������ͣ�����ֵ
};
