#ifndef LEXICALANALYZER_HPP
#define LEXICALANALYZER_HPP

#include "LexemePair.hpp"

#include <vector>
#include <unordered_map>
#include <iomanip>

#include <fstream>
#include <iostream>

//List of possible states for the state table
enum tokenState
{
    LETTER,
    DIGIT,
    DOLLAR,
    APOST,
    LEFTPAR,
    RIGHTPAR,
    LEFTCURL,
    RIGHTCURL,
    LEFTSQR,
    RIGHTSQR,
    COMMA,
    COLON,
    SEMICOLON,
    SPACE,
    PERIOD,
    EXCLA,
    OPER,
    BACKUP
};



#define ROWS 12
#define COLUMNS 18

class LexicalAnalyzer
{
  private:
    //Creates a map that associates characters to states (key = char, value = tokenState)
    std::unordered_map<char, tokenState> createStateMap();
    //Creates a map to help differentiate between identifiers and keywords (key = string, value = bool)
    std::unordered_map<std::string, bool> createKeySet();
    std::vector<LexemePair> tokens;
    int stateTable[ROWS][COLUMNS];
  public:
    LexicalAnalyzer(const char* filepath);

    bool lexer(const std::string &s);
    void printLexerTable();

    std::vector<LexemePair> get_tokens();
};
#endif