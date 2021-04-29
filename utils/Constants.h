#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_
class Constants
{
public:
    static const char GREATERTHAN = '>';
    static const char LESSTHAN = '<';
    static const char EQUAL = '=';
    static const char COMMA = ',';
    static const char DOT = '.';
    static const char COLON = ':';
    static const char SEMICOLON = ';';
    static const char OPENBRACE = '{';
    static const char CLOSEBRACE = '}';
    static const char OPENPARENTHESIS = '(';
    static const char CLOSEPARENTHESIS = ')';
    static const char OPENBRACKET = '[';
    static const char CLOSEBRACKET = ']';
    static const char ADD = '+';
    static const char SUB = '-';
    static const char MULT = '*';
    static const char POWER = '^';
    static const char SLASH = '/';
    static const char BACKSLASH = '\\';
    static const char SINGLEQUOTE = '\'';
    static const char DOUBLEQUOTE = '\"';
    static const char BACKQUOTE = '`';
    static const char SPACEHAT = '~';
    static const char UNDERLINE = '_';
    static const char AND = '&';
    static const char OR = '|';
    static const char NOT = '!';
    static const char HASH = '#';

    //special words
    static const char *vERT;
    static const char *VERT;

    static const char *RULE;
    static const char *PENALTY;
    static const char *DISCRETIONARY;
    static const char *OPENOUT;
    static const char *CLOSEOUT;
    static const char *WRITE;
    static const char *IMMEDIATE;
    static const char *SHIPOUT;
    static const char *SPECIAL;

    static const char *LEFT;
    static const char *RIGHT;

    static const char *GLUE;
    static const char *SMALLSKIP;
    static const char *MEDSKIP;
    static const char *VSKIP;
    static const char *HSKIP;
    static const char *ENSKIP;
    static const char *LDOTS;
    static const char *FRENCHSPACING;
    static const char *NONFRENCHSPACING;
    static const char *TENRM;
    static const char *KERN;
    static const char *SPACESKIP;
    static const char *XSPACESKIP;

    static const char *MATHCHOICE;
    static const char *RELPENALTY;

    static const char *FRAC;

    static const char *VCENTER;

    static const char *OVERLINE;
    static const char *UNDERLINEW;

    static const char *SQRT;
    static const char *RADICAL;

    static const char *MATHACCENT;

    static const char *OVER;
    static const char *ATOP;
    static const char *ABOVE;
    static const char *OVERWITHDELIMS;
    static const char *ATOPWITHDELIMS;
    static const char *ABOVEWITHDELIMS;
};
#include "Constants.cpp"
#endif