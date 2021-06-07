#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_
#include <string>
#include <fstream>

class Tokenizer
{
private:
    static int line;

public:
    static std::string nextToken(std::ifstream &myfile);

    static std::string nextToken2(std::ifstream &myfile);

    static bool isDigit(char c);

    static int getLine();
    static void setLine(int line);
};
#include "Tokenizer.cpp"
#endif