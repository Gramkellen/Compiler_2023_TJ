#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

// ���ű�ṹ
struct SymbolTableEntry {
    string name;
    string type;
    int address;
};

// ������ű���
class SymbolTable {
public:
    SymbolTable() {

    };

    // ���ű��������ӡ�����
    void addEntry(string name, string type);
    SymbolTableEntry* findEntry(string name);

private:
    vector<SymbolTableEntry> entries; // ���ű�����
};

#endif // SYMBOL_TABLE_H
