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

using namespace std;

string remove_unwanted(string source_code)
{
    vector<char> valid_chars = {'[', ']', '<', '>', '+', '-', '.', ','};
    string result;

    for (char c : source_code)
    {
        if (find(valid_chars.begin(), valid_chars.end(), c) != valid_chars.end())
        {
            result += c;
        }
    }
    return result;
}

string brainfuck_to_c(string source_code)
{
    string result;
    result = remove_unwanted(source_code);
    return result;
}

int main()
{
    string s = "[a]sdfg[+sdg+gdf+a]";
    cout << brainfuck_to_c(s) << endl;
}