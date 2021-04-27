#include <iostream>
#include <string>
#include "tex-parser/TexParser.h"
#define latexEq "latexEq.txt"

using namespace std;

int main()
{
    TexParser parser(latexEq, "out.txt");
    parser.parse();
    // ifstream f;
    // f.open(latexEq, ios::in);
    // cout<<Tokenizer::nextToken(f)<<endl;
    // cout<<Tokenizer::nextToken(f)<<endl;
    // cout<<Tokenizer::nextToken(f)<<endl;
    // cout<<Tokenizer::nextToken(f)<<endl;
    // cout<<Tokenizer::nextToken(f)<<endl;
    // cout<<Tokenizer::nextToken(f)<<endl;
    // cout<<Tokenizer::nextToken(f)<<endl;
    // cout<<Tokenizer::nextToken(f)<<endl;
    // cout<<Tokenizer::nextToken(f)<<endl;

    return 0;
}

