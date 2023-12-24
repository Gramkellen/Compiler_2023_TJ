#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

// 符号表结构
struct SymbolTableEntry {
    string name;
    string type;
    int address;
};

// 定义符号表类
class SymbolTable {
public:
    SymbolTable() {

    };

    // 符号表操作如添加、查找
    void addEntry(string name, string type);
    SymbolTableEntry* findEntry(string name);

private:
    vector<SymbolTableEntry> entries; // 符号表数组
};

#endif // SYMBOL_TABLE_H
