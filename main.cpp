#include <iostream>

#include "LexerLexer.h"

using namespace std;

int main()
{
	// Language Specifications Compilation Settings
	const string OR_SYMBOL = "||";
	const string NON_TERMINAL_EXPANSION_SYMBOL = "->";
	const string TERMINAL_ID_ASSIGNMENT_SYMBOL = "=";

	// Create Lexer Lexer
	LexerLexer languageSpecificationsLexer(OR_SYMBOL, NON_TERMINAL_EXPANSION_SYMBOL, TERMINAL_ID_ASSIGNMENT_SYMBOL);

	// Lex Language Specifications
	if (!languageSpecificationsLexer.lexLanguageSpecifications())
	{
		cout << "Exiting Program." << endl;
		return 0;
	}

	// Create the Lexer and Lex the code using the Rules

	return 0;
}
