package org.grant;


import org.grant.lex_parser.PL0BaseVisitor;
import org.grant.lex_parser.PL0Parser.VarDeclarationContext;
import org.grant.lex_parser.PL0Parser.AssignmentStatementContext;

public class PL0Visitor1 extends PL0BaseVisitor<Void> {

    @Override
    public Void visitVarDeclaration(VarDeclarationContext ctx) {
        // Handle variable declaration
        System.out.println("Variable Declaration: " + ctx.getText());
        return visitChildren(ctx); // Continue to visit child nodes
    }

    @Override
    public Void visitAssignmentStatement(AssignmentStatementContext ctx) {
        // Handle assignment statement
        System.out.println("Assignment Statement: " + ctx.getText());
        return visitChildren(ctx); // Continue to visit child nodes
    }

    // You can override more methods as needed
}
