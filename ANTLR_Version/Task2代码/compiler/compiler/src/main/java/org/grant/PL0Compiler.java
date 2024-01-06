package org.grant;

import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.ParseTree;
import org.grant.lex_parser.PL0Lexer;
import org.grant.lex_parser.PL0Parser;

public class PL0Compiler {
    public static void main(String[] args) throws Exception {
        // Assume input is a CharStream of PL0 source code
        CharStream input =
                CharStreams.fromString
                        ("PROGRAM add CONST z:=5, m:=10, b:=15; VAR x,y; BEGIN x:=1; y:=-m+x; WHILE x<5 DO x:=x*1; IF (y+z)-a>0 THEN IF z<0 THEN b:=25; IF a<>0 THEN a:=8; y:=y/(x-1) END");

        // Instantiate the lexer and parser
        PL0Lexer lexer = new PL0Lexer(input);
        CommonTokenStream tokens = new CommonTokenStream(lexer);
        PL0Parser parser = new PL0Parser(tokens);

        // Parse the input and get the parse tree
        ParseTree tree = parser.program();

        // Apply the custom visitor to the parse tree
        PL0Visitor3 visitor = new PL0Visitor3("intermediateCode.txt","symbolTable.txt");
        visitor.visit(tree);
        visitor.printCode();
        visitor.closeWriters();
    }
}
