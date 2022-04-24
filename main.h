#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

string getSubstringFromClockTime(string timeCounter, int indexFirstDigit, const int CURRENT_DATA_LENGTH)
{
    return timeCounter.substr(indexFirstDigit, CURRENT_DATA_LENGTH);
}

double stringToDouble(string inputString)
{
    stringstream stringToDouble;
    double returnNum;
    stringToDouble << inputString;
    stringToDouble >> returnNum;
    return returnNum;
}

bool isPlaceholder(string input)
{
    bool containsLowercaseLetter = false;
    for (int i = 0; i < input.length(); i++)
    {
        if (!isalpha(input.at(i)))
        {
            if (input.at(i) != '_')
            {
                return false;
            }
        }
        if (islower(input.at(i)))
        {
            containsLowercaseLetter = true;
        }
    }
    return containsLowercaseLetter;
}

bool isToken(string input)
{
    for (int i = 0; i < input.length(); i++)
    {
        if (!isalpha(input.at(i)))
        {
            if (input.at(i) != '_')
            {
                return false;
            }
        }
        if (islower(input.at(i)))
        {
            return false;
        }
    }
    return true;
}

int getIndexEndFirstWord(string lineIn)
{
    for (int i = 0; i < lineIn.length(); i++)
    {
        if (lineIn.at(i) == ' ' || lineIn.at(i) == '-')
        {
            return i;
        }
    }
    return -1;
}

bool containsNothing(string inputString)
{
    for (int i = 0; i < inputString.length(); i++)
    {
        if (inputString.at(i) != ' ')
        {
            return false;
        }
    }
    return true;
}

bool addIfDoesNotContain(vector<string> *vectorToCheck, string stringIn)
{
    bool containsString = false;
    for (int i = 0; i < vectorToCheck->size(); i++)
    {
        if (vectorToCheck->at(i) == stringIn)
        {
            containsString = true;
        }
    }
    if (!containsString)
    {
        vectorToCheck->push_back(stringIn);
    }

    // returns true if we added the element, meaning it wasn't already in the vector
    // returns false if we were not able to add the element, meanin it was already in the vector
    return !containsString;
}

void testPrintVector(vector<string> *vectorToPrint, string vectorName)
{
    cout << "       " << vectorName << ": " << endl;
    for (int i = 0; i < vectorToPrint->size(); i++)
    {
        cout << "			element " << i << ": " << vectorToPrint->at(i) << endl;
    }
}