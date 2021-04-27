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

std::string TexParser::syntaxError(std::string error)
{
    return std::to_string(Tokenizer::getLine()) + ": syntax error in: " + error + '\n';
}

void TexParser::start()
{
    std::string token = Tokenizer::nextToken(latexf);
    while (!latexf.eof() && token != STARTEXP)
        token = Tokenizer::nextToken(latexf);
    if (latexf.eof())
        return;
    token = Tokenizer::nextToken(latexf);
    expr(token);
    if (token == ENDEXP)
        start();
}

void TexParser::expr(std::string &token)
{
    if (token[0] == Constants::HASH)
    {
        syntaxError("# not allowed");
        exit(0);
    }
    if (token[0] == Constants::BACKSLASH)
    {
        if (!isWord(token.substr(1)) && token[1] != Constants::SLASH && token[1] != Constants::BACKSLASH)
        {
            std::cout << syntaxError("symbol after backslash not allowed");
            exit(0);
        }
        else if (token == Constants::RULE)
        {
        }
        else if (token == Constants::DISCRETIONARY)
        {
        }
        else if (token == Constants::PENALTY)
        {
        }
        if (token == Constants::SQRT)
        {
            token = Tokenizer::nextToken(latexf);
            sqrtExpr(token);
            expr(token);
        }
    }
    else if (isNumber(token) || isWord(token))
    {
        token = Tokenizer::nextToken(latexf);
        expr(token);
    }
    else if (token[0] == Constants::OPENBRACE)
    {
    }
    else if (token[0] == Constants::CLOSEBRACE)
    {
        std::cout << syntaxError(std::string(1, Constants::CLOSEBRACE));
        exit(0);
    }
}

void TexParser::expr1(std::string &token)
{
    if (token[0] == Constants::OPENBRACE)
    {
        token = Tokenizer::nextToken(latexf);
        expr2(token);
        if (token[0] == Constants::CLOSEBRACE)
            token = Tokenizer::nextToken(latexf);
        else
        {
            std::cout << syntaxError(std::string(1, Constants::CLOSEBRACE));
            exit(0);
        }
    }
}

void TexParser::expr2(std::string &token)
{
    bool e = false;
    if (token[0] == Constants::HASH)
    {
        syntaxError("# not allowed");
        exit(0);
    }
    if (token[0] == Constants::BACKSLASH)
    {
        e = true;
        if (!isWord(token.substr(1)) && token[1] != Constants::SLASH && token[1] != Constants::BACKSLASH)
        {
            std::cout << syntaxError("symbol after backslash not allowed");
            exit(0);
        }
        else if (token == Constants::RULE)
        {
        }
        else if (token == Constants::DISCRETIONARY)
        {
        }
        else if (token == Constants::PENALTY)
        {
        }
        if (token == Constants::SQRT)
        {
            token = Tokenizer::nextToken(latexf);
            sqrtExpr(token);
            expr(token);
        }
    }
    else if (isNumber(token) || isWord(token))
    {
        token = Tokenizer::nextToken(latexf);
        expr2(token);
    }
    else if (token[0] == Constants::OPENBRACE)
    {
    }
}

void TexParser::sqrtExpr(std::string &token)
{
    if (token[0] == Constants::OPENBRACKET)
    {
        token = Tokenizer::nextToken(latexf);
        expr(token);
        if (token[0] == Constants::CLOSEBRACKET)
        {
            token = Tokenizer::nextToken(latexf);
            if (token[0] == Constants::OPENBRACE)
                expr1(token);
            else
                expr(token);
        }
        else
        {
            std::cout << syntaxError(std::string(1, Constants::CLOSEBRACKET));
            exit(0);
        }
    }
}

bool TexParser::isNumber(std::string number)
{
    auto result = double();
    auto i = std::istringstream(number);

    i >> result;

    return !i.fail() && i.eof();
}

bool TexParser::isWord(std::string letter)
{
    bool valid = false;
    for (int i = 0; i < letter.size(); i++)
        if (Tokenizer::isDigit(letter[i]) || (int(letter[i]) >= 65 && int(letter[i]) <= 90) ||
            (int(letter[i]) >= 97 && int(letter[i] <= 122)))
            valid = true;
        else
            return false;

    return valid;
}