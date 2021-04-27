#ifndef _TEXPARSER_H_
#define _TEXPARSER_H_
#include<fstream>
#include<string>
#include <sstream>
#include<vector>

class TexParser
{
    private:
        const std::string STARTEXP = ".BL";
        const std::string ENDEXP = ".EL";

        std::string syntaxError(std::string error);
        
        std::ifstream latexf;
        std::ofstream output;

        void start();
        void expr(std::string &token);
        void expr1(std::string &token);
        void expr2(std::string &token);
        void sqrtExpr(std::string &token);

        bool isNumber(std::string number);
        bool isWord(std::string letter);

    public:
        TexParser(std::string input, std::string output);
        ~TexParser();
        
        void parse();
};
#include "TexParser.cpp"
#endif