#include <fstream>

#include "LexerLexer.h"
#include "main.h"

LexerLexer::LexerLexer()
{
    orSymbol = "0";
    nonTerminalExpansionSymbol = "0";
    terminalValueAssignmentSymbol = "0";
}

LexerLexer::LexerLexer(const string OR_SYMBOL_IN, const string NON_TERMINAL_EXPANSION_SYMBOL_IN, const string TERMINAL_VALUE_ASSIGNMENT_SYMBOL_IN)
{
    orSymbol = OR_SYMBOL_IN;
    nonTerminalExpansionSymbol = NON_TERMINAL_EXPANSION_SYMBOL_IN;
    terminalValueAssignmentSymbol = TERMINAL_VALUE_ASSIGNMENT_SYMBOL_IN;
}

LexerLexer::~LexerLexer()
{
}

void LexerLexer::clearLexLanguageSpecificationsMemory(vector<string> *undefinedNonTerminals, vector<string> *undefinedTerminals, vector<string> *definedNonTerminals, vector<string> *definedTerminals)
{
    delete undefinedNonTerminals;
    undefinedNonTerminals = NULL;

    delete undefinedTerminals;
    undefinedTerminals = NULL;

    delete definedNonTerminals;
    definedNonTerminals = NULL;

    delete definedTerminals;
    definedTerminals = NULL;
}

int LexerLexer::lexLanguageSpecifications()
{

    // define strategy scripting rules based on LanguageRules.txt
    vector<string> *undefinedNonTerminals = new vector<string>;
    vector<string> *undefinedTerminals = new vector<string>;
    vector<string> *definedNonTerminals = new vector<string>;
    vector<string> *definedTerminals = new vector<string>;
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

        clearLexLanguageSpecificationsMemory(undefinedNonTerminals, undefinedTerminals, definedNonTerminals, definedTerminals);
        return -1;
    }

    

    // parse non-terminal specifications
    while (getline(fs, currentLine) && (isNonTerminal(currentLine.substr(0, getIndexEndFirstWord(currentLine))) || containsNothing(currentLine)))
    {
        testPrintVector(undefinedNonTerminals, "undefined non-terminals");
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
                        // Determine if this non-terminal is on the undefined non-terminals list
                        for (int i = 0; i < undefinedNonTerminals->size(); i++)
                        {
                            if (undefinedNonTerminals->at(i) == currentWord)
                            {
                                currentFirstWordNeedsToBeDefined = true;
                            }
                        }
                        if (!currentFirstWordNeedsToBeDefined)
                        {
                            cout << "Error: Current non-terminal, " << currentWord << ", has no prior use and does not need to be defined." << endl;

                            clearLexLanguageSpecificationsMemory(undefinedNonTerminals, undefinedTerminals, definedNonTerminals, definedTerminals);
                            return -2;
                        }
                    }

                    currentFirstWord = currentWord;
                }
                if (wordIndex == 1)
                {
                    if (currentWord != nonTerminalExpansionSymbol)
                    {
                        cout << "Error: Language Specifications must contain a non-terminal expansion symbol " << nonTerminalExpansionSymbol << " after the initial non-terminal in a line." << endl;

                        clearLexLanguageSpecificationsMemory(undefinedNonTerminals, undefinedTerminals, definedNonTerminals, definedTerminals);
                        return -3;
                    }
                }
                else if (wordIndex > 1)
                {
                    if (isNonTerminal(currentWord))
                    {
                        addIfDoesNotContain(undefinedNonTerminals, currentWord);
                    }
                    else if (isTerminal(currentWord))
                    {
                        addIfDoesNotContain(undefinedTerminals, currentWord);
                    }
                    else
                    {
                        if (currentWord == orSymbol)
                        {
                            if (wordIndex == 2)
                            {
                                cout << "Error: " << orSymbol << " can only be used in a terminal or non-terminal production rule after a terminal or non-terminal." << endl;

                                clearLexLanguageSpecificationsMemory(undefinedNonTerminals, undefinedTerminals, definedNonTerminals, definedTerminals);
                                return -4;
                            }
                            if (prevWord == orSymbol)
                            {
                                cout << "Error: A terminal or non-terminal must come after the " << orSymbol << " symbol." << endl;

                                clearLexLanguageSpecificationsMemory(undefinedNonTerminals, undefinedTerminals, definedNonTerminals, definedTerminals);
                                return -5;
                            }
                        }
                        else
                        {
                            cout << "Error: " << currentWord << " is not a terminal or non-terminal." << endl;

                            clearLexLanguageSpecificationsMemory(undefinedNonTerminals, undefinedTerminals, definedNonTerminals, definedTerminals);
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

            // erase non-terminal that was just defined from undefined non-terminals list
            for (int i = 0; i < undefinedNonTerminals->size(); i++)
            {
                if (undefinedNonTerminals->at(i) == currentFirstWord)
                {
                    undefinedNonTerminals->erase(undefinedNonTerminals->begin() + i);
                }
            }

            // add non-terminal that was just defined to defined non-terminal list
            if (!addIfDoesNotContain(definedNonTerminals, currentFirstWord))
            {
                cout << "Error: non-terminal " << currentFirstWord << "was already defined." << endl;

                clearLexLanguageSpecificationsMemory(undefinedNonTerminals, undefinedTerminals, definedNonTerminals, definedTerminals);
                return -7;
            }

            currentFirstWord = "";
        }
    }

    if (nonWhitespaceLineIndex == 0)
    {
        // first word in Language Specifications was not a non-terminal
        cout << "Error: Language Specifications must contain a non-terminal as a first word." << endl;

        clearLexLanguageSpecificationsMemory(undefinedNonTerminals, undefinedTerminals, definedNonTerminals, definedTerminals);
        return -8;
    }

    // parse terminal value specifications
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
                    // Determine if this terminal is on the undefined terminals list
                    for (int i = 0; i < undefinedTerminals->size(); i++)
                    {
                        if (undefinedTerminals->at(i) == currentWord)
                        {
                            currentFirstWordNeedsToBeDefined = true;
                        }
                    }
                    if (!currentFirstWordNeedsToBeDefined)
                    {
                        cout << "Error: Current terminal, " << currentWord << ", has no prior use and does not need to be defined." << endl;

                        clearLexLanguageSpecificationsMemory(undefinedNonTerminals, undefinedTerminals, definedNonTerminals, definedTerminals);
                        return -9;
                    }

                    currentFirstWord = currentWord;
                }
                if (wordIndex == 1)
                {
                    if (currentWord != terminalValueAssignmentSymbol)
                    {
                        cout << "Error: Language Specifications must use a " << terminalValueAssignmentSymbol << " to assign a value to a terminal." << endl;

                        clearLexLanguageSpecificationsMemory(undefinedNonTerminals, undefinedTerminals, definedNonTerminals, definedTerminals);
                        return -10;
                    }
                }
                else if (wordIndex == 2)
                {
                    if (currentWord.at(0) != '\"' || currentWord.at(currentWord.length() - 1) != '\"')
                    {
                        cout << "Error: values assigned to terminals must be listed in double quotes (\")." << endl;

                        clearLexLanguageSpecificationsMemory(undefinedNonTerminals, undefinedTerminals, definedNonTerminals, definedTerminals);
                        return -11;
                    }
                }
                else if (wordIndex == 3)
                {
                    cout << "Error: Terminal definitions should take the format of [Terminal_ID] = \"Value\"." << endl;

                    clearLexLanguageSpecificationsMemory(undefinedNonTerminals, undefinedTerminals, definedNonTerminals, definedTerminals);
                    return -11;
                    // TODO: Need to THROW errors and make sure memory is cleared in the case of errors.
                }
                cout << "	word: " << currentWord << endl;

                wordIndex++;
                currentLine = currentLine.substr(getIndexEndFirstWord(currentLine), currentLine.length() - getIndexEndFirstWord(currentLine));
                currentWord = "";
            }

            // erase terminal that was just defined from undefined terminals list
            for (int i = 0; i < undefinedTerminals->size(); i++)
            {
                if (undefinedTerminals->at(i) == currentFirstWord)
                {
                    undefinedTerminals->erase(undefinedTerminals->begin() + i);
                }
            }

            // add terminal that was just defined to defined terminal list
            if (!addIfDoesNotContain(definedTerminals, currentFirstWord))
            {
                cout << "Error: Defined non-terminal " << currentFirstWord << ", which was previously defined." << endl;

                clearLexLanguageSpecificationsMemory(undefinedNonTerminals, undefinedTerminals, definedNonTerminals, definedTerminals);
                return -12;
            }

            currentFirstWord = "";
        }
    } while (getline(fs, currentLine) && (isTerminal(currentLine.substr(0, getIndexEndFirstWord(currentLine))) || containsNothing(currentLine)));

    if (undefinedNonTerminals->size() > 0)
    {
        cout << "Error: Finished lexing language specifications after non-terminal definitions and gound undefined non-terminals: " << endl;

        for (int i = 0; i < undefinedNonTerminals->size(); i++)
        {
            cout << "	" << undefinedNonTerminals->at(i) << endl;
        }

        clearLexLanguageSpecificationsMemory(undefinedNonTerminals, undefinedTerminals, definedNonTerminals, definedTerminals);
        return -13;
    }
    if (undefinedTerminals->size() > 0)
    {
        cout << "Error: Undefined terminals: " << endl;

        for (int i = 0; i < undefinedTerminals->size(); i++)
        {
            cout << "	" << undefinedTerminals->at(i) << endl;
        }

        clearLexLanguageSpecificationsMemory(undefinedNonTerminals, undefinedTerminals, definedNonTerminals, definedTerminals);
        return -14;
    }

    testPrintVector(definedNonTerminals, "defined non-terminals");
    testPrintVector(definedTerminals, "defined terminals");

    clearLexLanguageSpecificationsMemory(undefinedNonTerminals, undefinedTerminals, definedNonTerminals, definedTerminals);
    
    return 0;
}