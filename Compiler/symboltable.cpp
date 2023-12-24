#include "SymbolTable.h"

SymbolTable::SymbolTable() {}

// 添加符号表项
void SymbolTable::addEntry(string name, string type) {
    SymbolTableEntry entry;
    entry.name = name;
    entry.type = type;

    entries.push_back(entry);
}

// 查找符号表项
SymbolTableEntry* SymbolTable::findEntry(string name) {
    for (auto& entry : entries) {
        if (entry.name == name) {
            return &entry;
        }
    }
    return nullptr;
}
