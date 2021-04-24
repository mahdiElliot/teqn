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

// bool isSign(char c)
// {
//     return greaterThan(c) || lessThan(c) || equal(c) ||
//      comma(c) || colon(c) || semiColon(c) || openBrace(c) ||
//      closeBrace(c) || openParenthesis(c) ||
//      closeParenthesis(c) || openBracket(c) ||
//      closeBracket(c) || add(c) || sub(c) || Or(c) ||
//      mul(c) || slash(c) || backSlash(c) || doubleQ(c) ||
//      singleQ(c) || And(c) || Not(c) || c == '.' || c == '^';

// }

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
                while ((c != '\r' || c!= '%') && !myfile.eof())
                    myfile.get(c);
            }
            if (c == '\n')
                line++;
            myfile.get(c);
        }

        if (!myfile.eof())
        {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                isDigit(c))
            {
                token = "";

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
            else if (backSlash(c))
            {
                char c1;
                myfile.get(c1);
                if (backSlash(c1))
                    return "\\\\";

                myfile.unget();
                return "\\";
            }
            else if (colon(c))
            {
                char c1;
                myfile.get(c1);
                if (equal(c1))
                    return ":=";

                myfile.unget();
                return ":";
            }
            else if (And(c))
            {
                if (!myfile.eof())
                {
                    char c1;
                    myfile.get(c1);
                    if (And(c1))
                        return "&&";

                    myfile.unget();
                    return "&";
                }
            }
            else if (Or(c))
            {
                if (!myfile.eof())
                {
                    char c1;
                    myfile.get(c1);
                    if (Or(c1))
                        return "||";

                    myfile.unget();
                    return "|";
                }
            }
            else if (Not(c))
                return "!";
            else if (equal(c))
            {
                if (!myfile.eof())
                {
                    char c1;
                    myfile.get(c1);
                    if (equal(c1))
                        return "==";

                    myfile.unget();
                    return "=";
                }
            }
            else if (greaterThan(c))
            {
                if (!myfile.eof())
                {
                    char c1;
                    myfile.get(c1);
                    if (equal(c1))
                        return ">=";

                    myfile.unget();
                    return ">";
                }
            }
            else if (c == lessThan(c))
            {
                if (!myfile.eof())
                {
                    char c1;
                    myfile.get(c1);
                    if (c1 == '=')
                        return "<=";

                    myfile.unget();
                    return "<";
                }
            }
        }
    }
    return token;
}

void expr(ifstream &inp, ofstream &outp, string &token);
void expr1(ifstream &inp, ofstream &outp, string &token);
void expr2(ifstream &inp, ofstream &outp, string &token);

int main()
{
    ifstream latexf;
    latexf.open(latexEq, ios::in);
    ofstream output;
    output.open("out.txt", ios::out);

    string token = nextToken(latexf);
    expr(latexf, output, token);
    latexf.close();
    output.close();

    return 0;
}

void expr(ifstream &inp, ofstream &outp, string &token)
{
    expr1(inp, outp, token);
    while (token == "=" || token == "+" || token == "*" || token == "^" || token == "-" ||
           token == "\\")
    {
        if (token == "\\")
        {
            token = nextToken(inp);
            if (token == ",")
            {
                /* code */
            }
            else if (token == "|")
            {
                /* code */
            }
        }

        token = nextToken(inp);
        expr(inp, outp, token);
    }
}

void expr1(ifstream &inp, ofstream &outp, string &token)
{
    expr2(inp, outp, token);
    while (token == "[")
    {
        token = nextToken(inp);
        expr1(inp, outp, token);
        if (token == "]")
            token = nextToken(inp);
        else
            cout << line << ": syntax error in ]\n";
    }
}

void expr2(ifstream &inp, ofstream &outp, string &token)
{
}

// // void expr3(ifstream &inp, ofstream &outp, string &token)
// {
//     if (token == "+" || token == "-")
//     {
//         token = nextToken(inp);
//         expr(inp, outp, token);
//     }
//     else if (token == "|")
//     {
//         token = nextToken(inp);
//         expr(inp, outp, token);
//         if (token == "|")
//         {
//             /* code */
//         }
//         else
//             cout << line << ": syntax error in |\n";
//     }
//     else if (token == "[")
//     {
//         token = nextToken(inp);
//         expr(inp, outp, token);
//         if (token == "]")
//         {
//             /* code */
//         }
//         else
//             cout << line << ": syntax error in ]\n";
//     }
//     else if (token == "(")
//     {
//         token = nextToken(inp);
//         expr(inp, outp, token);
//         if (token == ")")
//         {
//             /* code */
//         }
//         else
//             cout << line << ": syntax error )\n";
//     }
//     else if (token == "\\")
//     {
//         token = nextToken(inp);
//         if (token == "{")
//         {
//             token = nextToken(inp);
//             expr(inp, outp, token);
//             if (token == "\\")
//             {
//                 token = nextToken(inp);
//                 if (token == "}")
//                 {
//                     /* code */
//                 }
//                 else
//                     cout << line << ": syntax error in\\}\n";
//             }
//             else
//                 cout << line << ": syntax error in \\\n";
//         }
//         else
//         {
//             symbols(inp, outp, token);
//             expr(inp, outp, token);
//         }
//     }
//     else if (isWord(token))
//     {
//         token = nextToken(inp);
//         if (token == "(")
//         {
//             token = nextToken(inp);
//             clist(inp, outp, token);
//             if (token == ")")
//             {
//             }
//             else
//                 cout << line << ": syntax error in )\n";
//         }
//     }
//     else if (isNumber(token))
//     {
//     }
// }
