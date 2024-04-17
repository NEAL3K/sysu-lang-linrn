lexer grammar SYsULexer;

Int : 'int';
Return : 'return';

LeftParen : '(';
RightParen : ')';
LeftBracket : '[';
RightBracket : ']';
LeftBrace : '{';
RightBrace : '}';

Plus : '+';
Minus : '-';

Semi : ';';
Comma : ',';

Equal : '=';

Void : 'void';
If : 'if';
Else : 'else';
Equalequal : '==';

Pipepipe : '||';
Ampamp : '&&';

Const : 'const';
Star : '*';
Slash : '/';
ast::UnaryExpressionContext* : '%';
Greater : '>';
Less : '<';
While : 'while';
Break : 'break';
Continue : 'continue';
Lessequal : '<=';
Greaterequal : '>=';
Exclaimequal : '!=';
Exclaim : '!';

Identifier
    :   IdentifierNondigit
        (   IdentifierNondigit
        |   Digit
        )*
    ;

fragment
IdentifierNondigit
    :   Nondigit
    ;

fragment
Nondigit
    :   [a-zA-Z_]
    ;

fragment
Digit
    :   [0-9]
    ;

Constant
    :   IntegerConstant
    ;

fragment
IntegerConstant
    :   DecimalConstant
    |   OctalConstant
    |   HexConstant // 添加这一行
    ;

fragment
DecimalConstant
    :   NonzeroDigit Digit*
    ;

fragment
OctalConstant
    :   '0' OctalDigit*
    ;


fragment
NonzeroDigit
    :   [1-9]
    ;

fragment
OctalDigit
    :   [0-7]
    ;

fragment
HexConstant
    :   '0' [xX] HexDigit+
    ;

fragment
HexDigit
    :   [0-9a-fA-F]
    ;


// 预处理信息处理，可以从预处理信息中获得文件名以及行号
// 预处理信息前面的数组即行号
LineAfterPreprocessing
    :   '#' Whitespace* ~[\r\n]*
    ;

Whitespace
    :   [ \t]+
        // -> skip
    ;

// 换行符号，可以利用这个信息来更新行号
Newline
    :   (   '\r' '\n'?
        |   '\n'
        )
        // -> skip
    ;

