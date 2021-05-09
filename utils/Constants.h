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
    static const char BACKS = '\\';
    static const char SINGLEQUOTE = '\'';
    static const char DOUBLEQUOTE = '\"';
    static const char BACKQUOTE = '`';
    static const char SPACEHAT = '~';
    static const char UNDERLINE = '_';
    static const char AND = '&';
    static const char OR = '|';
    static const char NOT = '!';
    static const char HASH = '#';
    static const char COMMENT = '%';
    static const char AT = '@';
    static const char *BACKSLASH;

    //greek and hebrew
    static const char *ALPHA;
    static const char *BETA;
    static const char *CHI;
    static const char *DELTA;
    static const char *EPSILON;
    static const char *ETA;
    static const char *GAMMA;
    static const char *IOTA;
    static const char *KAPPA;
    static const char *LAMBDA;
    static const char *MU;
    static const char *NU;
    static const char *OMEGA;
    static const char *PHI;
    static const char *PI;
    static const char *PSI;
    static const char *RHO;
    static const char *SIGMA;
    static const char *TAU;
    static const char *THETA;
    static const char *UPSILON;
    static const char *XI;
    static const char *ZETA;
    static const char *DIGAMMA;
    static const char *VAREPSILON;
    static const char *VARKAPPA;
    static const char *VARPHI;
    static const char *VARPI;
    static const char *VARRHO;
    static const char *VARSIGMA;
    static const char *VARTHETA;
    static const char *UDELTA;
    static const char *UGAMMA;
    static const char *ULAMBDA;
    static const char *UOMEGA;
    static const char *UPHI;
    static const char *UPI;
    static const char *UPSI;
    static const char *USIGMA;
    static const char *UTHETA;
    static const char *UUPSILON;
    static const char *UXI;
    

    //special words
    static const char *TIMES;
    static const char *SINGLEVERT;
    static const char *DOUBLEVERT;

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
    static const char *BIGL;
    static const char *BIGR;

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
    static const char *EXP;

    static const char *SQRT;
    static const char *RADICAL;

    static const char *MATHACCENT;

    static const char *OVER;
    static const char *ATOP;
    static const char *ABOVE;
    static const char *CHOOSE;
    static const char *OVERWITHDELIMS;
    static const char *ATOPWITHDELIMS;
    static const char *ABOVEWITHDELIMS;

    static const char *LANGLE;
    static const char *RANGLE;
    static const char *LFLOOR;
    static const char *RFLOOR;
    static const char *LCEIL;
    static const char *RCEIL;
    static const char *SINGLEUPARROW;
    static const char *DOUBLEUPARROW;
    static const char *SINGLEDOWNARROW;
    static const char *DOUBLEDOWNARROW;

    static const char *SUM;
    static const char *PROD;
    static const char *COPROD;
    static const char *INTEGRAL;

    static const char *HAT;
    static const char *TILDE;
    static const char *ODOT;
    static const char *VEC;
    static const char *BAR;
    

};
#include "Constants.cpp"
#endif