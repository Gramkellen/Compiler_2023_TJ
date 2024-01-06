// filename:PL0.g4
grammar PL0;

// 词法规则
PROGRAM : 'PROGRAM';
BEGIN   : 'BEGIN';
END     : 'END';
CONST   : 'CONST';
VAR     : 'VAR';
WHILE   : 'WHILE';
DO      : 'DO';
IF      : 'IF';
THEN    : 'THEN';

// 标识符
ID      : LETTER (LETTER | DIGIT)* ;

// 整数
INT     : DIGIT+ ;

// 算符和界符
PLUS    : '+';
MINUS   : '-';
STAR    : '*';
SLASH   : '/';
ASSIGN  : ':=';
EQUAL   : '=';
NOTEQUAL: '<>';
GT      : '>';
GTEQ    : '>=';
LT      : '<';
LTEQ    : '<=';
LPAREN  : '(';
RPAREN  : ')';
SEMI    : ';';
COMMA   : ',';

// 语法规则
program         : programHeader subProgram;
programHeader   : PROGRAM ID;
subProgram      : constDeclaration? varDeclaration? statement;

constDeclaration: CONST constDefinition (COMMA constDefinition)* SEMI;
constDefinition : ID ASSIGN INT;

varDeclaration  : VAR ID (COMMA ID)* SEMI;

compoundStatement: BEGIN statement (SEMI statement)* END;
statement       : assignmentStatement
                | conditionalStatement
                | loopStatement
                | compoundStatement
                | ; // 空语句

assignmentStatement: ID ASSIGN expression;
expression       : (PLUS | MINUS)? term
                | expression addOp term;
term             : factor
                | term mulOp factor;
factor           : ID
                | INT
                | LPAREN expression RPAREN;
addOp            : PLUS
                | MINUS;
mulOp            : STAR
                | SLASH;
conditionalStatement: IF condition THEN statement;
loopStatement    : WHILE condition DO statement;
condition        : expression relOp expression;
relOp            : EQUAL
                | NOTEQUAL
                | LT
                | LTEQ
                | GT
                | GTEQ;

// 辅助规则
fragment LETTER : 'a'..'z' | 'A'..'Z';
fragment DIGIT  : '0'..'9';

// 忽略空白
WS : [ \t\r\n]+ -> skip ;
