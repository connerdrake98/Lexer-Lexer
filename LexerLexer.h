#include<vector>

using namespace std;

class LexerLexer
{
private:
    string orSymbol = "0";
    string nonTerminalExpansionSymbol = "0";
    string terminalIDAssignmentSymbol = "0";

public:
    LexerLexer();
    LexerLexer(const string OR_SYMBOL_IN, const string NON_TERMINAL_EXPANSION_SYMBOL_IN, const string TERMINAL_ID_ASSIGNMENT_SYMBOL_IN);
    void clearLexLanguageSpecificationsMemory(vector<string> *undefinedNonTerminals, vector<string> *undefinedTerminals, vector<string> *definedNonTerminals, vector<string> *definedTerminals);
    int lexLanguageSpecifications();
    ~LexerLexer();
};