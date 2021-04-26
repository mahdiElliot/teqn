#include <iostream>
#include <vector>
#include <string>
#include "tex-parser/TexParser.h"
#define latexEq "latexEq.txt"

using namespace std;

int main()
{
    TexParser parser(latexEq, "out.txt");
    parser.parse();
    return 0;
}

