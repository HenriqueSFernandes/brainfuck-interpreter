// Brainfuck to c++ source-to-source translator
// Copyright (C) 2023  Henrique Fernandes

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// For any further questions or inquiries, please contact henriquesardofernandes@gmail.com

#include <algorithm>
#include <vector>
#include <iostream>
#include <stack>

using namespace std;

string removeUnwantedChars(const string &source_code)
{
    vector<char> valid_chars = {'[', ']', '<', '>', '+', '-', '.', ','};
    string result;

    for (char c : source_code)
    {
        if (find(valid_chars.begin(), valid_chars.end(), c) != valid_chars.end())
        {
            result += c; // If c is a valid char, add it to the result
        }
    }
    return result;
}

bool hasUnmatchedBrackets(const string &source_code)
{
    stack<char> bracketStack;

    for (char c : source_code)
    {
        if (c == '[')
        {
            bracketStack.push(c);
        }
        else if (c == ']')
        {
            if (bracketStack.empty() || bracketStack.top() != '[')
            {
                return true; // Unmatched closing bracket
            }
            bracketStack.pop();
        }
    }

    return !bracketStack.empty(); // Unmatched opening bracket
}

string removeEmptyBrackets(const string &source_code)
{
    // Recursively search for empty brackets and remove them
    string result = source_code;
    size_t pos1 = result.find("[]");
    size_t pos2 = result.find("<>");
    size_t pos3 = result.find("><");

    if (pos1 != string::npos)
    {
        result.erase(pos1, 2);
    }
    else if (pos2 != string::npos)
    {
        result.erase(pos2, 2);
    }
    else if (pos3 != string::npos)
    {
        result.erase(pos3, 2);
    }
    else
    {
        return result;
    }
    return removeEmptyBrackets(result);
}

string removeRedundantOperations(const string &source_code)
{
    string result;
    int count = 0;
    for (char c : source_code)
    {
        if (c == '+')
        {
            count++;
        }
        else if (c == '-')
        {
            count--;
        }
        else
        {
            result.append(abs(count), count > 0 ? '+' : '-');
            result += c;
            count = 0;
        }
    }
    result.append(abs(count), count > 0 ? '+' : '-');

    return result;
}

string filterSourceCode(string source_code)
{
    string result;
    result = removeUnwantedChars(source_code);
    if (hasUnmatchedBrackets(result))
    {
        return "Unmatched brackets";
    }
    result = removeEmptyBrackets(result);
    result = removeRedundantOperations(result);
    return result;
}

void handleOperations(char operation, uint8_t *&p)
{
    switch (operation)
    {
    case '+':
        ++(*p);
        break;
    case '-':
        --(*p);
        break;
    case '>':
        p++;
        break;
    case '<':
        p--;
        break;
    }
}

void handleIO(char operation, uint8_t *&p)
{
    if (operation == '.')
    {
        cout << *p;
    }
    else
    {
        cin >> *p;
    }
}

void executeSourceCode(string source_code)
{
    uint8_t data[30000] = {0};
    uint8_t *p = &data[0];
    for (char c : source_code)
    {
        if (c == '[' || c == ']')
        {
            continue;
        }
        else if (c == '.' || c == ',')
        {
            handleIO(c, p);
        }
        else
        {
            handleOperations(c, p);
        }
    }
    return;
}

int main()
{
    string s = "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.";
    cout << filterSourceCode(s) << endl;
    executeSourceCode(s);
    cout << endl;
}