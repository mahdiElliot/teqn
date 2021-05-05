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
        {Constants::UXI, "XI"}};

    std::unordered_map<std::string, std::string> translateFuncs = {
        {Constants::SQRT, "sqrt"},
        {Constants::LEFT, "left"},
        {Constants::RIGHT, "right"},
        {Constants::OVER, "over"},
        {Constants::FRAC, "over"},
        {Constants::ATOP, "above"},
        {Constants::OVERLINE, "bar"},
        {Constants::UNDERLINEW, "under"},
    };

    std::string
    syntaxError(std::string error);

    std::ifstream latexf;
    std::ofstream output;

    std::vector<std::string> openClose;

    void start();
    void body(std::string &token, std::vector<std::string> itemsScope);
    void stmt(std::string &token, std::vector<std::string> itemsScope);
    void expr(std::string &token, std::vector<std::string> itemsScope);
    void expr1(std::string &token, std::vector<std::string> itemsScope);
    void sqrtExpr(std::string &token, std::vector<std::string> itemsScope);

    bool unexpectedTokens(std::string &token);

    bool rules(std::string &token);
    bool discretionary(std::string &token);
    bool penalty(std::string &token);
    bool whatsit(std::string &token);
    bool boundaryItem(std::string &token, std::vector<std::string> itemsScope);

    bool glues(std::string &token);

    bool fourWayChoice(std::string &token);

    bool binAtom(std::string &token, std::vector<std::string> scopeItems);
    bool relAtom(std::string &token);
    bool openAtom(std::string &token);
    bool closeAtom(std::string &token);
    bool punctAtom(std::string &token);
    bool innerAtom(std::string &token);
    bool vcentAtom(std::string &token);
    bool overAtom(std::string &token);
    bool underAtom(std::string &token);
    bool radAtom(std::string &token, std::vector<std::string> scopeItems);
    bool accAtom(std::string &token);
    bool opAtom(std::string &token);
    bool ordAtom(std::string &token);
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