#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "prot.h"

TokenType ctyp[256];    // character's tokentype
Token token;
FILE *fp;

struct {
    char *ktext;
    TokenType ktokenType;
} KwdTable[] = {
    { "="     ,  ASSIGN   },
    {"\'"     ,  S_QUOTES }, { "\""  ,   D_QUOTES  },
    { ":"     ,  COLON    }, { ";"   ,   SEMICOLON },
    { ","     ,  COMMA    }, { "."   ,   DOT       },
    { "class" ,  CLASS    },
    { "func"  ,  FUNC     },
    { "if"    ,  IF       }, { "elif",   ELIF      }, { "else", ELSE },
    { "("     ,  LPAREN   }, { ")"   ,   RPAREN    },
    { "["     ,  LSQUARE  }, { "]"   ,   RSQUARE   },
    { "{"     ,  LBRACE   }, { "}"   ,   RBRACE    },
    { "+"     ,  ADD      }, { "-"   ,   SUB       },
    { "*"     ,  MULT     }, { "/"   ,   DIVI      },
    { "%"     ,  MOD      },
    { "<"     ,  LESS     }, { ">"   ,   GREAT     },
    { "=="    ,  EQUAL    }, { "!="  ,   NOT_EQ    },
    { "<="    ,  LESS_EQ  }, { ">="  ,   GREAT_EQ  },
    { "&&"    ,  AND      }, { "||"  ,   OR        },
    { "print" ,  PRINT    }, 
    { ""      ,  END_LIST },
};


int main(int argc, char* argv[])
{

    if (argv[1] == NULL) { exit(1); }
    else {
        fp = fopen("test.txt", "r");

        initCharType();

        printf("[ ");

        while (token.tokenType != E_O_F) {
            token = getToken();
            printf("%s ",convertTokenType(token.tokenType));
        }

        printf("]\n");

        fclose(fp);
    }

    return 0;
}

void initCharType(void)
{
    int i;

    for (i =  0 ; i < 256; i++) { ctyp[i] = OTHERS; }
    for (i = '0'; i <= '9'; i++) { ctyp[i] = NUMBER; }
    for (i = 'a'; i <= 'z'; i++) { ctyp[i] = LETTER; }
    for (i = 'A'; i <= 'Z'; i++) { ctyp[i] = LETTER; }
    ctyp['=']  = ASSIGN   ;
    ctyp['\''] = S_QUOTES ;  ctyp['"'] = D_QUOTES  ;
    ctyp[':']  = COLON    ;  ctyp[';'] = SEMICOLON ;
    ctyp[',']  = COMMA    ;  ctyp['.'] = DOT       ;
    ctyp['+']  = ADD      ;  ctyp['-'] = SUB       ;
    ctyp['*']  = MULT     ;  ctyp['/'] = DIVI      ;
    ctyp['%']  = MOD      ;
    ctyp['<']  = LESS     ;  ctyp['>'] = GREAT     ;
}

Token getToken(void)
{
    Token tkn = {NULLTYPE, "", 0};
    char *p = tkn.text, *p_31 = p+IDENT_SIZE, *p_100 = p+TEXT_SIZE;
    static int ch = ' ';

    while (isspace(ch)) { ch = nextChar(); }
    if (ch == EOF) { tkn.tokenType = E_O_F; return tkn; }

    switch (ctyp[ch]) {
        case LETTER:
            for ( ; ctyp[ch] == LETTER || ctyp[ch] == NUMBER; ch = nextChar()) {
                if (p < p_31) *p++ = ch;
                *p = '\0';
            }
            break;
        case NUMBER:
            for ( ; ctyp[ch] == NUMBER || ctyp[ch] == DOT; ch = nextChar()) {
                *p++ = ch;
                *p = '\0';
            }
            break;
        case ASSIGN:
            for ( ; ctyp[ch] == ASSIGN; ch = nextChar()) {
                *p++ = ch;
                *p = '\0';
            }
            break;
        case S_QUOTES:
            for (ch = nextChar(); ch != EOF && ch != '\n' && ch != '\''; ch = nextChar()) {
                if (p >= p_100) { printf("String literal is too long!\n"); exit(1); }
                else *p++ = ch;
            }
            *p = '\0';
            tkn.tokenType = STRING;
            ch = nextChar();
            break;
        case D_QUOTES:
            for (ch = nextChar(); ch != EOF && ch != '\n' && ch != '\"'; ch = nextChar()) {
                if (p >= p_100) { printf("String literal is too long!\n"); exit(1); }
                else *p++ = ch;
            }
            *p = '\0';
            tkn.tokenType = STRING;
            ch = nextChar();
            break;
        default:
            *p++ = ch; ch = nextChar();
            *p = '\0';
    }

    tkn = setTokenType(tkn);

    return tkn;
}

int nextChar(void)
{
    static int c = 0;

    if (c == EOF) return c;

    c = fgetc(fp);
    if (c == EOF) { return c; }

    return c;
}

Token setTokenType(Token t)
{
    char* s = t.text;

    for (int i=0; KwdTable[i].ktokenType != END_LIST; i++) {
        if (strcmp(KwdTable[i].ktext, s) == 0) {
            t.tokenType = KwdTable[i].ktokenType;
            return t;
        }
    }

    if (t.tokenType != STRING) {
        if      (ctyp[(wint_t)*s] == LETTER) {
            t.tokenType = NAME;
        }
        else if (ctyp[(wint_t)*s] == NUMBER) {
            t.tokenType = NUMBER; t.value = atof(s);
        }
    }

    return t;
}


char *convertTokenType(TokenType tp)
{
    switch (tp) {
        case NAME:      return "NAME";      break;
        case ASSIGN:    return "ASSIGN";    break;
        case LETTER:    return "LETTER";    break;
        case STRING:    return "STRING";    break;
        case NUMBER:    return "NUMBER";    break;
        case INDENT:    return "INDENT";    break;
        case S_QUOTES:  return "S_QUOTES";  break;
        case D_QUOTES:  return "D_QUOTES";  break;
        case COLON:     return "COLON";     break;
        case SEMICOLON: return "SEMICOLON"; break;
        case COMMA:     return "COMMA";     break;
        case DOT:       return "DOT";       break;
        case CLASS:     return "CLASS";     break;
        case FUNC:      return "FUNC";      break;
        case IF:        return "IF";        break;
        case ELIF:      return "ELIF";      break;
        case ELSE:      return "ELSE";      break;
        case LPAREN:    return "LAPAREN";   break;
        case RPAREN:    return "RPAREN";    break;
        case LSQUARE:   return "LSQUARE";   break;
        case RSQUARE:   return "RSQUARE";   break;
        case LBRACE:    return "LBRACE";    break;
        case RBRACE:    return "RBRACE";    break;
        case ADD:       return "PLUS";       break;
        case SUB:       return "MINUS";       break;
        case MULT:      return "MULT";      break;
        case DIVI:      return "DIVI";      break;
        case MOD:       return "MOD";       break;
        case LESS:      return "LESS";      break;
        case GREAT:     return "GREAT";     break;
        case EQUAL:     return "EQUAL";     break;
        case NOT_EQ:    return "NOT_EQ";    break;
        case LESS_EQ:   return "LESS_EQ";   break;
        case GREAT_EQ:  return "GREAT_EQ";  break;
        case AND:       return "&&";        break;
        case OR:        return "||";        break;
        case PRINT:     return "PRINT";     break;
        case OTHERS:    return "OTHERS";    break;
        case NULLTYPE:  return "NULLTYPE";  break;
        case E_O_F:     return "EOF";     break;
        default:        return NULL;
    }
}