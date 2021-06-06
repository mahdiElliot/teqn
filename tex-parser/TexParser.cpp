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

void TexParser::syntaxError(std::string e)
{
    error = true;
    std::cout << std::to_string(Tokenizer::getLine()) << ": syntax error in: " << e << '\n';
}

void TexParser::skipLines(std::string &token)
{
    while (token == "\n" || token == "\r")
    {
        if (outputMode)
            output << token;
        token = Tokenizer::nextToken(latexf);
    }
}

void TexParser::printOut(std::string token)
{
    if (!outputMode)
        return;
    std::string t = translate[token];
    t = t == "" ? translateLetters[token] : t;
    t = t == "" ? translateGenFracs[token] : t;
    t = t == "" ? translateFuncs[token] : t;
    t = t == "" ? translateStandardFunctions[token] : t;
    output << (t == "" ? token : t) << " ";
}

bool TexParser::isEndExpr(std::string &token)
{
    if (delimEnd != ' ' && openClose[0][0] == delimStart && token[0] == delimEnd)
        return true;
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
    justDelim = true;
    inLineEq = false;
    openClose.clear();
    std::string token = Tokenizer::nextToken2(latexf);
    while (token != "" && token != STARTEXP && token != ENDEXP && (delimStart == ' ' || token[0] != delimStart))
    {
        output << token;
        if (token == ".EQ")
        {
            token = Tokenizer::nextToken2(latexf);
            while (token != "delim" && token != ".EN")
            {
                output << token;
                token = Tokenizer::nextToken2(latexf);
            }
            output << token;
            if (token == "delim")
            {
                token = Tokenizer::nextToken2(latexf);
                while (token == " " || token == "\n" || token == "\r")
                {
                    output << token;
                    token = Tokenizer::nextToken2(latexf);
                }
                output << token;
                delimStart2 = token[0];
                token = Tokenizer::nextToken2(latexf);
                while (token == " ")
                {
                    output << token;
                    token = Tokenizer::nextToken2(latexf);
                }
                output << token;
                delimEnd2 = token[0];
                if (delimStart == delimStart2 && delimEnd == delimEnd2)
                {
                    delimStart = ' ';
                    delimEnd = ' ';
                }
            }
        }
        token = Tokenizer::nextToken2(latexf);
    }
    if (latexf.eof())
        return;
    else if (token == ENDEXP)
    {
        syntaxError("first of expression not specified");
        return;
    }

    inLineEq = delimStart != ' ' && token[0] == delimStart;

    std::string startToken = token;
    openClose.push_back(token);
    outputMode = false;
    scopeId = 1;
    genFracs.clear();

    std::istream::streampos p = latexf.tellg();
    token = Tokenizer::nextToken(latexf);

    std::vector<std::string> mainScope;
    body(token, mainScope, scopeId);
    if (inLineEq)
        output << delimStart2 << " ";
    else if (delimStart2 != ' ' && justDelim)
    {
        start();
        return;
    }
    else
        output << translate[startToken] << " ";

    if (((openClose[0] == STARTEXP && token == ENDEXP) ||
         (delimStart != ' ' && openClose[0][0] == delimStart && token[0] == delimEnd)) &&
        !error)
    {
        latexf.seekg(0, std::ios::beg);
        latexf.clear();
        latexf.seekg(p);
        outputMode = true;
        scopeId = 1;

        token = Tokenizer::nextToken(latexf);
        std::vector<std::string> mainScope;
        body(token, mainScope, scopeId);
        if (inLineEq)
            output << delimEnd2 << " ";
        else
            printOut(token);
        start();
    }
    else if (latexf.eof())
        syntaxError("error, end of expression not specified");
}

void TexParser::handleFractions(int scope)
{
    if (outputMode)
    {
        std::string x = genFracs[scope];
        if (x == "")
            return;
        if (x == Constants::OVER)
        {
            output << "{ ";
        }
        else if (x == Constants::ATOP)
        {
            output << "pile { {";
        }
        else if (x == Constants::CHOOSE)
        {
            output << "left ( pile { {";
        }
        genFracs.erase(scope);
    }
}

void TexParser::body(std::string &token, std::vector<std::string> &itemsScope, int scope)
{
    if (isEndExpr(token) || error)
        return;

    if ((token[0] == Constants::CLOSEBRACE && openClose.back() == std::string(1, Constants::OPENBRACE)) ||
        (token[0] == Constants::CLOSEBRACKET && openClose.back() == std::string(1, Constants::OPENBRACKET)) ||
        (token == Constants::RIGHT && openClose.back() == Constants::LEFT) ||
        (token == Constants::BIGR && openClose.back() == Constants::BIGL))
        return;

    stmt(token, itemsScope, scope);
    if (!latexf.eof() && token != ENDEXP)
    {
        body(token, itemsScope, scope);
    }
}

bool TexParser::unexpectedTokens(std::string &token)
{
    bool e = true;
    if (inLineEq && token[0] == delimEnd)
        return e;

    if (token[0] == Constants::BACKS)
    {
        e = false;
        if (token[1] == Constants::SINGLEQUOTE || token[1] == Constants::DOUBLEQUOTE ||
            token[1] == Constants::AT || token[1] == Constants::POWER || token[1] == Constants::EQUAL ||
            token[1] == Constants::ADD)
        {
            e = true;
            syntaxError(token);
            token = Tokenizer::nextToken(latexf);
        }
    }
    else if (token[0] == Constants::HASH)
    {
        syntaxError("# not allowed");
        token = Tokenizer::nextToken(latexf);
    }
    else if (token[0] == Constants::CLOSEBRACE)
    {
        syntaxError("} without {");
    }
    else if (token == Constants::RIGHT)
    {
        syntaxError("right without left");
    }
    else if (token == Constants::BIGR)
    {
        syntaxError("big right without left");
    }
    else
        e = false;

    return e;
}

void TexParser::stmt(std::string &token, std::vector<std::string> &itemsScope, int scope)
{
    handleFractions(scope);

    if (token[0] == '\r' || token[0] == '\n')
    {
        skipLines(token);
        return;
    }

    if (token != Constants::DELIM)
        justDelim = false;

    if (token[0] == Constants::COMMENT)
    {
        printOut(token);
        token = Tokenizer::nextToken(latexf);
    }
    else if (token[0] == Constants::SPACEHAT)
    {
        printOut(token);
        token = Tokenizer::nextToken(latexf);
    }
    else if (token[0] == Constants::OPENBRACE)
    {
        printOut(token);
        openClose.push_back(token);
        token = Tokenizer::nextToken(latexf);
        std::vector<std::string> localScope;
        scopeId++;
        body(token, localScope, scopeId);
        if (token[0] == Constants::CLOSEBRACE)
        {
            printOut(token);
            openClose.pop_back();
        }
        else
            syntaxError(std::string(1, Constants::CLOSEBRACE));

        token = Tokenizer::nextToken(latexf);
    }
    else if (boundaryItem(token, itemsScope, scope))
    {
        token = Tokenizer::nextToken(latexf);
    }
    else if (unexpectedTokens(token))
    {
        token = Tokenizer::nextToken(latexf);
    }
    else
    {
        expr(token, itemsScope, scope);
    }
}

void TexParser::expr(std::string &token, std::vector<std::string> &itemsScope, int scope)
{
    if (binAtom(token, itemsScope, scope))
        return;

    if (token[0] == Constants::BACKS)
    {
        if (token[1] == Constants::COMMENT)
        {
            printOut(std::string(1, token[1]));
            token = Tokenizer::nextToken(latexf);
        }
        else if (token[1] == Constants::HASH)
        {
            printOut(std::string(1, token[1]));
            token = Tokenizer::nextToken(latexf);
        }
        else if (token[1] == Constants::COMMA)
        {
            //TODO
            token = Tokenizer::nextToken(latexf);
        }
        else if (token[1] == Constants::SLASH)
        {
            printOut(token);
            token = Tokenizer::nextToken(latexf);
        }
        else if (token[1] == Constants::BACKS)
        {
            printOut(token);
            token = Tokenizer::nextToken(latexf);
        }
        else if (token[1] == Constants::SUB)
        {
            printOut(token);
            token = Tokenizer::nextToken(latexf);
        }
        else if (token[1] == Constants::UNDERLINE)
        {
            printOut(token);
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
        else if (innerAtom(token, itemsScope, scope))
        {
            token = Tokenizer::nextToken(latexf);
        }
        else if (generalizedFracs(token, itemsScope, scope))
        {
        }
        else if (radAtom(token, itemsScope, scope))
        {
            token = Tokenizer::nextToken(latexf);
        }
        else if (overAtom(token, itemsScope, scope))
        {
            token = Tokenizer::nextToken(latexf);
        }
        else if (underAtom(token, itemsScope, scope))
        {
            token = Tokenizer::nextToken(latexf);
        }
        else if (accAtom(token, itemsScope, scope))
        {
            token = Tokenizer::nextToken(latexf);
        }
        else if (token == Constants::TIMES)
        {
            printOut(token);
            token = Tokenizer::nextToken(latexf);
        }
        else if (opAtom(token, itemsScope, scope))
        {
        }
        else if (inLineEq && token == Constants::DELIM)
        {
            syntaxError("delim not allowed inside another delim!");
        }
        else if (delimCheck(token))
        {
            token = Tokenizer::nextToken(latexf);
        }
        else if (translateLetters[token] != "" || translateStandardFunctions[token] != "")
        {
            printOut(token);
            token = Tokenizer::nextToken(latexf);
        }
        else
        {
            syntaxError(token + " undefined");
            token = Tokenizer::nextToken(latexf);
        }
    }
    else if (relAtom(token))
    {
        token = Tokenizer::nextToken(latexf);
    }
    else if (token[0] == Constants::UNDERLINE)
    {
        printOut(token);
        token = Tokenizer::nextToken(latexf);
        skipLines(token);
        if (isEndExpr(token))
            syntaxError("sub needs argument");
        else
            expr1(token, itemsScope, scope);
        token = Tokenizer::nextToken(latexf);
    }
    else if (punctAtom(token))
    {
        token = Tokenizer::nextToken(latexf);
    }
    else if (Tokenizer::isDigit(token[0]) || isWord(token))
    {
        printOut(token);
        token = Tokenizer::nextToken(latexf);
    }
    else if (closeAtom(token) || openAtom(token) || token[0] == Constants::DOT)
    {
        printOut(token);
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

bool TexParser::boundaryItem(std::string &token, std::vector<std::string> &itemsScope, int scope)
{
    bool e = true;
    if (token == Constants::LEFT)
    {
        printOut(token);
        openClose.push_back(token);
        token = Tokenizer::nextToken(latexf);
        skipLines(token);
        if (openAtom(token) || closeAtom(token))
        {
            printOut(token);
            token = Tokenizer::nextToken(latexf);
            std::vector<std::string> localScope;
            scopeId++;
            body(token, localScope, scopeId);
            if (token == Constants::RIGHT)
            {
                printOut(token);
                openClose.pop_back();
                token = Tokenizer::nextToken(latexf);
                skipLines(token);
                if (openAtom(token) || closeAtom(token))
                {
                    printOut(token);
                }
                else if (token[0] == Constants::DOT)
                {
                }
                else
                    syntaxError(token);
            }
            else
                syntaxError("right");
        }
        else if (token[0] == Constants::DOT)
        {
        }
        else
            syntaxError("unexpected after left: " + token);
    }
    else if (token == Constants::BIGL)
    {
        printOut(token);
        openClose.push_back(token);
        token = Tokenizer::nextToken(latexf);
        skipLines(token);
        if (openAtom(token) || closeAtom(token))
        {
            printOut(token);
            token = Tokenizer::nextToken(latexf);
            std::vector<std::string> localScope;
            scopeId++;
            body(token, localScope, scopeId);
            if (token == Constants::BIGR)
            {
                printOut(token);
                openClose.pop_back();
                token = Tokenizer::nextToken(latexf);
                skipLines(token);
                if (openAtom(token) || closeAtom(token))
                {
                    printOut(token);
                }
                else if (token[0] == Constants::DOT)
                {
                }
                else
                    syntaxError(token);
            }
            else
                syntaxError("big right");
        }
        else if (token[0] == Constants::DOT)
        {
        }
        else
            syntaxError("unexpected after left: " + token);
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

bool TexParser::binAtom(std::string &token, std::vector<std::string> &itemsScope, int scope)
{
    bool e = true;
    if (token[0] == Constants::ADD || token[0] == Constants::SUB || token[0] == Constants::MULT || token[0] == Constants::SLASH)
    {
        printOut(token);
        token = Tokenizer::nextToken(latexf);
    }
    else if (token[0] == Constants::POWER)
    {
        printOut(token);
        token = Tokenizer::nextToken(latexf);
        skipLines(token);
        if (token[0] == Constants::OPENBRACE)
        {
            printOut(token);
            openClose.push_back(token);
            token = Tokenizer::nextToken(latexf);
            std::vector<std::string> localScope;
            scopeId++;
            body(token, localScope, scopeId);
            if (token[0] == Constants::CLOSEBRACE)
            {
                printOut(token);
                openClose.pop_back();
            }
            else
                syntaxError(std::string(1, Constants::CLOSEBRACE));

            token = Tokenizer::nextToken(latexf);
        }
        else if (translateFuncs[token] != "" || token[0] == Constants::POWER || unexpectedTokens(token))
        {
            syntaxError("not allowed");
        }
        else if ((isWord(token) || Tokenizer::isDigit(token[0])))
        {
            printOut(token);
            token = Tokenizer::nextToken(latexf);
            skipLines(token);
            if (token[0] == Constants::POWER)
            {
                syntaxError("double super script");
                token = Tokenizer::nextToken(latexf);
            }
        }
        else if (boundaryItem(token, itemsScope, scope))
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
        printOut(token);
        e = true;
    }
    return e;
}

bool TexParser::openAtom(std::string &token)
{
    return token[0] == Constants::OPENBRACKET || token[0] == Constants::OPENPARENTHESIS ||
           token[0] == Constants::OR || token == Constants::SINGLEVERT ||
           token == Constants::DOUBLEVERT || token == "\\|" || token == "\\{" || token[0] == Constants::SLASH ||
           token[0] == Constants::LESSTHAN || token == Constants::LANGLE ||
           token == Constants::LFLOOR || token == Constants::LCEIL || token == Constants::BACKSLASH ||
           token == Constants::SINGLEUPARROW || token == Constants::DOUBLEUPARROW ||
           token == Constants::SINGLEDOWNARROW || token == Constants::DOUBLEDOWNARROW;
}

bool TexParser::closeAtom(std::string &token)
{
    return token[0] == Constants::CLOSEBRACKET || token[0] == Constants::CLOSEPARENTHESIS ||
           token[0] == Constants::OR || token == Constants::SINGLEVERT ||
           token == Constants::DOUBLEVERT || token == "\\|" || token == "\\}" || token[0] == Constants::SLASH ||
           token[0] == Constants::GREATERTHAN || token == Constants::RANGLE ||
           token == Constants::RFLOOR || token == Constants::RCEIL || token == Constants::BACKSLASH ||
           token == Constants::SINGLEUPARROW || token == Constants::DOUBLEUPARROW ||
           token == Constants::SINGLEDOWNARROW || token == Constants::DOUBLEDOWNARROW;
}

bool TexParser::punctAtom(std::string &token)
{
    bool e = false;
    if (token[0] == Constants::COMMA || token[0] == Constants::SINGLEQUOTE || token[0] == Constants::DOUBLEQUOTE)
    {
        e = true;
        printOut(token);
    }
    return e;
}

bool TexParser::radAtom(std::string &token, std::vector<std::string> &itemsScope, int scope)
{
    bool e = true;
    if (token == Constants::SQRT)
    {
        printOut(token);
        token = Tokenizer::nextToken(latexf);
        skipLines(token);
        sqrtExpr(token, itemsScope, scope);
    }
    else if (token == Constants::RADICAL)
    {
        //TODO
    }
    else
        e = false;
    return e;
}

void TexParser::sqrtExpr(std::string &token, std::vector<std::string> &itemsScope, int scope)
{
    if (token[0] == Constants::OPENBRACKET)
    {
        openClose.push_back(token);
        token = Tokenizer::nextToken(latexf);
        std::vector<std::string> localScope;
        scopeId++;
        body(token, localScope, scopeId);
        if (token[0] == Constants::CLOSEBRACKET)
        {
            openClose.pop_back();
            token = Tokenizer::nextToken(latexf);
            skipLines(token);
            expr1(token, itemsScope, scope);
        }
        else
            syntaxError(std::string(1, Constants::CLOSEBRACKET));
    }
    else if (token[0] == Constants::BACKS)
        syntaxError("backslash not allowed");
    else if (isEndExpr(token))
        syntaxError("sqrt needs argument");
    else
        expr1(token, itemsScope, scope);
}

void TexParser::expr1(std::string &token, std::vector<std::string> &itemsScope, int scope)
{
    if (token[0] == Constants::OPENBRACE)
    {
        printOut(token);
        openClose.push_back(token);
        token = Tokenizer::nextToken(latexf);
        std::vector<std::string> localScope;
        scopeId++;
        body(token, localScope, scopeId);
        if (token[0] == Constants::CLOSEBRACE)
        {
            printOut(token);
            openClose.pop_back();
        }
        else
            syntaxError(std::string(1, Constants::CLOSEBRACE));
    }
    else
    {
        if (!unexpectedTokens(token))
            printOut(token);
    }
}

bool TexParser::innerAtom(std::string &token, std::vector<std::string> &itemsScope, int scope)
{
    bool e = true;
    if (token == Constants::FRAC)
    {
        token = Tokenizer::nextToken(latexf);
        skipLines(token);
        if (token[0] == Constants::OPENBRACE)
        {
            openClose.push_back(token);
            printOut(token);
            token = Tokenizer::nextToken(latexf);
            std::vector<std::string> localScope;
            scopeId++;
            body(token, localScope, scopeId);
            if (token[0] == Constants::CLOSEBRACE)
            {
                printOut(token);
                openClose.pop_back();

                token = Tokenizer::nextToken(latexf);
                skipLines(token);
                if (token[0] == Constants::OPENBRACE)
                {
                    printOut(Constants::FRAC);
                    printOut(token);
                    openClose.push_back(token);
                    token = Tokenizer::nextToken(latexf);
                    std::vector<std::string> localScope;
                    scopeId++;
                    body(token, localScope, scopeId);
                    if (token[0] == Constants::CLOSEBRACE)
                    {
                        openClose.pop_back();
                        printOut(token);
                    }
                    else
                        syntaxError(std::string(1, Constants::CLOSEBRACE));
                }
                else if (translateFuncs[token] != "" || translateGenFracs[token] != "" || token[0] == Constants::POWER || unexpectedTokens(token) || isEndExpr(token))
                {
                    std::string e = Constants::FRAC;
                    e.append(" missing second argument");
                    syntaxError(e);
                }
                else
                {
                    printOut(Constants::FRAC);
                    printOut(token);
                }
            }
            else
                syntaxError(std::string(1, Constants::CLOSEBRACE));
        }
        else if (translateFuncs[token] != "" || translateGenFracs[token] != "" || token[0] == Constants::POWER || unexpectedTokens(token) || isEndExpr(token))
        {
            std::string e = Constants::FRAC;
            e.append(" missing argument");
            syntaxError(e);
        }
        else
        {
            printOut(token);
            token = Tokenizer::nextToken(latexf);
            skipLines(token);
            if (translateFuncs[token] != "" || translateGenFracs[token] != "" || token[0] == Constants::POWER || unexpectedTokens(token) || isEndExpr(token))
            {
                std::string e = Constants::FRAC;
                e.append(" missing second argument");
                syntaxError(e);
            }
            else
            {
                printOut(Constants::FRAC);
                printOut(token);
            }
        }
    }
    else
        e = false;

    return e;
}

bool TexParser::generalizedFracs(std::string &token, std::vector<std::string> &itemsScope, int scope)
{
    bool e = true;
    if (token == Constants::OVER)
    {
        if (itemsScope.size() && translateGenFracs[itemsScope.back()] != "")
        {
            syntaxError("missing {");
        }
        else
        {
            if (outputMode)
            {
                output << "} ";
                printOut(token);
            }
            else
            {
                std::pair<int, std::string> p(scope, token);
                genFracs.insert(p);
            }
            itemsScope.push_back(token);
            if (outputMode)
                output << "{ ";
            token = Tokenizer::nextToken(latexf);
            body(token, itemsScope, scope);
            if (outputMode)
                output << "} ";
        }
    }
    else if (token == Constants::ATOP)
    {
        if (itemsScope.size() && translateGenFracs[itemsScope.back()] != "")
        {
            syntaxError("missing {");
        }
        else
        {
            if (outputMode)
            {
                output << "} ";
                printOut(token);
            }
            else
            {
                std::pair<int, std::string> p(scope, token);
                genFracs.insert(p);
            }
            itemsScope.push_back(token);
            if (outputMode)
                output << "{ ";
            token = Tokenizer::nextToken(latexf);
            body(token, itemsScope, scope);
            if (outputMode)
                output << "} } ";
        }
    }
    else if (token == Constants::CHOOSE)
    {
        if (itemsScope.size() && translateGenFracs[itemsScope.back()] != "")
        {
            syntaxError("missing {");
        }
        else
        {
            if (outputMode)
            {
                output << "} ";
                printOut(token);
            }
            else
            {
                std::pair<int, std::string> p(scope, token);
                genFracs.insert(p);
            }
            itemsScope.push_back(token);
            if (outputMode)
                output << "{ ";
            token = Tokenizer::nextToken(latexf);
            body(token, itemsScope, scope);
            if (outputMode)
                output << "} } right )";
        }
    }
    else
        e = false;

    return e;
}

bool TexParser::overAtom(std::string &token, std::vector<std::string> &itemsScope, int scope)
{
    bool e = false;
    if (token == Constants::OVERLINE)
    {
        e = true;
        token = Tokenizer::nextToken(latexf);
        skipLines(token);
        if (token[0] == Constants::OPENBRACE)
        {
            printOut(token);
            openClose.push_back(token);
            token = Tokenizer::nextToken(latexf);
            std::vector<std::string> localScope;
            scopeId++;
            body(token, localScope, scopeId);
            if (token[0] == Constants::CLOSEBRACE)
            {
                openClose.pop_back();
                printOut(token);
                printOut(Constants::OVERLINE);
            }
            else
                syntaxError(std::string(1, Constants::CLOSEBRACE));
        }
        else if (translateFuncs[token] != "" || translateGenFracs[token] != "" ||
                 token[0] == Constants::POWER || unexpectedTokens(token) || isEndExpr(token))
        {
            std::string e = Constants::OVERLINE;
            e.append(" missing argument");
            syntaxError(e);
        }
        else
        {
            printOut(token);
            printOut(Constants::OVERLINE);
        }
    }
    return e;
}

bool TexParser::underAtom(std::string &token, std::vector<std::string> &itemsScope, int scope)
{
    bool e = false;
    if (token == Constants::UNDERLINEW)
    {
        e = true;
        token = Tokenizer::nextToken(latexf);
        skipLines(token);
        if (token[0] == Constants::OPENBRACE)
        {
            printOut(token);
            openClose.push_back(token);
            token = Tokenizer::nextToken(latexf);
            std::vector<std::string> localScope;
            scopeId++;
            body(token, localScope, scopeId);
            if (token[0] == Constants::CLOSEBRACE)
            {
                openClose.pop_back();
                printOut(token);
                printOut(Constants::UNDERLINEW);
            }
            else
                syntaxError(std::string(1, Constants::CLOSEBRACE));
        }
        else if (translateFuncs[token] != "" || translateGenFracs[token] != "" ||
                 token[0] == Constants::POWER || unexpectedTokens(token) || isEndExpr(token))
        {
            std::string e = Constants::UNDERLINEW;
            e.append(" missing argument");
            syntaxError(e);
        }
        else
        {
            printOut(token);
            printOut(Constants::UNDERLINEW);
        }
    }
    return e;
}

bool TexParser::accAtom(std::string &token, std::vector<std::string> &itemsScope, int scope)
{
    bool e = false;
    std::string t = token;
    if (token == Constants::HAT || token == Constants::TILDE || token == Constants::ODOT ||
        token == Constants::VEC || token == Constants::BAR)
    {
        e = true;
        token = Tokenizer::nextToken(latexf);
        skipLines(token);
        if (token[0] == Constants::OPENBRACE)
        {
            printOut(token);
            openClose.push_back(token);
            token = Tokenizer::nextToken(latexf);
            std::vector<std::string> localScope;
            scopeId++;
            body(token, localScope, scopeId);
            if (token[0] == Constants::CLOSEBRACE)
            {
                openClose.pop_back();
                printOut(token);
                printOut(t);
            }
            else
                syntaxError(std::string(1, Constants::CLOSEBRACE));
        }
        else if (translateFuncs[token] != "" || translateGenFracs[token] != "" ||
                 token[0] == Constants::POWER || unexpectedTokens(token) || isEndExpr(token))
        {
            syntaxError(t + " missing argument");
        }
        else
        {
            printOut(token);
            printOut(t);
        }
    }
    return e;
}

bool TexParser::opAtom(std::string &token, std::vector<std::string> &itemsScope, int scope)
{
    if (token == Constants::SUM || token == Constants::PROD || token == Constants::COPROD || token == Constants::INTEGRAL)
    {
        printOut(token);
        token = Tokenizer::nextToken(latexf);
        skipLines(token);
        if (token[0] == Constants::UNDERLINE)
        {
            printOut("from");
            token = Tokenizer::nextToken(latexf);
            skipLines(token);
            if (token[0] == Constants::OPENBRACE)
            {
                printOut(token);
                openClose.push_back(token);
                token = Tokenizer::nextToken(latexf);
                std::vector<std::string> localScope;
                scopeId++;
                body(token, localScope, scopeId);
                if (token[0] == Constants::CLOSEBRACE)
                {
                    openClose.pop_back();
                    printOut(token);
                }
                else
                    syntaxError(std::string(1, Constants::CLOSEBRACE));
            }
            else
                printOut(token);
            token = Tokenizer::nextToken(latexf);
            skipLines(token);
            if (token[0] == Constants::POWER)
            {
                printOut("to");
                token = Tokenizer::nextToken(latexf);
                skipLines(token);
                if (token[0] == Constants::OPENBRACE)
                {
                    printOut(token);
                    openClose.push_back(token);
                    token = Tokenizer::nextToken(latexf);
                    std::vector<std::string> localScope;
                    scopeId++;
                    body(token, localScope, scopeId);
                    if (token[0] == Constants::CLOSEBRACE)
                    {
                        openClose.pop_back();
                        printOut(token);
                    }
                    else
                        syntaxError(std::string(1, Constants::CLOSEBRACE));
                }
                else
                    printOut(token);

                token = Tokenizer::nextToken(latexf);
            }
        }
        return true;
    }
    return false;
}

bool TexParser::delimCheck(std::string &token)
{
    if (token == Constants::DELIM)
    {
        delimStart = ' ';
        delimEnd = ' ';
        if (delimStart2 == ' ')
            printOut("delim");
        token = Tokenizer::nextToken(latexf);
        skipLines(token);
        if (token[0] != Constants::BACKS)
        {
            delimStart = token[0];
            if (delimStart2 == ' ')
                if(outputMode)
                    output<<token;
            token = Tokenizer::nextToken(latexf);
            if (token[0] != Constants::BACKS)
            {
                delimEnd = token[0];
                if (delimStart2 == ' ')
                {
                    printOut(token);
                    if (outputMode)
                    {
                        delimStart2 = delimStart;
                        delimEnd2 = delimEnd;
                    }
                }
            }
            else
                syntaxError("argument for delim not allowed");
        }
        else
            syntaxError("argument for delim not allowed");
        return true;
    }
    return false;
}

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