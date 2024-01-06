// Generated from C:/Works/����/����ԭ��/��ҵ/compiler/compiler/src/main/java/org/grant/PL0.g4 by ANTLR 4.13.1
package org.grant.lex_parser;
import org.antlr.v4.runtime.tree.ParseTreeVisitor;

/**
 * This interface defines a complete generic visitor for a parse tree produced
 * by {@link PL0Parser}.
 *
 * @param <T> The return type of the visit operation. Use {@link Void} for
 * operations with no return type.
 */
public interface PL0Visitor<T> extends ParseTreeVisitor<T> {
	/**
	 * Visit a parse tree produced by {@link PL0Parser#program}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitProgram(PL0Parser.ProgramContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#programHeader}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitProgramHeader(PL0Parser.ProgramHeaderContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#subProgram}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitSubProgram(PL0Parser.SubProgramContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#constDeclaration}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitConstDeclaration(PL0Parser.ConstDeclarationContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#constDefinition}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitConstDefinition(PL0Parser.ConstDefinitionContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#varDeclaration}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitVarDeclaration(PL0Parser.VarDeclarationContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#compoundStatement}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitCompoundStatement(PL0Parser.CompoundStatementContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#statement}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitStatement(PL0Parser.StatementContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#assignmentStatement}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAssignmentStatement(PL0Parser.AssignmentStatementContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#expression}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitExpression(PL0Parser.ExpressionContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#term}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTerm(PL0Parser.TermContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#factor}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFactor(PL0Parser.FactorContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#addOp}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAddOp(PL0Parser.AddOpContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#mulOp}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitMulOp(PL0Parser.MulOpContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#conditionalStatement}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitConditionalStatement(PL0Parser.ConditionalStatementContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#loopStatement}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitLoopStatement(PL0Parser.LoopStatementContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#condition}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitCondition(PL0Parser.ConditionContext ctx);
	/**
	 * Visit a parse tree produced by {@link PL0Parser#relOp}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitRelOp(PL0Parser.RelOpContext ctx);
}