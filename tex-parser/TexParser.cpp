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

void TexParser::start()
{
    std::string token = Tokenizer::nextToken(latexf);
    while (!latexf.eof() && token != ".BL")
        token = Tokenizer::nextToken(latexf);
    if (latexf.eof())
        return;
    token = Tokenizer::nextToken(latexf);
    expr(token);
}

void TexParser::expr(std::string token)
{
}

bool TexParser::isNumber(std::string number)
{
    auto result = double();
    auto i = std::istringstream(number);

    i >> result;

    return !i.fail() && i.eof();
}