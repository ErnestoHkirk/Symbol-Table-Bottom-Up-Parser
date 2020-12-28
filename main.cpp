/*
 * PROJECT: Lexical Analyzer
 * 
 * Group Memebers:
 * Jacob Rapmund
 * Ernesto Hooghkirk
 * Andy Duong
 * Michael Lau
 * 
 * Professor: Anthony Le
 * CPSC 323-03
 */

#include "FileReader.hpp"
#include "LexicalAnalyzer.hpp"
#include "SyntaxAnalyzer.hpp"
#include "Memory.hpp"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <iomanip>

using namespace std;

int eye = 0;
vector<string> syntaxOutput;
ofstream outfile2;

int memoryCounter = 5000;

//Syntax analyzer prototypes
bool analyzeSyntax(std::vector<LexemePair> &tokens); // function to start syntax analysis
// each function is for each rule
bool A(const std::vector<LexemePair> &tokens);
bool APrime(const std::vector<LexemePair> &tokens);
bool B(const std::vector<LexemePair> &tokens);
bool BPrime(const std::vector<LexemePair> &tokens);
bool C(const std::vector<LexemePair> &tokens);
bool D(const std::vector<LexemePair> &tokens);

void printSyntaxOutput();

void printAllSyntax();

int main()
{
    LexicalAnalyzer la1("states.txt"), la2("states.txt");

    outfile2.open("outputPrdp.txt");
    
    cout << "==========================================================" << endl;
    cout << "Start of Lexical Analysis" << endl;
    cout << "==========================================================" << endl << endl;


    //extract source code from file
    std::string fileData = FileReader::readFile("testfile.txt");
    std::string fileData2 = FileReader::readFile("testfile2.txt");

    //Perform lexical analysis
    if (!la1.lexer(fileData))
        la1.printLexerTable();
    else
        std::cout << "ERROR: Lexer could not analyze input file.";

    std::cout << "\nLexeme Set 2\n"
              << std::endl;
    if (!la2.lexer(fileData2))
        la2.printLexerTable();
    else
        std::cout << "ERROR: Lexer could not analyze input file.";

    cout << endl;
    cout << "==========================================================" << endl;
    cout << "End of Lexical Analysis" << endl;
    cout << "==========================================================" << endl << endl;

    // SYNTAX ANALYSIS
    cout << "==========================================================" << endl; 
    std::cout << "[Bottom-Up LR Syntax analysis.]" << std::endl;
    cout << "==========================================================" << endl << endl;

    SyntaxAnalyzer sa;
    sa.setPrintAnalysis(true);
    std::cout << (sa.analyze(la2.get_tokens()) ? "[input accepted.]\n" : "[input rejected.]\n") << std::endl;

    cout << "==========================================================" << endl;
    std::cout << "[End Syntax analysis (bottom-up).]" << std::endl;
    cout << "==========================================================" << endl << endl;

    sa.logToOutputFile();

    cout << "==========================================================" << endl;
    cout << "Start of Syntax Analysis (PRDP)." << endl;
    cout << "==========================================================" << endl << endl;

    std::vector<LexemePair> Tokens = la1.get_tokens();

    if (analyzeSyntax(Tokens))
    {
        std::cout << "******** Accepted ********" << std::endl;
    }
    else
    {
        std::cout << "******** Not accepted ********" << std::endl;
        std::cout << "Invalid token and lexeme: \n";
        std::cout << "Identifier ====================== Lexeme" << endl;
        std::cout << Tokens[eye - 1].identifier << " | " << Tokens[eye - 1].lexeme << endl;
    }
    cout << endl;
    cout << "==========================================================" << endl;
    cout << "End of Syntax Analysis (PRDP)." << endl;
    cout << "==========================================================" << endl << endl;
    outfile2.close();


    SymbolTable st;
    std::vector<LexemePair> tokens = la2.get_tokens();

    //generate symbol table
    for (int i = 0; i < tokens.size(); i++) { 
      if (tokens[i].lexeme == "int" || tokens[i].lexeme == "bool" || tokens[i].lexeme == "boolean" ||
         tokens[i].lexeme == "string" || tokens[i].lexeme == "float") {
        if(!st.exists_in_table(tokens[i].lexeme)){
          st.add_to_table(memoryCounter, tokens[i+1].lexeme, tokens[i].lexeme);
          memoryCounter++;
        }
      }
    }
    st.print_table();
    
    //uncomment if console closes
    //system("pause");
    
    return 0;
}

/********************************************************
				CODE FOR PRDP
*********************************************************/
bool analyzeSyntax(std::vector<LexemePair> &tokens)
{
    LexemePair end;
    end.lexeme = "$";
    end.identifier = "a";

    tokens.push_back(end);
    if (A(tokens))
    {
        //std::cout << "Working!" << std::endl;
        return true;
    }
    // if assignment
    // if declarative
    return false;
}

// <Expression> -> <Term><Expression Prime>
bool A(const std::vector<LexemePair> &tokens)
{
    syntaxOutput.push_back("A -> BA'");
    if (B(tokens))
    {
        if (APrime(tokens))
        {
            return true;
        }
    }
    return false;
}

// <Expression Prime> -> +<Term><Expression Prime> || <Expression Prime> -> -<Term><Expression Prime>
bool APrime(const std::vector<LexemePair> &tokens)
{
    if (tokens[eye].lexeme == "+" || tokens[eye].lexeme == "-")
    {
        cout << endl;
        std::cout << "Token: " << tokens[eye].identifier << "  Lexeme: " << tokens[eye].lexeme << std::endl;
        outfile2 << "\n"
                 << "Token: " << tokens[eye].identifier << "  Lexeme: " << tokens[eye].lexeme << "\n";
        ++eye;
        syntaxOutput.push_back("A' -> +BA | A' -> -BA");
        printSyntaxOutput();
        if (B(tokens))
        {
            if (APrime(tokens))
            {
                printSyntaxOutput();
                return true;
            }
        }
    }
    else if (tokens[eye].lexeme == "$" || tokens[eye].lexeme == ")")
    {
        // follow set of A'
        syntaxOutput.push_back("A' -> epsilon");
        return true;
    }
    return false;
}

// <Term> -> <Factor><Term Prime>
bool B(const std::vector<LexemePair> &tokens)
{
    syntaxOutput.push_back("B -> CB'");
    if (C(tokens))
    {
        if (BPrime(tokens))
        {
            return true;
        }
    }
    return false;
}

// <Term Prime> -> *<Factor><Term Prime> || <Term Prime> -> /<Factor><Term Prime>
bool BPrime(const std::vector<LexemePair> &tokens)
{
    if (tokens[eye].lexeme == "*" || tokens[eye].lexeme == "/")
    {
        cout << endl;
        std::cout << "Token: " << tokens[eye].identifier << "  Lexeme: " << tokens[eye].lexeme << std::endl;
        outfile2 << "\n"
                 << "Token: " << tokens[eye].identifier << "  Lexeme: " << tokens[eye].lexeme << "\n";
        ++eye;
        syntaxOutput.push_back("B' -> *CB' | /CB'");

        printSyntaxOutput();
        if (C(tokens))
        {
            if (BPrime(tokens))
            {
                return true;
            }
        }
    }
    else if (tokens[eye].lexeme == "+" || tokens[eye].lexeme == "-" || tokens[eye].lexeme == "$" || tokens[eye].lexeme == ")")
    {
        syntaxOutput.push_back("B' -> epsilon");
        printSyntaxOutput();
        return true;
    }
    return false;
}

// <Factor> -> (Expression) || <Factor> -> <ID>
bool C(const std::vector<LexemePair> &tokens)
{
    if (tokens[eye].lexeme == "(")
    {
        std::cout << "Token: " << tokens[eye].identifier << "  Lexeme: " << tokens[eye].lexeme << endl;
        outfile2 << "\n"
                 << "Token: " << tokens[eye].identifier << "  Lexeme: " << tokens[eye].lexeme << "\n";
        syntaxOutput.push_back("C -> (");
        printSyntaxOutput();
        eye++;
        if (A(tokens))
        {
            if (tokens[eye].lexeme == ")")
            {
                std::cout << std::endl
                          << "Token: " << tokens[eye].identifier << "  Lexeme: " << tokens[eye].lexeme << endl;
                outfile2 << "\n"
                         << "Token: " << tokens[eye].identifier << "  Lexeme: " << tokens[eye].lexeme << "\n";
                syntaxOutput.push_back("C -> )");
                printSyntaxOutput();
                eye++;
                return true;
            }
        }
    }
    if (tokens[eye].identifier == "IDENTIFIER")
    {
        syntaxOutput.push_back("C -> D");
        if (D(tokens))
        {
            return true;
        }
    }
    return false;
}

// <ID> -> id
bool D(const std::vector<LexemePair> &tokens)
{
    syntaxOutput.push_back("D -> id");
    if (tokens[eye].identifier == "IDENTIFIER")
    {
        cout << endl;
        std::cout << "Token: " << tokens[eye].identifier << "  Lexeme: " << tokens[eye].lexeme << endl;
        outfile2 << "\n"
                 << "Token: " << tokens[eye].identifier << "  Lexeme: " << tokens[eye].lexeme << "\n";
        ++eye;
        printSyntaxOutput();
        return true;
    }
    return false;
}

void printSyntaxOutput()
{
    for (int i = 0; i < syntaxOutput.size(); i++)
    {
        std::cout << syntaxOutput[i] << std::endl;
    }
    printAllSyntax();
    syntaxOutput.clear();
}

void printAllSyntax()
{
    for (int i = 0; i < syntaxOutput.size(); i++)
    {
        outfile2 << syntaxOutput[i];
        outfile2 << "\n";
    }
}