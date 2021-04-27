#ifndef _TEXPARSER_H_
#define _TEXPARSER_H_
#include<fstream>
#include<string>
#include <sstream>
#include<vector>

class TexParser
{
    private:
        std::ifstream latexf;
        std::ofstream output;

        void start();
        void expr(std::string token);
        void expr1(std::string token);
        void expr2(std::string token);

        bool isNumber(std::string number);

    public:
        TexParser(std::string input, std::string output);
        ~TexParser();
        
        void parse();
};
#include "TexParser.cpp"
#endif