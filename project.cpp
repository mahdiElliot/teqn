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
    // std::istream::streampos p = f.tellg(); // or, in C++11: auto p = f.tellg();
    // cout << Tokenizer::nextToken2(f) << endl;
    // f.clear() here if there's a possibility that the stream is in a bad state
    // f.seekg(0, std::ios::beg);
    // f.clear();
    // f.seekg(p);
    // cout << Tokenizer::nextToken2(f) << endl;
    // cout<<Tokenizer::nextToken(f)<<endl;
    // cout<<Tokenizer::nextToken(f)<<endl;
    // cout<<Tokenizer::nextToken(f)<<endl;
    // cout<<Tokenizer::nextToken(f)<<endl;
    // cout<<Tokenizer::nextToken(f)<<endl;
    // cout<<Tokenizer::nextToken(f)<<endl;
    // cout<<Tokenizer::nextToken(f)<<endl;
    return 0;
}
