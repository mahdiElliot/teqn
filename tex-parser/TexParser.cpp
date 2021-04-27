#include <iostream>
#include "../utils/Tokenizer.h"
#include "TexParser.h"

TexParser::TexParser(std::string input, std::string output)
{
    latexf.open(input, std::ios::in);
    this->output.open(output, std::ios::out);
}

TexParser::~TexParser()
{
    latexf.close();
    output.close();
}

void TexParser::parse()
{
    start();
    latexf.close();
    output.close();
}

void TexParser::start()
{
    std::string token = Tokenizer::nextToken(latexf);
    while (!latexf.eof() && token != ".BL")
        token = Tokenizer::nextToken(latexf);
    if (latexf.eof())
        return;
    token = Tokenizer::nextToken(latexf);
    expr(token);
    if (token == ".EL")
        start();
}

void TexParser::expr(std::string token)
{
    expr1(token);
    while (token[0] == Constants::OPENBRACE)
    {
        token = Tokenizer::nextToken(latexf);
        expr1(token);
        if (token[0] == Constants::CLOSEBRACE)
            token = Tokenizer::nextToken(latexf);
        else
            std::cout << Tokenizer::getLine() << ": syntax error in }\n";
    }
}

void TexParser::expr1(std::string token)
{
    if (token[0] == Constants::BACKSLASH)
    {
        if (token.substr(1) == "sqrt")
        {
            token = Tokenizer::nextToken(latexf);
            expr2(token);
        }
    } else if(isNumber(token))
    {

    }
}

void TexParser::expr2(std::string token)
{
    expr(token);
    while (token[0] == Constants::OPENBRACKET)
    {
        token = Tokenizer::nextToken(latexf);
        expr(token);
        if (token[0] == Constants::CLOSEBRACKET)
            token = Tokenizer::nextToken(latexf);
        else
            std::cout << Tokenizer::getLine() << ": syntax error in ]\n";
    }
}

bool TexParser::isNumber(std::string number)
{
    auto result = double();
    auto i = std::istringstream(number);

    i >> result;

    return !i.fail() && i.eof();
}