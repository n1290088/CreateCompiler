#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define IDENT_SIZE 31
#define TEXT_SIZE 100

typedef enum {
    NAME,       // identifier

    ASSIGN,     // "="

    LETTER,     // letter

    STRING,     // string
    NUMBER,     // real number

    INDENT,     // indent

    S_QUOTES,   // '
    D_QUOTES,   // "

    COLON,      // ":"
    SEMICOLON,  // ";"

    COMMA,      // ","
    DOT,        // "."

    CLASS,      // "class"

    FUNC,       // function

    IF,         // if
    ELIF,       // elif
    ELSE,       // else

    LPAREN,     // "("
    RPAREN,     // ")"
    LSQUARE,    // "["
    RSQUARE,    // "]"
    LBRACE,     // "{"
    RBRACE,     // "}"

    ADD,        // "+"
    SUB,        // "-"
    MULT,       // "*"
    DIVI,       // "/"
    MOD,        // "%"

    LESS,       // "<"
    GREAT,      // ">"

    EQUAL,      // "=="
    NOT_EQ,     // "!="
    LESS_EQ,    // "<="
    GREAT_EQ,   // ">="

    AND,        // "&&"
    OR,         // "||"

    PRINT,      // printf, puts

    OTHERS,     // others
    NULLTYPE,   // null
    END_LIST,   // ""
    E_O_F,      // End of Token
} TokenType;


typedef struct {
    TokenType tokenType;
    char      text[IDENT_SIZE+1];
    int    value;
} Token;