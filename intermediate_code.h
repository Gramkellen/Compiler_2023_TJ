#ifndef INTERMEDIATE_CODE_GENERATOR_H
#define INTERMEDIATE_CODE_GENERATOR_H

#include <iostream>
#include <vector>
#include <string>
#include "symboltable.h" // 包含符号表的头文件

// 中间代码结构
struct IntermediateCode {
    string opt;
    string op1;
    string op2;
    string re;
};

// 中间代码生成器类
class IntermediateCodeGenerator {
public:
    IntermediateCodeGenerator();

    // 生成不同类型的中间代码指令
    void generateAssignment(string target, string source);
    void generateArithmeticOperation(string target, string operand1, string operand2, string operation);

    // 获取生成的中间代码
    vector<IntermediateCode> getIntermediateCode() { return imdCode; };

private:
    vector<IntermediateCode> imdCode; // 存储生成的中间代码
    SymbolTable symbolTable; // 符号表

};

#endif // INTERMEDIATE_CODE_GENERATOR_H
