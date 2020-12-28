#include "LexicalAnalyzer.hpp"

LexicalAnalyzer::LexicalAnalyzer(const char* filepath)
{
      std::ifstream inFile;

      try {
          inFile.open(filepath);
          for (int i = 0; i < ROWS; i++) {
              for (int j = 0; j < COLUMNS; j++) {
                  inFile >> stateTable[i][j];
              }
          }
      }
      catch (std::exception& e) {
          std::cout << "ERROR: Could not read file.\n";
      }
}

bool LexicalAnalyzer::lexer(const std::string &s)
{
    tokenState tState = LETTER;
    int state = 0;

    std::unordered_map<char, tokenState> stateMap = createStateMap();
    std::unordered_map<std::string, bool> keySet = createKeySet();

    std::string::const_iterator curr = s.cbegin();
    std::string::const_iterator pplace = s.cbegin();

    while (curr != s.cend())
    {
        //check next character
        if (std::isalpha(*curr))
            tState = LETTER;
        else if (std::isdigit(*curr))
            tState = DIGIT;
        else if (stateMap.find(*curr) != stateMap.end())
            tState = stateMap[*curr];
        else
            return 1; // because the character is not valid

        //update state table
        state = stateTable[state][tState] - 1;

        //check for valid state
        if (state == -1)
            return 1;

        //update character pointer
        curr++;

        //update pplace (useful for extra spaces)
        if (state == 0)
            pplace = curr;

        //check if accepting state and update
        if (state == 2 || state == 4 || state == 7 || state == 8 || state == 9 || state == 11 || curr == s.cend())
        {
            LexemePair npair;

            //backup character pointer if necessary
            if (stateTable[state][BACKUP])
                curr--;

            //eof check
            if (!(state == 2 || state == 4 || state == 7 || state == 8 || state == 9 || state == 11))
                state = stateTable[state][SPACE] - 1;

            //if the ending state was because we had a comment, skip creating a pair
            if (state != 7 && state != 11)
            {
                npair.lexeme = s.substr(pplace - s.cbegin(), curr - pplace);

                if (state == 2)
                {
                    //npair.identifier = (keySet.find(npair.lexeme) != keySet.end() ? "KEYWORD" : "IDENTIFIER");

                    if (keySet.find(npair.lexeme) != keySet.end())
                    {
                        npair.identifier = "KEYWORD";
                        npair.type = KEYWORD;
                    }
                    else
                    {
                        npair.identifier = "IDENTIFIER";
                        npair.type = IDENTIFIER;
                    }
                }
                else if (state == 4)
                {
                    npair.identifier = "INTEGER";
                    npair.type = INTEGER;
                }
                else if (state == 8)
                {
                    //npair.identifier = (tState == OPER ? "OPERATOR" : "SEPARATOR");

                    if (tState == OPER)
                    {
                        npair.identifier = "OPERATOR";
                        npair.type = OPERATOR;
                    }
                    else
                    {
                        npair.identifier = "SEPARATOR";
                        npair.type = SEPARATOR;
                    }
                }
                else if (state == 9)
                {
                    npair.identifier = "REAL";
                    npair.type = REAL;
                }

                if (npair.lexeme != "" && npair.identifier != "")
                    tokens.push_back(npair);
            }
            //revert back to the initial state and update pplace
            state = 0;
            pplace = curr;
        }
    }
    return 0;
}

std::unordered_map<char, tokenState> LexicalAnalyzer::createStateMap()
{
    std::unordered_map<char, tokenState> stateMap;

    stateMap['$'] = DOLLAR;
    stateMap['\''] = APOST;
    stateMap['('] = LEFTPAR;
    stateMap[')'] = RIGHTPAR;
    stateMap['{'] = LEFTCURL;
    stateMap['}'] = RIGHTCURL;
    stateMap['['] = LEFTSQR;
    stateMap[']'] = RIGHTSQR;
    stateMap[','] = COMMA;
    stateMap[':'] = COLON;
    stateMap[';'] = SEMICOLON;
    stateMap['\t'] = SPACE;
    stateMap['\n'] = SPACE;
    stateMap[' '] = SPACE;
    stateMap['\r'] = SPACE;
    stateMap['.'] = PERIOD;
    stateMap['!'] = EXCLA;
    stateMap['+'] = OPER;
    stateMap['-'] = OPER;
    stateMap['/'] = OPER;
    stateMap['*'] = OPER;
    stateMap['='] = OPER;
    stateMap['%'] = OPER;
    stateMap['>'] = OPER;
    stateMap['<'] = OPER;

    return stateMap;
}

std::unordered_map<std::string, bool> LexicalAnalyzer::createKeySet()
{
    std::unordered_map<std::string, bool> keySet;

    keySet["int"] = true;
    keySet["float"] = true;
    keySet["bool"] = true;
    keySet["true"] = true;
    keySet["false"] = true;
    keySet["if"] = true;
    keySet["else"] = true;
    keySet["then"] = true;
    keySet["endif"] = true;
    keySet["while"] = true;
    keySet["whileend"] = true;
    keySet["do"] = true;
    keySet["doend"] = true;
    keySet["for"] = true;
    keySet["forend"] = true;
    keySet["input"] = true;
    keySet["output"] = true;
    keySet["and"] = true;
    keySet["or"] = true;
    keySet["not"] = true;

    return keySet;
}

void LexicalAnalyzer::printLexerTable()
{
    std::ofstream outfile;
    outfile.open("output.txt");
    outfile << std::left << std::setw(15) << "TOKENS"
            << "|" << std::right << std::setw(15) << "Lexemes"
            << "\n";
    outfile << std::setfill('=') << std::setw(31) << "" << std::setfill(' ') << std::endl;
    for (auto token : tokens)
        outfile << std::left << std::setw(15) << token.identifier << "=" << std::right << std::setw(15) << token.lexeme << std::endl;
    outfile.close();

    std::cout << std::left << std::setw(15) << "TOKENS"
              << "|" << std::right << std::setw(15) << "Lexemes"
              << "\n";
    std::cout << std::setfill('=') << std::setw(31) << "" << std::setfill(' ') << std::endl;
    for (auto token : tokens)
        std::cout << std::left << std::setw(15) << token.identifier << "=" << std::right << std::setw(15) << token.lexeme << std::endl;
}

std::vector<LexemePair> LexicalAnalyzer::get_tokens()
{
  return this->tokens;
}