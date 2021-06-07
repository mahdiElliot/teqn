#include <iostream>
#include <string>
#include "tex-parser/TexParser.h"
#define latexEq "latexEq.txt"

using namespace std;

int main()
{
    ofstream input;
    input.open(latexEq, ios::out);
    string line;
    while (getline(cin, line)){
        input << line << '\n';
    }
    input.close();
    TexParser parser(latexEq, "out.txt");
    parser.parse();
    remove(latexEq);
    fstream output;
    output.open("out.txt");
    string out;
    while (getline(output, out))
        cout << out << '\n';
    return 0;
}
