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

string remove_unwanted(const string &source_code)
{
    vector<char> valid_chars = {'[', ']', '<', '>', '+', '-', '.', ','};
    string result;

    for (char c : source_code)
    {
        if (find(valid_chars.begin(), valid_chars.end(), c) != valid_chars.end())
        {
            result += c; // if c is a valid char, add it to the result
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

string brainfuck_to_c(string source_code)
{
    string result;
    result = remove_unwanted(source_code);
    if (hasUnmatchedBrackets(result))
    {
        return "Unmatched brackets";
    }
    return result;
}

int main()
{
    string s = "+[>[+]-]";
    cout << brainfuck_to_c(s) << endl;
}