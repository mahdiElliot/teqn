#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <math.h>

#define latexEq "latexEq.txt"

using namespace std;

int line = 1;

bool greaterThan(char gt)
{
    return gt == '>';
}

bool lessThan(char lt)
{
    return lt == '<';
}

bool equal(char eq)
{
    return eq == '=';
}

bool comma(char c)
{
    return c == ',';
}

bool dot(char c)
{
    return c == '.';
}

bool colon(char d)
{
    return d == ':';
}

bool semiColon(char s)
{
    return s == ';';
}

bool isDigit(char digit)
{
    return digit >= '0' && digit <= '9';
}

bool openBrace(char obrace)
{
    return obrace == '{';
}

bool closeBrace(char cbrace)
{
    return cbrace == '}';
}

bool openParenthesis(char op)
{
    return op == '(';
}

bool closeParenthesis(char cp)
{
    return cp == ')';
}

bool openBracket(char b)
{
    return b == '[';
}

bool closeBracket(char b)
{
    return b == ']';
}

bool add(char p)
{
    return p == '+';
}

bool sub(char s)
{
    return s == '-';
}

bool mul(char m)
{
    return m == '*';
}

bool slash(char d)
{
    return d == '/';
}

bool backSlash(char b)
{
    return b == '\\';
}

bool doubleQ(char dq)
{
    return dq == '\"';
}

bool backQ(char c)
{
    return c == '`';
}

bool singleQ(char sq)
{
    return sq == '\'';
}

bool underline(char c)
{
    return c == '_';
}

bool spaceHat(char c)
{
    return c == '~';
}

bool powerSign(char c)
{
    return c == '^';
}

bool And(char a)
{
    return a == '&';
}

bool Or(char o)
{
    return o == '|';
}

bool Not(char n)
{
    return n == '!';
}

bool isWord(string letter)
{
    bool valid = false;
    if ((int(letter[0]) >= 65 && int(letter[0]) <= 90) ||
        (int(letter[0]) >= 97 && int(letter[0] <= 122)) || letter[0] == '_')
    {
        if (letter.size() == 1)
            valid = true;
        for (int i = 1; i < letter.size(); i++)
            if (isDigit(letter[i]) || (int(letter[i]) >= 65 && int(letter[i]) <= 90) ||
                (int(letter[i]) >= 97 && int(letter[i] <= 122)) || letter[i] == '_')
                valid = true;
            else
                return false;
    }

    return valid;
}

bool isNumber(string number)
{
    auto result = double();
    auto i = istringstream(number);

    i >> result;

    return !i.fail() && i.eof();
}

string nextToken(ifstream &myfile)
{
    char c;
    string token = "";

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
                        isDigit(c) || dot(c)) &&
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
            else if (backSlash(c))
            {
                token = c;
                myfile.get(c);
                if (!myfile.eof() && backSlash(c))
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
            else if (dot(c))
                return ".";
            else if (underline(c))
                return "_";
            else if (spaceHat(c))
                return "~";
            else if (comma(c))
                return ",";
            else if (c == '~')
                return "~";
            else if (semiColon(c))
                return ";";
            else if (openBrace(c))
                return "{";
            else if (closeBrace(c))
                return "}";
            else if (openParenthesis(c))
                return "(";
            else if (closeParenthesis(c))
                return ")";
            else if (openBracket(c))
                return "[";
            else if (closeBracket(c))
                return "]";
            else if (add(c))
                return "+";
            else if (sub(c))
                return "-";
            else if (mul(c))
                return "*";
            else if (slash(c))
                return "/";
            else if (powerSign(c))
                return "^";
            else if(singleQ(c))
            {
                return "'";
            }
            else if(backQ(c))
            {
                return "`";
            }
            else if (colon(c))
            {
                return ":";
            }
            else if (And(c))
            {
               return "&";
            }
            else if (Or(c))
            {
                return "|";
            }
            else if (Not(c))
                return "!";
            else if (equal(c))
            {
                return "=";
            }
            else if (greaterThan(c))
            {
                return ">";
            }
            else if (lessThan(c))
            {
                return "<";
            }
        }
    }
    return token;
}

void run();


int main()
{
    run();

    return 0;
}

void run()
{
    ifstream latexf;
    latexf.open(latexEq, ios::in);
    ofstream output;
    output.open("out.txt", ios::out);
    string token = nextToken(latexf);
    latexf.close();
    output.close();
}


