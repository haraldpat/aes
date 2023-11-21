#include <bits/stdc++.h>
using namespace std;

vector<string> generate_four(int i, vector<string> key)
{
    vector<string> res;
    for (int j = i; j < i + 4; j++)
        res.push_back(key[j]);
    return res;
}

vector<string> de_generate_four(int i, vector<string> key)
{
    vector<string> res;
    for (int j = i; j < i + 4; j++)
        res.push_back(key[j]);
    reverse(res.begin(), res.end());
    return res;
}

vector<vector<string>> addRoundKey(vector<vector<string>> state, vector<string> key)
{
    vector<vector<string>> temp_state = state;
    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            // cout << bin2hex(state[row][col]) << " " << bin2hex(key[col].substr(8*row,8)) << endl;
            temp_state[row][col] = xora(state[row][col], key[col].substr(8 * row, 8));
        }
    }
    return temp_state;
}