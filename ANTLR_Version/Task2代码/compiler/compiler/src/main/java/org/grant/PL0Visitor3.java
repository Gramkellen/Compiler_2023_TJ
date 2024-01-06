package org.grant;


import org.antlr.v4.runtime.tree.TerminalNode;
import org.grant.lex_parser.PL0BaseVisitor;
import org.grant.lex_parser.PL0Parser;
import org.grant.lex_parser.PL0Parser.VarDeclarationContext;
import org.grant.lex_parser.PL0Parser.AssignmentStatementContext;

import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public class PL0Visitor3 extends PL0BaseVisitor<String> {


    private int tempVarCount = 0;   // 中间变量 t0 t1.....的指代
    private int instructionCount = 1; // 记录代码行数
    private int labelCount = 0;       // 回填标签
    private List<String> codeLines = new ArrayList<>();  // 待输出的命令行
    private List<Integer> labelValues = new ArrayList<>();   // 各个回填地址记录
    // 输入文件名
    private PrintWriter codeWriter; // 用于写入中间代码
    private PrintWriter symbolTableWriter; // 用于写入符号表
    // 变量表
    private List<String> varVariables = new ArrayList<>();  //记录代码中的变量
    private List<String> constVariables = new ArrayList<>(); //记录代码中的常量

    public PL0Visitor3(String codeFilename, String symbolTableFilename) {
        try {
            codeWriter = new PrintWriter(new FileWriter(codeFilename, true)); // 追加模式
            symbolTableWriter = new PrintWriter(new FileWriter(symbolTableFilename, true)); // 追加模式
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    // 关闭文件写入
    public void closeWriters() {
        if (codeWriter != null) {
            codeWriter.close();
        }
        if (symbolTableWriter != null) {
            symbolTableWriter.close();
        }
    }

    // 生成一个新的中间变量
    private String newTemp() {
        return "t" + tempVarCount++;
    }

    // 用于生成新的标签并存储其位置
    private int newLabel() {
        labelValues.add(null);  // 初始时标签位置未知
        return labelCount++;
    }

    //   把翻译的中间代码放入待输出列表
    private void emitCode(String code) {
        codeLines.add(code);
        this.instructionCount++;
    }

    // 用于写入符号表
    private void emitSymbol(String symbol) {
        symbolTableWriter.println(symbol);
    }

    // 替换回填地址，并输出代码
    public void printCode() {
        for (int i = 0; i < codeLines.size(); i++) {
            String line = codeLines.get(i);
            // 替换所有的标签引用
            for (int j = 0; j < labelValues.size(); j++) {
                if (labelValues.get(j) != null) {
                    line = line.replace("$" + (j), labelValues.get(j).toString());
                }
            }
            System.out.println(i + ": " + line);
            codeWriter.println(i + ": " + line);
        }
    }

    @Override
    public String visitSubProgram(PL0Parser.SubProgramContext ctx) {
        // 检查是否存在 var 声明
        if (ctx.varDeclaration() == null) {
            System.err.println("Error: No 'var' declarations found in the subprogram");
        } else {
            // 如果存在 var 声明，正常处理它
            visitVarDeclaration(ctx.varDeclaration());
        }

        // 继续处理 subProgram 的其他部分
        if (ctx.constDeclaration() != null) {
            visitConstDeclaration(ctx.constDeclaration());
        }
        if (ctx.statement() != null) {
            visit(ctx.statement());
        }

        return null;
    }


    @Override
    public String visitConstDeclaration(PL0Parser.ConstDeclarationContext ctx) {
        // 确保常量定义列表不为空
        if (ctx.constDefinition() == null || ctx.constDefinition().isEmpty()) {
            System.err.println("Empty constant declaration");
            return null;
        }

        // 遍历所有的常量定义
        for (PL0Parser.ConstDefinitionContext constDef : ctx.constDefinition()) {
            // 检查每个常量定义是否有效
            if (constDef == null) {
                System.err.println("Invalid constant definition in declaration");
                continue; // 跳过无效的常量定义，继续处理其他定义
            }
            visit(constDef); // 处理每个常量定义
        }
        return null;
    }
    @Override
    public String visitConstDefinition(PL0Parser.ConstDefinitionContext ctx) {
        //错误处理 确保标识符和整数值都存在
        if (ctx.ID() == null || ctx.ID().getText() == null) {
            System.err.println("Invalid constant definition: missing identifier");
            return null;
        }
        if (ctx.INT() == null || ctx.INT().getText() == null) {
            System.err.println("Invalid constant definition: missing integer value");
            return null;
        }
        String constName = ctx.ID().getText();
        // 加入常数表中
        if(constVariables.contains(constName))
        {
            System.err.println("Const variable has been defined");
        }
        constVariables.add(constName);
        String constValue = ctx.INT().getText();
        emitSymbol(constName +" "+ constValue + " const");
        return null;
    }
    @Override
    public String visitVarDeclaration(VarDeclarationContext ctx) {
        // 错误处理 确保标识符列表不为空
        if (ctx.ID() == null || ctx.ID().isEmpty()) {
            System.err.println("Empty variable declaration");
            return null;
        }
        for (TerminalNode idNode : ctx.ID()) {
            // 错误处理 检查每个标识符是否有效
            if (idNode == null || idNode.getText() == null) {
                System.err.println("Invalid identifier in variable declaration");
                continue; // 跳过无效的标识符，继续处理其他标识符
            }

            String varName = idNode.getText(); // 获取变量名

            if(varVariables.contains(varName))
            {
                System.err.println("Variable has been defined");
            }
            varVariables.add(varName);

            emitSymbol(varName + " var"); // 生成表示变量声明的代码或注释
        }
        return null;
    }

    @Override
    public String visitCompoundStatement(PL0Parser.CompoundStatementContext ctx) {
        // 确保语句列表不为空
        if (ctx.statement() == null || ctx.statement().isEmpty()) {
            System.err.println("Empty compound statement");
            return null;
        }

        // 遍历复合语句块中的所有语句
        for (PL0Parser.StatementContext stmtCtx : ctx.statement()) {
            if (stmtCtx == null) {
                System.err.println("Invalid statement in compound statement");
                continue; // 跳过无效的语句，继续处理其他语句
            }
            visit(stmtCtx); // 访问并处理每个子语句
        }
        return null;
    }
    @Override
    public String visitAssignmentStatement(AssignmentStatementContext ctx) {
        // 错误处理
        if (ctx.ID() == null || ctx.expression().getChild(0) == null) {
            System.err.println("Syntax error in assignment statement");
            return null;
        }



        // Handle assignment statement
        String var = ctx.ID().toString(); // 获取变量名

        if (!varVariables.contains(var) && !constVariables.contains(var)) {
            System.err.println("Error: Variable '" + var + "' not declared");
            return null;
        }

        if(constVariables.contains(var))
        {
            System.err.println("Error: Const Variable '" + var + "' can not be assigned");
            return null;
        }

        String exprCode = visit(ctx.expression()); // 访问表达式并获取其代码
        emitCode(var + " = " + exprCode); // 生成赋值语句的三地址代码
        return null;
    }

    // todo 前面只有正负的情况
    @Override
    public String visitExpression(PL0Parser.ExpressionContext ctx) {
        if (ctx.getChildCount() == 1) {
            // 错误处理
            if (ctx.getChild(0)== null) {
                System.err.println("Invalid expression: missing child");
                return null;
            }
            return visit(ctx.getChild(0)); // 直接返回单个子项的代码
        }else if (ctx.getChildCount() == 2) {
            // 处理带有前缀的正号或负号的情况
            String op = ctx.getChild(0).getText(); // 获取前缀操作符（+或-）

            // 错误处理
            if (!op.equals("+") && !op.equals("-")) {
                System.err.println("Invalid operator in expression: " + op);
                return null;
            }
            String term = visit(ctx.term());
            // 错误处理
            if (term == null) {
                System.err.println("Invalid term in expression");
                return null;
            }
            String temp = newTemp();
            if (op.equals("-")) {
                emitCode(temp + " = -" + term); // 处理负号
            } else {
                emitCode(temp + " = " + term); // 正号，不需要改变
            }
            return temp;
        } else if (ctx.getChildCount() == 3) {
            String left = visit(ctx.getChild(0)); // 访问左侧子表达式
            String op = ctx.getChild(1).getText(); // 获取操作符
            String right = visit(ctx.getChild(2)); // 访问右侧子表达式
            // 错误处理
            if (left == null || right == null) {
                System.err.println("Invalid operands in expression");
                return null;
            }

            String temp = newTemp(); // 生成新的临时变量
            emitCode(temp + " = " + left + " " + op + " " + right); // 生成三地址代码
            return temp;
        } else {
            //错误处理
            System.err.println("Unexpected number of children in expression: " + ctx.getChildCount());
            return null;
        }
    }

    @Override
    public String visitTerm(PL0Parser.TermContext ctx)
    {
        if (ctx.getChildCount() == 1) {
            // 错误处理 确保因子节点存在
            if (ctx.factor().getChild(0) == null) {
                System.err.println("Invalid term: missing factor");
                return null;
            }
            // 仅有一个因子，直接访问它
            return visit(ctx.factor());
        } else if (ctx.getChildCount() == 3) {
            // 错误处理 确保左侧项和右侧因子都有效
            if (ctx.term().getChild(0) == null || ctx.factor().getChild(0) == null) {
                System.err.println("Invalid term: missing term or factor");
                return null;
            }
            // 有多个因子，进行递归处理
            String left = visit(ctx.term()); // 访问左侧项
            String op = ctx.mulOp().getText(); // 获取乘法或除法操作符
            String right = visit(ctx.factor()); // 访问右侧因子
            String temp = newTemp(); // 创建一个新的临时变量
            emitCode(temp + " = " + left + " " + op + " " + right); // 生成三地址代码
            return temp;
        }else{
            // 如果子节点数量既不是1也不是3，报告错误
            System.err.println("Unexpected number of children in term: " + ctx.getChildCount());
            return null;
        }
    }

    @Override
    public String visitFactor(PL0Parser.FactorContext ctx)
    {
        if (ctx.ID() != null) {
            // 是一个标识符
            String varName = ctx.ID().getText();
            if (!varVariables.contains(varName) && !constVariables.contains(varName)) {
                System.err.println("Error: Variable '" + varName + "' not declared");
                return null;
            }
            return varName;
        } else if (ctx.INT() != null) {
            // 是一个整数
            return ctx.INT().getText();
        } else if (ctx.LPAREN() != null) {
            //错误处理
            if (ctx.expression() == null) {
                System.err.println("Invalid factor: missing expression inside parentheses");
                return null;
            }
            // 是一个括号内的表达式
            return visit(ctx.expression());
        }
        System.err.println("Invalid or unsupported factor type");
        return null;
    }


    @Override
    public String visitConditionalStatement(PL0Parser.ConditionalStatementContext ctx) {
        // 错误处理 检查条件表达式是否存在
        if (ctx.condition().getChild(0) == null) {
            System.err.println("Invalid conditional statement: missing condition");
            return null;
        }

        int label1 = newLabel(); // 用于条件成立时的跳转
        int label2 = newLabel(); // 用于条件不成立时的跳转

        // 生成条件检查代码
        String conditionCode = visit(ctx.condition()); // 假设这返回条件表达式的中间代码
        emitCode("if " + conditionCode + " goto $" + label1);

        // 生成跳转到else部分的代码
        emitCode("goto $" + label2);

        // 给第一个标签赋值
        labelValues.set(label1,instructionCount-1);

        // 错误处理 检查语句是否存在
        if (ctx.statement().getChild(0) == null) {
            System.err.println("Invalid conditional statement: missing statement");
            return null;
        }

        visit(ctx.statement());

        // 给第二个标签赋值
        labelValues.set(label2,instructionCount-1);

        return null;
    }


    @Override
    public String visitLoopStatement(PL0Parser.LoopStatementContext ctx)
    {
        // 错误处理 检查条件表达式是否存在
        if (ctx.condition().getChild(0) == null) {
            System.err.println("Invalid loop statement: missing condition");
            return null;
        }
        // 错误处理 检查循环体语句是否存在
        if (ctx.statement().getChild(0) == null) {
            System.err.println("Invalid loop statement: missing loop body");
            return null;
        }

        int labelBegin = newLabel(); // while begin
        int labelEnd = newLabel(); // while jump out
        int labelStatement = newLabel();

        labelValues.set(labelBegin,instructionCount-1); //在while最开始的时候进行赋值
        // 生成条件检查代码
        String conditionCode = visit(ctx.condition()); // 假设这返回条件表达式的中间代码
        emitCode("if " + conditionCode + " goto $" + labelStatement);

        // 生成跳转到else部分的代码
        emitCode("goto $" + labelEnd);
        labelValues.set(labelStatement,instructionCount-1);
        // 访问循环体
        visit(ctx.statement());
        // 循环体跳转回初始时刻
        emitCode("goto $" + labelBegin);
        // 记录循环结束的代码
        labelValues.set(labelEnd,instructionCount-1);
        return null;

    }



    @Override
    public String visitCondition(PL0Parser.ConditionContext ctx) {
        // 错误处理 确保左侧和右侧的表达式存在
        if (ctx.expression(0).getChild(0) == null || ctx.expression(1).getChild(0) == null) {
            System.err.println("Invalid condition: one or more expressions are missing");
            return null;
        }

        String leftExpr = visit(ctx.expression(0)); // 访问左侧表达式
        // 错误处理
        if (leftExpr == null) {
            System.err.println("Invalid left expression in condition");
            return null;
        }

        String rightExpr = visit(ctx.expression(1)); // 访问右侧表达式
        // 错误处理
        if (rightExpr == null) {
            System.err.println("Invalid right expression in condition");
            return null;
        }

        if (ctx.relOp().getChild(0) == null || ctx.relOp().getText() == null) {
            System.err.println("Invalid condition: missing or invalid relational operator");
            return null;
        }

        String op = ctx.relOp().getText(); // 获取关系操作符

        // 返回比较的式子
        return leftExpr + " " + op + " " + rightExpr;
    }


}
