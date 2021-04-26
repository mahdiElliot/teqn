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
        while ((c == ' ' || c == '\n' || c == '\r' || c == '%') && !myfile.eof())
        {
            if (c == '%')
            {
                myfile.get(c);
                while ((c != '\r' || c != '%') && !myfile.eof())
                    myfile.get(c);
            }
            if (c == '\n')
                line++;
            myfile.get(c);
        }

        if (!myfile.eof())
        {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || isDigit(c))
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
            else if (c == Constants::BACKSLASH)
            {
                token = c;
                myfile.get(c);
                if (!myfile.eof() && c == Constants::BACKSLASH)
                    return "\\\\";

                while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') &&
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

// if (c == Constants::DOT)
//                 return std::string(1, Constants::DOT);
//             else if (c == Constants::UNDERLINE)
//                 return std::string(1, Constants::UNDERLINE);
//             else if (c == Constants::SPACEHAT)
//                 return std::string(1, Constants::SPACEHAT);
//             else if (c == Constants::COMMA)
//                 return std::string(1, Constants::COMMA);
//             else if (c == Constants::SEMICOLON)
//                 return std::string(1, Constants::SEMICOLON);
//             else if (c == Constants::OPENBRACE)
//                 return std::string(1, Constants::OPENBRACE);
//             else if (c == Constants::CLOSEBRACE)
//                 return std::string(1, Constants::CLOSEBRACE);
//             else if (c == Constants::OPENPARENTHESIS)
//                 return std::string(1, Constants::OPENPARENTHESIS);
//             else if (closeParenthesis(c))
//                 return ")";
//             else if (openBracket(c))
//                 return "[";
//             else if (closeBracket(c))
//                 return "]";
//             else if (add(c))
//                 return "+";
//             else if (sub(c))
//                 return "-";
//             else if (mul(c))
//                 return "*";
//             else if (slash(c))
//                 return "/";
//             else if (powerSign(c))
//                 return "^";
//             else if (singleQ(c))
//             {
//                 return "'";
//             }
//             else if (backQ(c))
//             {
//                 return "`";
//             }
//             else if (colon(c))
//             {
//                 return ":";
//             }
//             else if (And(c))
//             {
//                 return "&";
//             }
//             else if (Or(c))
//             {
//                 return "|";
//             }
//             else if (Not(c))
//                 return "!";
//             else if (equal(c))
//             {
//                 return "=";
//             }
//             else if (greaterThan(c))
//             {
//                 return ">";
//             }
//             else if (lessThan(c))
//             {
//                 return "<";
//             }
//         }