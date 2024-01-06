#include "SymbolTable.h"

// SymbolTable::SymbolTable() {}

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

void SymbolTable::saveSymbol() {
    ofstream outFile;
    outFile.open("SymbolTable.txt");
    if (!outFile.is_open()) {
        cerr << "Unable to open file";
        exit(1); // ����ϵͳ��ֹͣ��������
    }
    outFile << "SymbolTable\n";
    outFile << "name    type\n";
    for (auto i : entries) {
        outFile << i.name << "        " << i.type << endl;
    }
    outFile.close();
}
