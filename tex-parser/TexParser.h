#ifndef _TEXPARSER_H_
#define _TEXPARSER_H_
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "../utils/Constants.h"

class TexParser
{
private:
    const std::string STARTEXP = ".BL";
    const std::string ENDEXP = ".EL";

    std::unordered_map<std::string, std::string> translate = {
        {STARTEXP, ".EQ"},
        {ENDEXP, ".EN"},
        {Constants::SINGLEVERT, "|"},
        {Constants::DOUBLEVERT, "||"},
        {Constants::TIMES, "times"},
        {Constants::LANGLE, "left <"},
        {Constants::RANGLE, "right >"},
        {Constants::LFLOOR, "\\(lf"},
        {Constants::RFLOOR, "\\(rf"},
        {Constants::LCEIL, "\\(lc"},
        {Constants::RCEIL, "\\(rc"},
        {Constants::SINGLEUPARROW, "\\(ua"},
        {Constants::SINGLEDOWNARROW, "\\(da"},
        {Constants::SUM, "sum"},
        {Constants::PROD, "prod"},
        {Constants::COPROD, "coprod"},
        {Constants::INTEGRAL, "int"},
        {Constants::BACKSLASH, "\\"},
        {"\\|", "||"},
        {"\\{", "{"},
        {"\\}", "}"},
        {std::string(1, Constants::POWER), "sup"}};

    std::unordered_map<std::string, std::string> translateLetters = {
        {Constants::ALPHA, "alpha"},
        {Constants::BETA, "beta"},
        {Constants::CHI, "chi"},
        {Constants::DELTA, "delta"},
        {Constants::EPSILON, "epsilon"},
        {Constants::ETA, "eta"},
        {Constants::GAMMA, "gamma"},
        {Constants::IOTA, "iota"},
        {Constants::KAPPA, "kappa"},
        {Constants::LAMBDA, "lambda"},
        {Constants::MU, "mu"},
        {Constants::NU, "nu"},
        {Constants::OMEGA, "omega"},
        {Constants::PHI, "phi"},
        {Constants::PI, "pi"},
        {Constants::PSI, "psi"},
        {Constants::RHO, "rho"},
        {Constants::SIGMA, "sigma"},
        {Constants::TAU, "tau"},
        {Constants::THETA, "theta"},
        {Constants::UPSILON, "upsilon"},
        {Constants::XI, "xi"},
        {Constants::ZETA, "zeta"},
        {Constants::DIGAMMA, "digamma"}, //unknown
        {Constants::VAREPSILON, "\\(*e"},
        {Constants::VARKAPPA, "varkappa"}, //unknown
        {Constants::VARPHI, "varphi"},     //unknown
        {Constants::VARPI, "varpi"},       //unknown
        {Constants::VARRHO, "varrho"},     //unknown
        {Constants::VARSIGMA, "\\(ts"},
        {Constants::UDELTA, "DELTA"},
        {Constants::UGAMMA, "GAMMA"},
        {Constants::ULAMBDA, "LAMBDA"},
        {Constants::UOMEGA, "OMEGA"},
        {Constants::UPHI, "PHI"},
        {Constants::UPI, "PI"},
        {Constants::UPSI, "PSI"},
        {Constants::USIGMA, "SIGMA"},
        {Constants::UTHETA, "THETA"},
        {Constants::UUPSILON, "UPSILON"},
        {Constants::EXP, "exp"},
        {Constants::UXI, "XI"},
        {Constants::BULLET, "\\(bu"},
        {Constants::BOX, "\\(sq"},
        {Constants::CIRC, "\\(de"},
        {Constants::AST, "\\(**"},
        {Constants::S, "\\(sc"},
        {Constants::DAG, "\\(dg"},
        {Constants::DAGGER, "\\(dg"},
        {Constants::DDAG, "\\(dd"},
        {Constants::DDAGGER, "\\(dd"},
        {Constants::SETMINUS, "\\"},
        {Constants::SURD, "\\(sr"},
        {Constants::EQUIV, "\\(=="},
        {Constants::LEQ, "\\(<="},
        {Constants::GEQ, "\\(>="},
        {Constants::SIM, "\\(ap"},
        {Constants::SIMEQ, "\\(|="},
        {Constants::NEQ, "\\(!="},
        {Constants::LEFTARROW, "\\(<-"},
        {Constants::RIGHTARROW, "\\(->"},
        {Constants::DIV, "\\(di"},
        {Constants::PM, "\\(+-"},
        {Constants::CUP, "\\(cu"},
        {Constants::CAP, "\\(ca"},
        {Constants::SUBSET, "\\(sb"},
        {Constants::SUPSET, "\\(sp"},
        {Constants::SUBSET, "\\(ib"},
        {Constants::SUPSETEQ, "\\(ip"},
        {Constants::INFTY, "\\(if"},
        {Constants::PARTIAL, "\\(pd"};
        {Constants::NABLA, "\\(gr"},
        {Constants::PROPTO, "\\(pt"},
        {Constants::VARNOTHING, "\\(es"},
        {Constants::EMPTY, "\\(es"},
        {Constants::IN, "\\(mo"}
        };

    std::unordered_map<std::string, std::string> translateFuncs = {
        {Constants::SQRT, "sqrt"},
        {Constants::LEFT, "left"},
        {Constants::RIGHT, "right"},
        {Constants::BIGL, "left"},
        {Constants::BIGR, "right"},
        {Constants::FRAC, "over"},
        {Constants::OVERLINE, "bar"},
        {Constants::BAR, "bar"},
        {Constants::UNDERLINEW, "under"},
        {Constants::HAT, "hat"},
        {Constants::TILDE, "tilde"},
        {Constants::ODOT, "dot"},
        {Constants::VEC, "vec"},
        {std::string(1, Constants::UNDERLINE), "sub"}};

    std::unordered_map<std::string, std::string> translateGenFracs = {
        {Constants::OVER, "over"},
        {Constants::ATOP, "above"},
        {Constants::CHOOSE, "above"}};

    std::unordered_map<std::string, std::string> translateStandardFunctions = {
        {"\\arccos", "arccos"},
        {"\\arcsin", "arcsin"},
        {"\\arctan", "arctan"},
        {"\\arg", "arg"},
        {"\\cos", "cos"},
        {"\\cosh", "cosh"},
        {"\\cot", "cot"},
        {"\\coth", "coth"},
        {"\\csc", "csc"},
        {"\\deg", "deg"},
        {"\\det", "det"},
        {"\\dim", "dim"},
        {"\\exp", "exp"},
        {"\\gcd", "gcd"},
        {"\\hom", "hom"},
        {"\\inf", "inf"},
        {"\\ker", "ker"},
        {"\\lg", "lg"},
        {"\\lim", "lim"},
        {"\\liminf", "liminf"},
        {"\\limsup", "limsup"},
        {"\\ln", "ln"},
        {"\\log", "log"},
        {"\\max", "max"},
        {"\\min", "min"},
        {"\\Pr", "Pr"},
        {"\\sec", "sec"},
        {"\\sin", "sin"},
        {"\\sinh", "sinh"},
        {"\\sup", "sup"},
        {"\\tan", "tan"},
        {"\\tanh", "tanh"}
        };

    void syntaxError(std::string error);

    std::ifstream latexf;
    std::ofstream output;

    bool error = false;
    bool outputMode = true;
    int scopeId = 1;

    std::vector<std::string> openClose;
    std::unordered_map<int, std::string> genFracs;

    void start();
    void body(std::string &token, std::vector<std::string> &itemsScope, int scope);
    void stmt(std::string &token, std::vector<std::string> &itemsScope, int scope);
    void expr(std::string &token, std::vector<std::string> &itemsScope, int scope);
    void expr1(std::string &token, std::vector<std::string> &itemsScope, int scope);
    void sqrtExpr(std::string &token, std::vector<std::string> &itemsScope, int scope);

    bool unexpectedTokens(std::string &token);

    bool rules(std::string &token);
    bool discretionary(std::string &token);
    bool penalty(std::string &token);
    bool whatsit(std::string &token);
    bool boundaryItem(std::string &token, std::vector<std::string> &itemsScope, int scope);

    bool glues(std::string &token);

    bool fourWayChoice(std::string &token);

    bool binAtom(std::string &token, std::vector<std::string> &itemsScope, int scope);
    bool relAtom(std::string &token);
    bool openAtom(std::string &token);
    bool closeAtom(std::string &token);
    bool punctAtom(std::string &token);
    bool innerAtom(std::string &token, std::vector<std::string> &itemsScope, int scope);
    bool vcentAtom(std::string &token);
    bool overAtom(std::string &token, std::vector<std::string> &itemsScope, int scope);
    bool underAtom(std::string &token, std::vector<std::string> &itemsScope, int scope);
    bool radAtom(std::string &token, std::vector<std::string> &itemsScope, int scope);
    bool accAtom(std::string &token, std::vector<std::string> &itemsScope, int scope);
    bool opAtom(std::string &token);
    bool ordAtom(std::string &token);
    bool generalizedFracs(std::string &token, std::vector<std::string> &itemsScope, int scope);
    void handleFractions(int scope);
    bool greekHebrowLetters(std::string token);

    bool isEndExpr(std::string &token);

    bool isWord(std::string letter);

    void printOut(std::string token);

public:
    TexParser(std::string input, std::string output);
    ~TexParser();

    void parse();
};
#include "TexParser.cpp"
#endif