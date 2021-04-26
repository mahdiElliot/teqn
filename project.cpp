#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "utils/Tokenizer.h"

#define latexEq "latexEq.txt"

using namespace std;

ifstream latexf;

bool isNumber(string number)
{
    auto result = double();
    auto i = istringstream(number);

    i >> result;

    return !i.fail() && i.eof();
}

void run();
void start();
void expr(string token);

int main()
{
    // run();
    latexf.open(latexEq, ios::in);
    ofstream output;
    output.open("out.txt", ios::out);
    string token = Tokenizer::nextToken(latexf);
    token = Tokenizer::nextToken(latexf);
    cout<<token<<endl;
    latexf.close();
    output.close();
    return 0;
}

void run()
{
    latexf.open(latexEq, ios::in);
    ofstream output;
    output.open("out.txt", ios::out);
    start();
    latexf.close();
    output.close();
}

void start()
{
    string token = Tokenizer::nextToken(latexf);
    while (token != ".BL")
        token = Tokenizer::nextToken(latexf);
    token = Tokenizer::nextToken(latexf);
    expr(token);
}

void expr(string token)
{

}
