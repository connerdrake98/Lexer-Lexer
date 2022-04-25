#include <fstream>

#include "LexerLexer.h"
#include "main.h"

LexerLexer::LexerLexer()
{
    orSymbol = "0";
    placeholderExpansionSymbol = "0";
    tokenIDAssignmentSymbol = "0";
}

LexerLexer::LexerLexer(const string OR_SYMBOL_IN, const string PLACEHOLDER_EXPANSION_SYMBOL_IN, const string TOKEN_ID_ASSIGNMENT_SYMBOL_IN)
{
    orSymbol = OR_SYMBOL_IN;
    placeholderExpansionSymbol = PLACEHOLDER_EXPANSION_SYMBOL_IN;
    tokenIDAssignmentSymbol = TOKEN_ID_ASSIGNMENT_SYMBOL_IN;
}

LexerLexer::~LexerLexer()
{
}

void LexerLexer::clearLexLanguageSpecificationsMemory(vector<string> *undefinedPlaceholders, vector<string> *undefinedTokens, vector<string> *definedPlaceholders, vector<string> *definedTokens)
{
    delete undefinedPlaceholders;
    undefinedPlaceholders = NULL;

    delete undefinedTokens;
    undefinedTokens = NULL;

    delete definedPlaceholders;
    definedPlaceholders = NULL;

    delete definedTokens;
    definedTokens = NULL;
}

int LexerLexer::lexLanguageSpecifications()
{

    // define strategy scripting rules based on LanguageRules.txt
    vector<string> *undefinedPlaceholders = new vector<string>;
    vector<string> *undefinedTokens = new vector<string>;
    vector<string> *definedPlaceholders = new vector<string>;
    vector<string> *definedTokens = new vector<string>;
    stringstream getWord;
    string currentWord;
    string currentLine;
    string currentFirstWord;
    bool currentFirstWordNeedsToBeDefined;
    int nonWhitespaceLineIndex = 0;
    int wordIndex = 0;
    string prevWord;
    fstream fs;

    fs.open("LanguageSpecifications.txt");
    if (fs.fail())
    {
        cout << "Error: LanguageSpecifications.txt file not found..." << endl;

        clearLexLanguageSpecificationsMemory(undefinedPlaceholders, undefinedTokens, definedPlaceholders, definedTokens);
        return -1;
    }

    

    // parse placeholder specifications
    while (getline(fs, currentLine) && (isPlaceholder(currentLine.substr(0, getIndexEndFirstWord(currentLine))) || containsNothing(currentLine)))
    {
        testPrintVector(undefinedPlaceholders, "undefined placeholders");
        currentFirstWordNeedsToBeDefined = false;
        getWord.clear();

        cout << "Current line: " << currentLine << endl;

        if (!containsNothing(currentLine))
        {
            cout << currentLine << endl;
            nonWhitespaceLineIndex++;
            wordIndex = 0;
            getWord << currentLine;

            while (getWord >> currentWord)
            {
                if (wordIndex == 0)
                {
                    if (nonWhitespaceLineIndex != 1)
                    {
                        // Determine if this placeholder is on the undefined placeholders list
                        for (int i = 0; i < undefinedPlaceholders->size(); i++)
                        {
                            if (undefinedPlaceholders->at(i) == currentWord)
                            {
                                currentFirstWordNeedsToBeDefined = true;
                            }
                        }
                        if (!currentFirstWordNeedsToBeDefined)
                        {
                            cout << "Error: Current placeholder, " << currentWord << ", has no prior use and does not need to be defined." << endl;

                            clearLexLanguageSpecificationsMemory(undefinedPlaceholders, undefinedTokens, definedPlaceholders, definedTokens);
                            return -2;
                        }
                    }

                    currentFirstWord = currentWord;
                }
                if (wordIndex == 1)
                {
                    if (currentWord != placeholderExpansionSymbol)
                    {
                        cout << "Error: Language Specifications must contain a placeholder expansion symbol " << placeholderExpansionSymbol << " after the initial token placeholder in a line." << endl;

                        clearLexLanguageSpecificationsMemory(undefinedPlaceholders, undefinedTokens, definedPlaceholders, definedTokens);
                        return -3;
                    }
                }
                else if (wordIndex > 1)
                {
                    if (isPlaceholder(currentWord))
                    {
                        addIfDoesNotContain(undefinedPlaceholders, currentWord);
                    }
                    else if (isToken(currentWord))
                    {
                        addIfDoesNotContain(undefinedTokens, currentWord);
                    }
                    else
                    {
                        if (currentWord == orSymbol)
                        {
                            if (wordIndex == 2)
                            {
                                cout << "Error: " << orSymbol << " can only be used in a token or token placeholder formula after a token or token placeholder." << endl;

                                clearLexLanguageSpecificationsMemory(undefinedPlaceholders, undefinedTokens, definedPlaceholders, definedTokens);
                                return -4;
                            }
                            if (prevWord == orSymbol)
                            {
                                cout << "Error: a token or token placeholder must come after the " << orSymbol << " symbol." << endl;

                                clearLexLanguageSpecificationsMemory(undefinedPlaceholders, undefinedTokens, definedPlaceholders, definedTokens);
                                return -5;
                            }
                        }
                        else
                        {
                            cout << "Error: " << currentWord << " is not a token or token placeholder." << endl;

                            clearLexLanguageSpecificationsMemory(undefinedPlaceholders, undefinedTokens, definedPlaceholders, definedTokens);
                            return -6;
                        }
                    }
                }
                cout << "	word: " << currentWord << endl;

                wordIndex++;
                currentLine = currentLine.substr(getIndexEndFirstWord(currentLine), currentLine.length() - getIndexEndFirstWord(currentLine));
                prevWord = currentWord;
                currentWord = "";
            }

            // erase placeholder that was just defined from undefined placeholders list
            for (int i = 0; i < undefinedPlaceholders->size(); i++)
            {
                if (undefinedPlaceholders->at(i) == currentFirstWord)
                {
                    undefinedPlaceholders->erase(undefinedPlaceholders->begin() + i);
                }
            }

            // add placeholder that was just defined to defined placeholders list
            if (!addIfDoesNotContain(definedPlaceholders, currentFirstWord))
            {
                cout << "Error: Defined placeholder " << currentFirstWord << ", which was previously defined." << endl;

                clearLexLanguageSpecificationsMemory(undefinedPlaceholders, undefinedTokens, definedPlaceholders, definedTokens);
                return -7;
            }

            currentFirstWord = "";
        }
    }

    if (nonWhitespaceLineIndex == 0)
    {
        // first word in Language Specifications was not a token placeholder
        cout << "Error: Language Specifications must contain a token placeholder as a first word." << endl;

        clearLexLanguageSpecificationsMemory(undefinedPlaceholders, undefinedTokens, definedPlaceholders, definedTokens);
        return -8;
    }

    // parse token value specifications
    do
    {
        currentFirstWordNeedsToBeDefined = false;
        getWord.clear();

        cout << "Current line: " << currentLine << endl;

        if (!containsNothing(currentLine))
        {
            cout << currentLine << endl;
            nonWhitespaceLineIndex++;
            wordIndex = 0;
            getWord << currentLine;

            while (getWord >> currentWord)
            {
                if (wordIndex == 0)
                {
                    // Determine if this placeholder is on the undefined placeholders list
                    for (int i = 0; i < undefinedTokens->size(); i++)
                    {
                        if (undefinedTokens->at(i) == currentWord)
                        {
                            currentFirstWordNeedsToBeDefined = true;
                        }
                    }
                    if (!currentFirstWordNeedsToBeDefined)
                    {
                        cout << "Error: Current token, " << currentWord << ", has no prior use and does not need to be defined." << endl;

                        clearLexLanguageSpecificationsMemory(undefinedPlaceholders, undefinedTokens, definedPlaceholders, definedTokens);
                        return -9;
                    }

                    currentFirstWord = currentWord;
                }
                if (wordIndex == 1)
                {
                    if (currentWord != tokenIDAssignmentSymbol)
                    {
                        cout << "Error: Language Specifications must use an equals sign (=) to assign a token ID." << endl;

                        clearLexLanguageSpecificationsMemory(undefinedPlaceholders, undefinedTokens, definedPlaceholders, definedTokens);
                        return -10;
                    }
                }
                else if (wordIndex == 2)
                {
                    if (currentWord.at(0) != '\"' || currentWord.at(currentWord.length() - 1) != '\"')
                    {
                        cout << "Error: Strings representing tokens must be listed in double quotes (\")." << endl;

                        clearLexLanguageSpecificationsMemory(undefinedPlaceholders, undefinedTokens, definedPlaceholders, definedTokens);
                        return -11;
                    }
                }
                else if (wordIndex == 3)
                {
                    cout << "Error: Token Definitions should take the format of [TokenID] = \"String Representing Token\"." << endl;

                    clearLexLanguageSpecificationsMemory(undefinedPlaceholders, undefinedTokens, definedPlaceholders, definedTokens);
                    return -11;
                    // TODO: Need to THROW errors and make sure memory is cleared in the case of errors.
                }
                cout << "	word: " << currentWord << endl;

                wordIndex++;
                currentLine = currentLine.substr(getIndexEndFirstWord(currentLine), currentLine.length() - getIndexEndFirstWord(currentLine));
                currentWord = "";
            }

            // erase token that was just defined from undefined tokens list
            for (int i = 0; i < undefinedTokens->size(); i++)
            {
                if (undefinedTokens->at(i) == currentFirstWord)
                {
                    undefinedTokens->erase(undefinedTokens->begin() + i);
                }
            }

            // add token that was just defined to defined tokens list
            if (!addIfDoesNotContain(definedTokens, currentFirstWord))
            {
                cout << "Error: Defined placeholder " << currentFirstWord << ", which was previously defined." << endl;

                clearLexLanguageSpecificationsMemory(undefinedPlaceholders, undefinedTokens, definedPlaceholders, definedTokens);
                return -12;
            }

            currentFirstWord = "";
        }
    } while (getline(fs, currentLine) && (isToken(currentLine.substr(0, getIndexEndFirstWord(currentLine))) || containsNothing(currentLine)));

    if (undefinedPlaceholders->size() > 0)
    {
        cout << "Error: Lexer Got Last Token Definition and Found Undefined Token Placeholders: " << endl;

        for (int i = 0; i < undefinedPlaceholders->size(); i++)
        {
            cout << "	" << undefinedPlaceholders->at(i) << endl;
        }

        clearLexLanguageSpecificationsMemory(undefinedPlaceholders, undefinedTokens, definedPlaceholders, definedTokens);
        return -13;
    }
    if (undefinedTokens->size() > 0)
    {
        cout << "Error: Undefined Tokens: " << endl;

        for (int i = 0; i < undefinedTokens->size(); i++)
        {
            cout << "	" << undefinedTokens->at(i) << endl;
        }

        clearLexLanguageSpecificationsMemory(undefinedPlaceholders, undefinedTokens, definedPlaceholders, definedTokens);
        return -14;
    }

    testPrintVector(definedPlaceholders, "defined placeholders");
    testPrintVector(definedTokens, "defined tokens");

    clearLexLanguageSpecificationsMemory(undefinedPlaceholders, undefinedTokens, definedPlaceholders, definedTokens);
    
    return 0;
}