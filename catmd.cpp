// clang++ -std=c++11 -stdlib=libc++ -o test test.cpp
#include <cstdlib>
#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include "replace.h"

//stolen code from stack overflow
namespace std
{

template<class BidirIt, class Traits, class CharT, class UnaryFunction>
std::basic_string<CharT> regex_replace(BidirIt first, BidirIt last,
    const std::basic_regex<CharT,Traits>& re, UnaryFunction f)
{
    std::basic_string<CharT> s;

    typename std::match_results<BidirIt>::difference_type
        positionOfLastMatch = 0;
    auto endOfLastMatch = first;

    auto callback = [&](const std::match_results<BidirIt>& match)
    {
        auto positionOfThisMatch = match.position(0);
        int diff = positionOfThisMatch - positionOfLastMatch;

        auto startOfThisMatch = endOfLastMatch;
        std::advance(startOfThisMatch, diff);

        s.append(endOfLastMatch, startOfThisMatch);
        s.append(f(match));

        auto lengthOfMatch = match.length(0);

        positionOfLastMatch = positionOfThisMatch + lengthOfMatch;

        endOfLastMatch = startOfThisMatch;
        std::advance(endOfLastMatch, lengthOfMatch);
    };

    std::regex_iterator<BidirIt> begin(first, last, re), end;
    std::for_each(begin, end, callback);

    s.append(endOfLastMatch, last);

    return s;
}

template<class Traits, class CharT, class UnaryFunction>
std::string regex_replace(const std::string& s,
    const std::basic_regex<CharT,Traits>& re, UnaryFunction f)
{
    return regex_replace(s.cbegin(), s.cend(), re, f);
}

} 
//end stolen code

int main(int argc, const char *argv[])
{
    std::ifstream file;

    const char* fileName = argv[1];
    file.open(fileName, std::ios::in);

    std::string line;
    std::string fullData;

    bool inCodeBlock = false;

    std::regex emptyCheckboxRe("(^\\s*)\\* \\[ \\] (.*)");
    std::regex checkedCheckboxRe("(^\\s*)\\* \\[x\\] (.*)");
    std::regex codeBlockStartRe("```(.*)$");
    std::regex inlineCodeRe("`(.*?)`");
    std::regex bulletPointRe("(^\\s*)(\\*|\\+|-) (.*)");
    std::regex boldRe("(\\*|_){2}(.+?)\\1{2}");
    std::regex italicRe("(\\*|_)([^*\n]+)\\1");
    std::regex headersRe("^(#+) +(.*)");
    std::regex markdownLinkRe("\\[(.+?)\\]\\((.+?)\\)");
    std::regex blockQuoteRe("(^\\s*)>(.*)");
    std::regex hrRe("^-{3,}$");
    std::regex linkRe("(A-Za-z]+)://([^'\" ]+)");

    while(std::getline(file, line)){
        //code
        line = std::regex_replace(line, codeBlockStartRe, [&](const std::smatch& m){
            inCodeBlock = !inCodeBlock;
            return inCodeBlock ? codeBlockStart(m[0], m[1]) : codeBlockEnd(m[0]);
        });
        if(inCodeBlock){
            line = codeBlockFormatting(line);
            fullData += line + "\n";
            continue;
        }
        line = std::regex_replace(line, inlineCodeRe, [](const std::smatch& m){
            return inlineCode(m[0], m[1]);
        });
        //checklists
        //empty
        line = std::regex_replace(line, emptyCheckboxRe, [](const std::smatch& m){
            return emptyCheckbox(m[0], m[1], m[2]);
        });
        //checked
        line = std::regex_replace(line, checkedCheckboxRe, [](const std::smatch& m){
            return checkedCheckbox(m[0], m[1], m[2]);
        });

        //bullet points
        line = std::regex_replace(line, bulletPointRe, [](const std::smatch& m){
            return bulletPoint(m[0], m[1], m[2], m[3]);
        });

        //bold
        line = std::regex_replace(line, boldRe, [](const std::smatch& m){
            return bold(m[0], m[1], m[2]);
        });

        //italic
        line = std::regex_replace(line, italicRe, [](const std::smatch& m){
            return italic(m[0], m[1], m[2]);
        });
        
        //headers
        line = std::regex_replace(line, headersRe, [](const std::smatch& m){
            return headers(m[0], m[1], m[2]);
        });

        //markdown links
        line = std::regex_replace(line, markdownLinkRe, [](const std::smatch& m){
            return markdownLink(m[0], m[1], m[2]);
        });

        //block quotes
        line = std::regex_replace(line, blockQuoteRe, [](const std::smatch& m){
            return blockQuote(m[0], m[1], m[2]);
        });

        //hr
        line = std::regex_replace(line, hrRe, [](const std::smatch& m){
            return hr(m[0]);
        });

        line = std::regex_replace(line, linkRe, [](const std::smatch& m){
            return link(m[0], m[1], m[2]);
        });

        //strikethrough
        line = std::regex_replace(line, std::regex("~~(.*)~~"), [](const std::smatch& m){
            return strikethrough(m[0], m[1]);
        });

        fullData += line + "\n";
    }
    std::cout << fullData << std::endl;

    return 0;
}