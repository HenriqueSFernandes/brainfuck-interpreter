/// @file
/// @author Henrique Fernandes <henriquesardofernandes@gmail.com>
/// @version 1.0

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

// Copyright (C) 2023  Henrique Fernandes
// For any further questions or inquiries, please contact henriquesardofernandes@gmail.com

/// @section DESCRIPTION

// This C++ program parses and executes Brainfuck code, providing an environment to run and interpret Brainfuck programs

#include <algorithm>
#include <vector>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <fstream>
#include <unistd.h>

using namespace std;

/// @brief Removes any invalid characters
/// @param source_code Source code
void removeUnwantedChars(string &source_code)
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
    source_code = result;
}

/// @brief Detects and stores matching bracket pairs in source_code while handling errors for unmatched brackets using a stack and a map
/// @param source_code Source code
/// @param bracketPairs Unordered map used to store the bracket pairs
void matchBrackets(const string &source_code, unordered_map<int, int> &bracketPairs)
{
    stack<int> bracketStack;

    for (int i = 0; i < (int)source_code.length(); i++)
    {
        if (source_code[i] == '[')
        {
            bracketStack.push(i);
        }
        else if (source_code[i] == ']')
        {
            if (bracketStack.empty())
            {
                cout << "Error: unmatched closing bracket\n";
                exit(1);
            }
            else
            {
                int openBracketIndex = bracketStack.top();
                bracketStack.pop();
                bracketPairs[openBracketIndex] = i; // Store pair and its reverse
                bracketPairs[i] = openBracketIndex;
            }
        }
    }
    if (!bracketStack.empty())
    {
        cout << "Error: unmatched opening bracket\n";
        exit(1);
    }
}

/// @brief Recursively search for empty brackets (angled and squared) and remove them
/// @param source_code Source code
/// @return String containing the source code without empty brackets
string removeEmptyBrackets(const string &source_code)
{
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

/// @brief Removes redundant +/- operations
/// @param source_code Source code
void removeRedundantOperations(string &source_code)
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

    source_code = result;
}

/// @brief Handles +/-/</> operations
/// @param operation Character representing the operation
/// @param p Pointer to the current memory cell
/// @param data Array of all memory cells
void handleOperations(char operation, uint8_t *&p, uint8_t data[])
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
        if (p == &data[29999])
        {
            p = &data[0];
        }
        else
        {
            p++;
        }
        break;
    case '<':
        if (p == &data[0])
        {
            p = &data[29999];
        }
        else
        {
            p--;
        }
        break;
    }
}

/// @brief Handles input/output operations
/// @param operation Character representing the operation
/// @param p Pointer to the current memory cell
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

/// @brief Filters and executes the source code
/// @param source_code Source code
void executeSourceCode(string source_code)
{
    cout << "Original code: " << source_code << endl;
    removeUnwantedChars(source_code);
    source_code = removeEmptyBrackets(source_code);
    removeRedundantOperations(source_code);
    cout << "Filtered code: " << source_code << endl;
    unordered_map<int, int> bracketPairs;
    matchBrackets(source_code, bracketPairs);

    uint8_t data[30000] = {0};
    uint8_t *p = &data[0];
    unordered_map<int, int> loopExecutionCounters;
    int loopExecutionThreshold = 1000;

    for (int instructionIndex = 0; instructionIndex < (int)source_code.length(); instructionIndex++)
    {
        char operation = source_code[instructionIndex];
        if (operation == '[' && *p == 0)
        {
            loopExecutionCounters[instructionIndex]++;
            if (loopExecutionCounters[instructionIndex] > 1000)
            {
                cout << "Potential infinite loop detected. Loop executed more than the current threshold (" << loopExecutionThreshold << "), do you wish do increase the threshold and continue with the execution? (y/n)\n";
                char anwser;
                cin >> anwser;
                if (anwser != 'y')
                {
                    exit(1);
                }
                loopExecutionThreshold *= 2;
            }
            instructionIndex = bracketPairs[instructionIndex];
        }
        else if (operation == ']' && *p != 0)
        {
            instructionIndex = bracketPairs[instructionIndex];
        }
        else if (operation == '.' || operation == ',')
        {
            handleIO(operation, p);
        }
        else
        {
            handleOperations(operation, p, data);
        }
    }
    return;
}

int main(int argc, char **argv)
{
    if (argc == 3 && (argv[1] == string("-c") || argv[1] == string("--code")))
    {
        cout << "Running argument as source code.\n";
        string source_code = argv[2];
        executeSourceCode(source_code);
        cout << endl;
    }

    else if (argc == 3 && (argv[1] == string("-i") || argv[1] == string("--input")))
    {
        string path = argv[2];
        cout << "Running " << argv[2] << ".\n";
        ifstream input_file(path);
        string source_code;
        string s;
        if (!input_file.is_open())
        {
            cerr << "Error opening file: " << path << endl;
            return 1; // Return an error code
        }
        while (!input_file.eof())
        {
            getline(input_file, s);
            source_code += s;
        }
        executeSourceCode(source_code);
        cout << endl;
    }

    else
    {
        cout << "Usage: brainfuck --option <argument>\n\nOptions:\n";
        cout << "-c <code>, --code <code>\t\tRun argument as code\n";
        cout << "-i <file>, --input <file>\t\tRun file\n";
    }
}