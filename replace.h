#pragma once
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

std::string strikethrough(std::string fullFind, std::string strikethroughType, std::string text){
    return "\033[9m" + text + "\033[0m";
}

std::string emptyCheckbox(std::string fullFind, std::string whiteSpace, std::string text){
    return whiteSpace + "*\033[31m [ ]\033[39m " + text;
}
std::string checkedCheckbox(std::string fullFind, std::string whiteSpace, std::string text){
    return whiteSpace + "*\033[32m [x]\033[39m " + text;
}

std::string codeBlockStart(std::string fullFind, std::string language){
    return "\033[32m" + language + "\033[0m\033[9;39m\n------------------------------------";
}
std::string codeBlockEnd(std::string fullFind){
    return "\033[0m\033[9m------------------------------------\033[0m";
}
std::string codeBlockFormatting(std::string line){
    return "\033[2m" + line + "\033[0m";
}

std::string inlineCode(std::string fullFind, std::string text){
    return "\033[2m" + text + "\033[0m";
}

std::string tabbed(std::string fullFind, std::string whitespace, std::string text){
    return whitespace + "\033[2m" + text + "\033[0m";
}

std::string bulletPoint(std::string fullFind, std::string whitespace, std::string bulletType, std::string text){
    return whitespace + bulletType + " " + text;
}

std::string bold(std::string fullFind, std::string boldType, std::string text){
    return "\033[1m" + text + "\033[0m";
}

std::string italic(std::string fullFind, std::string italicType, std::string text){
    return "\033[3m" + text + "\033[0m";
}

std::string headers(std::string fullFind, std::string hashes, std::string text){
    return "\033[1;2;32m" + hashes + "\033[0;39m \033[4m\033[1m" + text + "\033[0m";
}

std::string markdownLink(std::string fullFind, std::string displayText, std::string link){
    return "\033[1m" + displayText + "\033[0m (\033[34m" + link + "\033[0;39m)";
}

std::string blockQuote(std::string fullFind, std::string whitespace, std::string text){
    return whitespace + "|\033[2;3m" + text + "\033[0m"; 
}

std::string hr(std::string fullFind){
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);  
    std::string _hr = "";
    for(int i = 0; i < size.ws_col; i++){
        _hr += "-";
    }
    return "\033[9m" + _hr + "\033[0m"; 
}

std::string link(std::string fullFind, std::string protocol, std::string url){
    return "\033[34m" + fullFind + "\033[39m";
}

std::string html(std::string fullFind, std::string element, std::string elementAttrs, std::string innerText){
    return "<\033[35m" + element + "\033[2;36m" + elementAttrs + "\033[0m>" + innerText + "</\033[35m" + element + "\033[39m>";
}
