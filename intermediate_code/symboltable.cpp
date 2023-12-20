#include "SymbolTable.h"

SymbolTable::SymbolTable() {}

// ��ӷ��ű���
void SymbolTable::addEntry(string name, string type) {
    SymbolTableEntry entry;
    entry.name = name;
    entry.type = type;

    entries.push_back(entry);
}

// ���ҷ��ű���
SymbolTableEntry* SymbolTable::findEntry(string name) {
    for (auto& entry : entries) {
        if (entry.name == name) {
            return &entry;
        }
    }
    return nullptr;
}
