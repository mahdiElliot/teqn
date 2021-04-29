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
    body(token);
    if (token == ENDEXP)
        start();
    else if (latexf.eof())
        std::cout << "error, end of expression not specified\n";
}

void TexParser::body(std::string &token)
{
    if (token == ENDEXP)
        return;
    bool e = (token[0] == Constants::CLOSEBRACE && openClose.back() == std::string(1, Constants::OPENBRACE)) ||
             (token[0] == Constants::CLOSEBRACKET && openClose.back() == std::string(1, Constants::OPENBRACKET));

    if (e)
        return;
        
    stmt(token);
    if (!latexf.eof() && token != ENDEXP)
    {
        body(token);
    }
}

bool TexParser::unexpectedTokens(std::string &token)
{
    if (token[0] == Constants::CLOSEBRACE)
    {
        std::cout << "} without {\n";
    }
    else
        return false;

    return true;
}

void TexParser::stmt(std::string &token)
{
    if (token[0] == Constants::OPENBRACE)
    {
        openClose.push_back(std::string(1, token[0]));
        token = Tokenizer::nextToken(latexf);
        body(token);
        if (token[0] != Constants::CLOSEBRACE)
        {
            std::cout << syntaxError(std::string(1, Constants::CLOSEBRACE));
        }
        else
            openClose.pop_back();
        token = Tokenizer::nextToken(latexf);
    }
    else if (token == Constants::LEFT)
    {
        token = Tokenizer::nextToken(latexf);
        if (token[0] == Constants::OPENBRACKET)
        {
        }
        else if (token[0] == Constants::OPENPARENTHESIS)
        {
        }
    }
    else if (token == Constants::RIGHT)
    {
        token = Tokenizer::nextToken(latexf);
        if (token[0] == Constants::CLOSEBRACKET)
        {
        }
        else if (token[0] == Constants::CLOSEPARENTHESIS)
        {
        }
    }
    else if (unexpectedTokens(token))
    {
        token = Tokenizer::nextToken(latexf);
    }
    else
    {
        expr(token);
    }
}

void TexParser::expr(std::string &token)
{
    if (token[0] == Constants::HASH)
    {
        syntaxError("# not allowed");
    }

    if (token[0] == Constants::BACKSLASH)
    {
        if (!isWord(token.substr(1)) && token[1] != Constants::SLASH && token[1] != Constants::BACKSLASH)
        {
            std::cout << syntaxError("symbol after backslash not allowed");
            token = Tokenizer::nextToken(latexf);
        }
        else if (rules(token))
        {
        }
        else if (discretionary(token))
        {
        }
        else if (penalty(token))
        {
        }
        else if (whatsit(token))
        {
        }
        else if (boundaryItem(token))
        {
        }
        else if (glues(token))
        {
        }
        if (token == Constants::SQRT)
        {
            token = Tokenizer::nextToken(latexf);
            sqrtExpr(token);
        }
    }
    else if (binAtom(token))
    {
    }
    else if (isNumber(token) || isWord(token))
    {
        token = Tokenizer::nextToken(latexf);
    }
    else if (token[0] == Constants::CLOSEBRACKET)
    {
        token = Tokenizer::nextToken(latexf);
    }
    else if (token[0] == Constants::CLOSEPARENTHESIS)
    {
        token = Tokenizer::nextToken(latexf);
    }
}

void TexParser::expr1(std::string &token)
{
    if (token[0] == Constants::OPENBRACE)
    {
        token = Tokenizer::nextToken(latexf);
        expr(token);
        if (token[0] == Constants::CLOSEBRACE)
            token = Tokenizer::nextToken(latexf);
        else
        {
            std::cout << syntaxError(std::string(1, Constants::CLOSEBRACE));
            token = Tokenizer::nextToken(latexf);
        }
    }
}

void TexParser::expr2(std::string &token)
{
}

void TexParser::sqrtExpr(std::string &token)
{
    if (token[0] == Constants::OPENBRACKET)
    {
        openClose.push_back(std::string(1, token[0]));
        token = Tokenizer::nextToken(latexf);
        body(token);
        if (token[0] == Constants::CLOSEBRACKET)
        {
            openClose.pop_back();
            token = Tokenizer::nextToken(latexf);
            if (token[0] == Constants::OPENBRACE)
            {
                openClose.push_back(std::string(1, token[0]));
                token = Tokenizer::nextToken(latexf);
                body(token);
                if (token[0] != Constants::CLOSEBRACE)
                {
                    std::cout << syntaxError(std::string(1, Constants::CLOSEBRACE));
                }
                else
                    openClose.pop_back();

                token = Tokenizer::nextToken(latexf);
            }
            else
            {
                if (unexpectedTokens(token))
                {
                }
                token = Tokenizer::nextToken(latexf);
            }
        }
        else
        {
            std::cout << syntaxError(std::string(1, Constants::CLOSEBRACKET));
            token = Tokenizer::nextToken(latexf);
        }
    }
    else if (token[0] == Constants::OPENBRACE)
    {
        openClose.push_back(std::string(1, token[0]));
        token = Tokenizer::nextToken(latexf);
        body(token);
        if (token[0] != Constants::CLOSEBRACE)
        {
            std::cout << syntaxError(std::string(1, Constants::CLOSEBRACE));
        }
        else
            openClose.pop_back();

        token = Tokenizer::nextToken(latexf);
    }
    else
    {
        if (unexpectedTokens(token))
        {
        }
        token = Tokenizer::nextToken(latexf);
    }
}

bool TexParser::rules(std::string &token)
{
    bool e = false;
    if (token == Constants::RULE)
    {
        e = true;
    }
    return e;
}

bool TexParser::discretionary(std::string &token)
{
    bool e = false;
    if (token == Constants::DISCRETIONARY)
    {
        e = true;
    }
    return e;
}

bool TexParser::penalty(std::string &token)
{
    bool e = true;
    ;
    if (token == Constants::PENALTY)
    {
    }
    else if (token == Constants::RELPENALTY)
    {
    }
    else
        e = false;
    return e;
}

bool TexParser::whatsit(std::string &token)
{
    bool e = true;
    if (token == Constants::OPENOUT)
    {
    }
    else if (token == Constants::CLOSEOUT)
    {
    }
    else if (token == Constants::WRITE)
    {
    }
    else if (token == Constants::IMMEDIATE)
    {
    }
    else if (token == Constants::SHIPOUT)
    {
    }
    else if (token == Constants::SPECIAL)
    {
    }
    else
        e = false;

    return e;
}

bool TexParser::boundaryItem(std::string &token)
{
    bool e = true;
    if (token == Constants::LEFT)
    {
    }
    else if (token == Constants::RIGHT)
    {
    }
    else
        e = false;

    return e;
}

bool TexParser::glues(std::string &token)
{
    bool e = true;
    if (token == Constants::SMALLSKIP)
    {
    }
    else if (token == Constants::MEDSKIP)
    {
    }
    else if (token == Constants::VSKIP)
    {
    }
    else if (token == Constants::HSKIP)
    {
    }
    else if (token == Constants::LDOTS)
    {
    }
    else if (token == Constants::FRENCHSPACING)
    {
    }
    else if (token == Constants::NONFRENCHSPACING)
    {
    }
    else if (token == Constants::TENRM)
    {
    }
    else if (token == Constants::KERN)
    {
    }
    else if (token == Constants::KERN)
    {
    }
    else
        e = false;

    return e;
}

bool TexParser::fourWayChoice(std::string &token)
{
    bool e = false;
    if (token == Constants::MATHCHOICE)
    {
        e = true;
    }

    return e;
}

bool TexParser::binAtom(std::string &token)
{
    bool e = false;
    if (token[0] == Constants::ADD || token[0] == Constants::SUB || token[0] == Constants::MULT || token[0] == Constants::SLASH || token[0] == Constants::POWER)
    {
        e = true;
    }
    return e;
}

bool TexParser::relAtom(std::string &token)
{
    bool e = false;
    if (token[0] == Constants::EQUAL || token[0] == Constants::GREATERTHAN || token[0] == Constants::LESSTHAN)
    {
        e = true;
    }
    return e;
}

bool TexParser::openAtom(std::string &token)
{
    bool e = false;
    if (token[0] == Constants::OPENBRACKET || token[0] == Constants::OPENPARENTHESIS)
    {
        e = true;
    }
    return e;
}

bool TexParser::closeAtom(std::string &token)
{
    bool e = false;
    if (token[0] == Constants::CLOSEBRACKET || token[0] == Constants::CLOSEPARENTHESIS)
    {
        e = true;
    }
    return e;
}

bool TexParser::punctAtom(std::string &token)
{
    bool e = false;
    if (token[0] == Constants::COMMA)
    {
        e = true;
    }
    return e;
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