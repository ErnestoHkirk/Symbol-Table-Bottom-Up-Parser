#ifndef LEXEMEPAIR_HPP
#define LEXEMEPAIR_HPP

#include <string>

enum lexemeType { KEYWORD, IDENTIFIER, INTEGER, OPERATOR, SEPARATOR, REAL };

struct LexemePair
{
	std::string lexeme;
	std::string identifier;
	lexemeType type;
};

#endif