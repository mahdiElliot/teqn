#include "Tokenizer.h"
#include "Constants.h"

int Tokenizer::line = 1;

int Tokenizer::getLine()
{
    return line;
}

bool Tokenizer::isDigit(char c)
{
    return c >= '0' && c <= '9';
}

std::string Tokenizer::nextToken(std::ifstream &myfile)
{
    char c;
    std::string token = "";

    if (!myfile.eof())
    {
        myfile.get(c);
        short a = (short)c;
        while ((c == ' ') && !myfile.eof())
        {
            if (c == '\n')
                line++;
            myfile.get(c);
        }

        if (!myfile.eof())
        {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || isDigit(c) || c == Constants::DOT)
            {
                token = c;
            }
            else if (c == Constants::BACKS)
            {
                token = c;
                myfile.get(c);
                if (!((c >= 'a' && c <= 'z') ||
                      (c >= 'A' && c <= 'Z')) &&
                    !myfile.eof())
                    return token + c;

                if (c != ' ' && !myfile.eof())
                    token = token + c;

                myfile.get(c);

                while ((c != ' ' || c != Constants::BACKS) &&
                       ((c >= 'a' && c <= 'z') ||
                        (c >= 'A' && c <= 'Z')) &&
                       !myfile.eof())
                {
                    token = token + c;
                    myfile.get(c);
                }

                if (!myfile.eof())
                {
                    myfile.unget();
                }
            }
            else
                token = c;
        }
    }
    return token;
}

std::string Tokenizer::nextToken2(std::ifstream &myfile)
{
    char c;
    std::string token = "";

    if (!myfile.eof())
    {
        myfile.get(c);
        short a = (short)c;
        while ((c == ' ') && !myfile.eof())
        {
            if (c == '\n')
                line++;
            myfile.get(c);
        }

        if (!myfile.eof())
        {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || isDigit(c) || c == Constants::DOT)
            {
                token = c;
                myfile.get(c);
                while (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                        isDigit(c) || c == Constants::DOT) &&
                       !myfile.eof())
                {
                    token = token + c;
                    myfile.get(c);
                }

                if (!myfile.eof())
                {
                    myfile.unget();
                }
            }
            else if (c == Constants::BACKS)
            {
                token = c;
                myfile.get(c);
                if (!((c >= 'a' && c <= 'z') ||
                      (c >= 'A' && c <= 'Z')) &&
                    !myfile.eof())
                    return token + c;

                if (c != ' ' && !myfile.eof())
                    token = token + c;

                myfile.get(c);

                while ((c != ' ' || c != Constants::BACKS) &&
                       ((c >= 'a' && c <= 'z') ||
                        (c >= 'A' && c <= 'Z')) &&
                       !myfile.eof())
                {
                    token = token + c;
                    myfile.get(c);
                }

                if (!myfile.eof())
                {
                    myfile.unget();
                }
            }
            else
                token = c;
        }
    }
    return token;
}

