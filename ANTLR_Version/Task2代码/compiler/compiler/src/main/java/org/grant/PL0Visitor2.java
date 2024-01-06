package org.grant;


import org.grant.lex_parser.PL0BaseVisitor;
import org.grant.lex_parser.PL0Parser;
import org.grant.lex_parser.PL0Parser.VarDeclarationContext;
import org.grant.lex_parser.PL0Parser.AssignmentStatementContext;

public class PL0Visitor2 extends PL0BaseVisitor<String> {


    // 回填模式，数值道标深度
    private int backPatchMode = 0;

    // 中间变量 t0 t1.....的指代
    private int tempVarCount = 0;

    //生成代码的次序
    private int instructionCount = 1;


    // 生成一个新的中间变量
    private String newTemp() {
        if(!this.isBackPatchMode())
            return "t" + tempVarCount++;
        else
            return "t" + tempVarCount;
    }

    // 生成 序号 + 指令
    private void emit(String code) {
        if(!this.isBackPatchMode())
            System.out.println(instructionCount++ + ":  " + code);
        else
            instructionCount++;
    }

    // 判断是否为回填模式
    boolean isBackPatchMode(){
        return backPatchMode>0;
    }



    private int labelCount = 0; // 标签计数器

    private String newLabel() {
        return "L" + labelCount++; // 生成标签并增加计数器
    }


    @Override
    public String visitVarDeclaration(VarDeclarationContext ctx) {
        // Handle variable declaration
        System.out.println(ctx.getText());
        return visitChildren(ctx); // Continue to visit child nodes
    }

    @Override
    public String visitAssignmentStatement(AssignmentStatementContext ctx) {
        // Handle assignment statement
        String var = ctx.ID().toString(); // 获取变量名
        String exprCode = visit(ctx.expression()); // 访问表达式并获取其代码
        emit(var + " = " + exprCode); // 生成赋值语句的三地址代码
        return null;
    }

    @Override
    public String visitExpression(PL0Parser.ExpressionContext ctx) {
        if (ctx.getChildCount() == 1) {
            return visit(ctx.getChild(0)); // 直接返回单个子项的代码
        } else {
            String left = visit(ctx.getChild(0)); // 访问左侧子表达式
            String op = ctx.getChild(1).getText(); // 获取操作符
            String right = visit(ctx.getChild(2)); // 访问右侧子表达式
            String temp = newTemp(); // 生成新的临时变量
            emit(temp + " = " + left + " " + op + " " + right); // 生成三地址代码
            return temp;
        }
    }

    @Override
    public String visitTerm(PL0Parser.TermContext ctx)
    {
        if (ctx.getChildCount() == 1) {
            // 仅有一个因子，直接访问它
            return visit(ctx.factor());
        } else {
            // 有多个因子，进行递归处理
            String left = visit(ctx.term()); // 访问左侧项
            String op = ctx.mulOp().getText(); // 获取乘法或除法操作符
            String right = visit(ctx.factor()); // 访问右侧因子
            String temp = newTemp(); // 创建一个新的临时变量
            emit(temp + " = " + left + " " + op + " " + right); // 生成三地址代码
            return temp;
        }
    }

    @Override
    public String visitFactor(PL0Parser.FactorContext ctx)
    {
        if (ctx.ID() != null) {
            // 是一个标识符
            return ctx.ID().getText();
        } else if (ctx.INT() != null) {
            // 是一个整数
            return ctx.INT().getText();
        } else if (ctx.LPAREN() != null) {
            // 是一个括号内的表达式
            return visit(ctx.expression());
        }
        // 其他因子？
        return null;
    }


    @Override
    public String visitConditionalStatement(PL0Parser.ConditionalStatementContext ctx) {
        String conditionCode = visit(ctx.condition()); // 处理条件表达式
        // 开启回填模式
        backPatchMode++;
        // 先保留现在的程序进行地址
        int instructionCountRecord = this.instructionCount;
        // 计算回填地址
        visit(ctx.statement());
        //计算完毕，关闭回填地址,回填深度--
        backPatchMode--;
        // 复原
        int instructionCountTemp =  this.instructionCount;
        this.instructionCount = instructionCountRecord;


        emit("if " + conditionCode + " goto " + (instructionCountTemp + 1)); // 生成条件跳转指令

        // 正式处理then部分的语句
        visit(ctx.statement());

        return null;
    }


    @Override
    public String visitLoopStatement(PL0Parser.LoopStatementContext ctx)
    {

        backPatchMode++;
        // 先保留现在的程序进行地址
        int loopBegin = this.instructionCount;
        // 计算回填地址
        visit(ctx.statement());
        //计算完毕，关闭回填地址,回填深度--
        backPatchMode--;
        // 复原
        int loopEnd =  this.instructionCount;
        this.instructionCount = loopBegin;

        String conditionCode = visit(ctx.condition()); // 生成并评估循环条件
        // String loopEndLabel = newLabel(); // 创建循环结束的标签
        emit("if " + conditionCode + " goto " + (loopEnd+1+1)); // 如果条件为假，则跳到循环结束
        visit(ctx.statement()); // 生成循环体的代码
        emit("goto " + loopBegin); // 循环结束后跳回循环开始

        return null;
    }



    @Override
    public String visitCondition(PL0Parser.ConditionContext ctx) {
        String leftExpr = visit(ctx.expression(0)); // 访问左侧表达式
        String rightExpr = visit(ctx.expression(1)); // 访问右侧表达式
        String op = ctx.relOp().getText(); // 获取关系操作符

        // 返回比较的式子
        return leftExpr + " " + op + " " + rightExpr;
    }



    // You can override more methods as needed
}
