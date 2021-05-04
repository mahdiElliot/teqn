#ifndef _TEXPARSER_H_
#define _TEXPARSER_H_
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

class TexParser
{
private:
    const std::string STARTEXP = ".BL";
    const std::string ENDEXP = ".EL";

    const std::unordered_map<std::string, std::string> translate = {
        {Constants::SINGLEVERT, "|"},
        {Constants::DOUBLEVERT, "||"},
        {Constants::SQRT, "sqrt"},
        {Constants::LEFT, "left"},
        {Constants::RIGHT, "right"},
        {Constants::OVER, "over"},
        {Constants::LANGLE, "left <"},
        {Constants::RANGLE, "right >"},
        {Constants::LFLOOR, "\\(lf"},
        {Constants::RFLOOR, "\\(rf"},
        {"\\|", "||"},
        {"\\{", "{"},
        {"\\}", "}"},
        {std::string(1, Constants::POWER), "sup"}
        };

    std::string syntaxError(std::string error);

    std::ifstream latexf;
    std::ofstream output;

    std::vector<std::string> openClose;

    void start();
    void body(std::string &token, std::vector<std::string> itemsScope);
    void stmt(std::string &token, std::vector<std::string> itemsScope);
    void expr(std::string &token, std::vector<std::string> itemsScope);
    void expr1(std::string &token, std::vector<std::string> itemsScope);
    void expr2(std::string &token, std::vector<std::string> itemsScope);
    void sqrtExpr(std::string &token, std::vector<std::string> itemsScope);

    bool unexpectedTokens(std::string &token);
    bool rules(std::string &token);
    bool discretionary(std::string &token);
    bool penalty(std::string &token);
    bool whatsit(std::string &token);
    bool boundaryItem(std::string &token, std::vector<std::string> itemsScope);
    void rightBoundaryAtom(std::string &token, std::string rightItem);

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
    bool underLineAtom(std::string &token);
    bool radAtom(std::string &token, std::vector<std::string> scopeItems);
    bool accAtom(std::string &token);
    bool opAtom(std::string &token);
    bool ordAtom(std::string &token);

    bool isEndExpr(std::string &token);

    bool isWord(std::string letter);

public:
    TexParser(std::string input, std::string output);
    ~TexParser();

    void parse();
};
#include "TexParser.cpp"
#endif