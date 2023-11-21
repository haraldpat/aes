#include <bits/stdc++.h>
#include "Round_key_add_gen.h"
#include "tools_gf2n.h"
using namespace std;

vector<vector<string>> de_shift_rows(vector<vector<string>> state)
{
    for (int r = 1; r <= 3; r++)
    {
        vector<string> temp;
        for (int c = 0; c <= 3; c++)
            temp.push_back(state[r][c]);
        for (int c = 0; c < 4; c++)
        {
            int val = (c + r) % 4;
            // cout<<"R: "<<r<<"C: "<<c<<" val: "<<val<<endl;;
            state[r][val] = temp[c];
        }
    }
    return state;
}

vector<vector<string>> de_sub_byte(vector<vector<string>> state)
{
    map<char, int> c_in = {
        {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}, {'a', 10}, {'b', 11}, {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15}};
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            string a = bin2hex(state[i][j]);
            string word = rsbox[(16 * c_in[a[0]]) + c_in[a[1]]];
            state[i][j] = hex2bin(word);
        }
    }
    return state;
}

vector<vector<string>> de_mix_columns(vector<vector<string>> state)
{
    vector<vector<string>> trans_state = state;
    for (int col = 0; col < 4; col++)
    {
        string a = state[0][col], b = state[1][col], c = state[2][col], d = state[3][col];
        trans_state[0][col] = num2bin((gf2n_multiply(bin2num(a), 0x0e)) ^ (gf2n_multiply(bin2num(b), 0x0b)) ^ (gf2n_multiply(bin2num(c), 0x0d)) ^ (gf2n_multiply(bin2num(d), 0x09)));
        trans_state[1][col] = num2bin((gf2n_multiply(bin2num(a), 0x09)) ^ (gf2n_multiply(bin2num(b), 0x0e)) ^ (gf2n_multiply(bin2num(c), 0x0b)) ^ (gf2n_multiply(bin2num(d), 0x0d)));
        trans_state[2][col] = num2bin((gf2n_multiply(bin2num(a), 0x0d)) ^ (gf2n_multiply(bin2num(b), 0x09)) ^ (gf2n_multiply(bin2num(c), 0x0e)) ^ (gf2n_multiply(bin2num(d), 0x0b)));
        trans_state[3][col] = num2bin((gf2n_multiply(bin2num(a), 0x0b)) ^ (gf2n_multiply(bin2num(b), 0x0d)) ^ (gf2n_multiply(bin2num(c), 0x09)) ^ (gf2n_multiply(bin2num(d), 0x0e)));
    }
    return trans_state;
}

vector<vector<string>> decrypt(vector<vector<string>> state, vector<string> roundKey)
{
    state = addRoundKey(state, de_generate_four(0, roundKey));
    // for (int i = 0; i < 4; i++)
    // {
    //     for (int j = 0; j < 4; j++)
    //         cout << bin2hex(state[i][j]) << " ";
    //     cout << endl;
    // }
    for (int round = 1; round <= 10; round++)
    {
        //cout << "Round: " << round << endl;
        state = de_shift_rows(state);
        state = de_sub_byte(state);
        state = addRoundKey(state, de_generate_four(round * 4, roundKey));
        if (round != 10)
            state = de_mix_columns(state);
    }
    return state;
}