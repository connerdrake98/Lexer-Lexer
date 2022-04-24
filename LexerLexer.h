#include<vector>

using namespace std;

class LexerLexer
{
private:
    string orSymbol = "0";
    string placeholderExpansionSymbol = "0";
    string tokenIDAssignmentSymbol = "0";

public:
    LexerLexer();
    LexerLexer(const string OR_SYMBOL_IN, const string PLACEHOLDER_EXPANSION_SYMBOL_IN, const string TOKEN_ID_ASSIGNMENT_SYMBOL_IN);
    void clearLexLanguageSpecificationsMemory(vector<string> *undefinedPlaceholders, vector<string> *undefinedTokens, vector<string> *definedPlaceholders, vector<string> *definedTokens);
    int lexLanguageSpecifications();
    ~LexerLexer();
};