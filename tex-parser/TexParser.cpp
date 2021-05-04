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

bool TexParser::isEndExpr(std::string &token)
{
    if (token[0] == Constants::DOT)
    {
        std::string t = ".";
        char c;
        latexf.get(c);
        if (c == 'E')
        {
            t += c;
            latexf.get(c);
            if (c == 'L')
            {
                t += c;
                token = t;
                return true;
            }
            latexf.unget();
        }
        else
            latexf.unget();
    }
    return false;
}

void TexParser::start()
{
    std::string token = Tokenizer::nextToken2(latexf);
    while (!latexf.eof() && token != STARTEXP && token != ENDEXP)
    {
        output << token << " ";
        token = Tokenizer::nextToken2(latexf);
    }
    if (latexf.eof())
        return;
    else if (token == ENDEXP)
    {
        std::cout << "first of expression not specified\n";
        return;
    }
    openClose.push_back(".BL");
    output << ".EQ ";
    token = Tokenizer::nextToken(latexf);
    std::vector<std::string> mainScope;
    body(token, mainScope);
    if (token == ENDEXP)
    {
        output << ".EN ";
        start();
    }
    else if (latexf.eof())
        std::cout << "error, end of expression not specified\n";
}

void TexParser::body(std::string &token, std::vector<std::string> itemsScope)
{
    if (isEndExpr(token))
        return;

    if ((token[0] == Constants::CLOSEBRACE && openClose.back() == std::string(1, Constants::OPENBRACE)) ||
        (token[0] == Constants::CLOSEBRACKET && openClose.back() == std::string(1, Constants::OPENBRACKET)) ||
        (token == Constants::RIGHT && openClose.back() == Constants::LEFT))
        return;

    stmt(token, itemsScope);
    if (!latexf.eof() && token != ENDEXP)
    {
        body(token, itemsScope);
    }
}

bool TexParser::unexpectedTokens(std::string &token)
{
    if (token[0] == Constants::CLOSEBRACE)
    {
        std::cout << "} without {\n";
    }
    else if (token == Constants::RIGHT)
    {
        std::cout << syntaxError("right without left");
    }
    else
        return false;

    return true;
}

void TexParser::stmt(std::string &token, std::vector<std::string> itemsScope)
{
    if (token[0] == '\r' || token[0] == '\n')
    {
        output << token;
        token = Tokenizer::nextToken(latexf);
    }
    if (token[0] == Constants::COMMENT)
    {
        //TODO
        token = Tokenizer::nextToken(latexf);
    }
    else if (token[0] == Constants::OPENBRACE)
    {
        output << token << " ";
        openClose.push_back(token);
        token = Tokenizer::nextToken(latexf);
        std::vector<std::string> localScope;
        body(token, localScope);
        if (token[0] == Constants::CLOSEBRACE)
        {
            output << token << " ";
            openClose.pop_back();
        }
        else
            std::cout << syntaxError(std::string(1, Constants::CLOSEBRACE));

        token = Tokenizer::nextToken(latexf);
    }
    else if (boundaryItem(token, itemsScope))
    {
        token = Tokenizer::nextToken(latexf);
    }
    else if (unexpectedTokens(token))
    {
        token = Tokenizer::nextToken(latexf);
    }
    else
    {
        expr(token, itemsScope);
    }
}

void TexParser::expr(std::string &token, std::vector<std::string> itemsScope)
{
    if (token[0] == Constants::HASH)
    {
        syntaxError("# not allowed");
        token = Tokenizer::nextToken(latexf);
    }
    else if (token[0] == Constants::BACKS)
    {
        if (token[1] == Constants::COMMENT)
        {
            output << token[1] << " ";
            token = Tokenizer::nextToken(latexf);
        }
        else if (token[1] == Constants::HASH)
        {
            output << token[1] << " ";
            token = Tokenizer::nextToken(latexf);
        }
        else if (token[1] == Constants::COMMA)
        {
            //TODO
            token = Tokenizer::nextToken(latexf);
        }
        else if (token[1] == Constants::SLASH)
        {
            output << token << " ";
            token = Tokenizer::nextToken(latexf);
        }
        else if (token[1] == Constants::BACKS)
        {
            output << token << " ";
            token = Tokenizer::nextToken(latexf);
        }
        else if (token[1] == Constants::SUB)
        {
            output << token << " ";
            token = Tokenizer::nextToken(latexf);
        }
        else if (token[1] == Constants::UNDERLINE)
        {
            output << token[1] << " ";
            token = Tokenizer::nextToken(latexf);
        }
        else if (rules(token))
        {
            //TODO
            token = Tokenizer::nextToken(latexf);
        }
        else if (discretionary(token))
        {
            //TODO
            token = Tokenizer::nextToken(latexf);
        }
        else if (penalty(token))
        {
            //TODO
            token = Tokenizer::nextToken(latexf);
        }
        else if (whatsit(token))
        {
            //TODO
            token = Tokenizer::nextToken(latexf);
        }
        else if (glues(token))
        {
            //TODO
            token = Tokenizer::nextToken(latexf);
        }
        else if (radAtom(token, itemsScope))
        {
            token = Tokenizer::nextToken(latexf);
        }
    }
    else if (binAtom(token, itemsScope))
    {
    }
    else if (Tokenizer::isDigit(token[0]) || isWord(token))
    {
        output << token << " ";
        token = Tokenizer::nextToken(latexf);
    }
    else if (closeAtom(token) || openAtom(token))
    {
        output << token << " ";
        token = Tokenizer::nextToken(latexf);
    }
}

void TexParser::expr2(std::string &token, std::vector<std::string> itemsScope)
{
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

bool TexParser::boundaryItem(std::string &token, std::vector<std::string> itemsScope)
{
    bool e = true;
    if (token == Constants::LEFT)
    {
        output << token.substr(1) << " ";
        openClose.push_back(token);
        token = Tokenizer::nextToken(latexf);
        if (token[0] == Constants::OPENBRACKET)
        {
            output << token << " ";
            token = Tokenizer::nextToken(latexf);
            std::vector<std::string> localScope;
            body(token, localScope);
            rightBoundaryAtom(token, std::string(1, Constants::CLOSEBRACKET));
        }
        else if (token[0] == Constants::OPENPARENTHESIS)
        {
            output << token << " ";
            token = Tokenizer::nextToken(latexf);
            std::vector<std::string> localScope;
            body(token, localScope);
            rightBoundaryAtom(token, std::string(1, Constants::CLOSEPARENTHESIS));
        }
        else if (token[0] == Constants::OR)
        {
            output << token << " ";
            token = Tokenizer::nextToken(latexf);
            std::vector<std::string> localScope;
            body(token, localScope);
            rightBoundaryAtom(token, std::string(1, Constants::OR));
        }
        else if (token[0] == Constants::DOT)
        {
            output << token << " ";
            token = Tokenizer::nextToken(latexf);
            std::vector<std::string> localScope;
            body(token, localScope);
            if (token == Constants::RIGHT)
            {
                output << token.substr(1) << " ";
                openClose.pop_back();
                token = Tokenizer::nextToken(latexf);
                if (token[0] == Constants::DOT || openAtom(token) || closeAtom(token))
                {
                    output << token << " ";
                }
                else
                    std::cout << syntaxError(std::string(1, Constants::OR));
            }
            else
                std::cout << syntaxError("right");
        }
        else if (token == "\\{")
        {
            output << token.substr(1) << " ";
            token = Tokenizer::nextToken(latexf);
            std::vector<std::string> localScope;
            body(token, localScope);
            rightBoundaryAtom(token, "\\}");
        }
    }
    else
        e = false;

    return e;
}

void TexParser::rightBoundaryAtom(std::string &token, std::string rightItem)
{
    if (token == Constants::RIGHT)
    {
        if (rightItem == "")
        {
            std::cout << syntaxError("right without left");
            return;
        }
        output << token.substr(1) << " ";
        openClose.pop_back();
        token = Tokenizer::nextToken(latexf);
        if (token == rightItem)
        {
            output << token << " ";
        }
        else if (token[0] == Constants::DOT)
        {
        }
        else
            std::cout << syntaxError(rightItem);
    }
    else
        std::cout << syntaxError("right");
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

bool TexParser::binAtom(std::string &token, std::vector<std::string> scopeItems)
{
    bool e = true;
    if (token[0] == Constants::ADD || token[0] == Constants::SUB || token[0] == Constants::MULT || token[0] == Constants::SLASH)
    {
        output << token << " ";
        token = Tokenizer::nextToken(latexf);
    }
    else if (token[0] == Constants::POWER)
    {
        output << "sup ";
        token = Tokenizer::nextToken(latexf);
        if (token[0] == Constants::POWER)
        {
            std::cout << syntaxError(std::string(1, Constants::POWER));
            token = Tokenizer::nextToken(latexf);
        }
        else if (token[0] == Constants::OPENBRACE)
        {
            output << token << " ";
            openClose.push_back(token);
            token = Tokenizer::nextToken(latexf);
            std::vector<std::string> localScope;
            body(token, localScope);
            if (token[0] == Constants::CLOSEBRACE)
            {
                output << token << " ";
                openClose.pop_back();
            }
            else
                std::cout << syntaxError(std::string(1, Constants::CLOSEBRACE));

            token = Tokenizer::nextToken(latexf);
        }
        else if ((isWord(token) || Tokenizer::isDigit(token[0])))
        {
            output << token << " ";
            token = Tokenizer::nextToken(latexf);
            if (token[0] == Constants::POWER)
            {
                std::cout << syntaxError("double super script");
                token = Tokenizer::nextToken(latexf);
            }
        }
        else if (boundaryItem(token, scopeItems))
            token = Tokenizer::nextToken(latexf);
    }
    else
        e = false;
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
    if (token[0] == Constants::OPENBRACKET || token[0] == Constants::OPENPARENTHESIS ||
        token[0] == Constants::OR || token == "\\{")
    {
        e = true;
    }
    return e;
}

bool TexParser::closeAtom(std::string &token)
{
    bool e = false;
    if (token[0] == Constants::CLOSEBRACKET || token[0] == Constants::CLOSEPARENTHESIS || token == "\\}")
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

bool TexParser::radAtom(std::string &token, std::vector<std::string> itemsScope)
{
    bool e = true;
    if (token == Constants::SQRT)
    {
        output << token.substr(1) << " ";
        token = Tokenizer::nextToken(latexf);
        sqrtExpr(token, itemsScope);
    }
    else if (token == Constants::RADICAL)
    {
        //TODO
    }
    else
        e = false;
    return e;
}

void TexParser::sqrtExpr(std::string &token, std::vector<std::string> itemsScope)
{
    if (token[0] == Constants::OPENBRACKET)
    {
        openClose.push_back(token);
        token = Tokenizer::nextToken(latexf);
        std::vector<std::string> localScope;
        body(token, localScope);
        if (token[0] == Constants::CLOSEBRACKET)
        {
            openClose.pop_back();
            token = Tokenizer::nextToken(latexf);
            expr1(token, itemsScope);
        }
        else
            std::cout << syntaxError(std::string(1, Constants::CLOSEBRACKET));
    }
    else if (token[0] == Constants::BACKS)
        std::cout << syntaxError("backslash not allowed");
    else if (isEndExpr(token))
        std::cout << syntaxError("sqrt needs argument");
    else
        expr1(token, itemsScope);
}

void TexParser::expr1(std::string &token, std::vector<std::string> itemsScope)
{
    if (token[0] == Constants::OPENBRACE)
    {
        output << token << " ";
        openClose.push_back(token);
        token = Tokenizer::nextToken(latexf);
        std::vector<std::string> localScope;
        body(token, localScope);
        if (token[0] == Constants::CLOSEBRACE)
        {
            output << token << " ";
            openClose.pop_back();
        }
        else
            std::cout << syntaxError(std::string(1, Constants::CLOSEBRACE));
    }
    else
    {
        if (!unexpectedTokens(token))
            output << token << " ";
    }
}

// bool TexParser::isNumber(std::string number)
// {
//     auto result = double();
//     auto i = std::istringstream(number);

//     i >> result;

//     return !i.fail() && i.eof();
// }

bool TexParser::isWord(std::string letter)
{
    bool valid = false;
    for (int i = 0; i < letter.size(); i++)
        if ((int(letter[i]) >= 65 && int(letter[i]) <= 90) ||
            (int(letter[i]) >= 97 && int(letter[i] <= 122)))
            valid = true;
        else
            return false;

    return valid;
}