#include <bits/stdc++.h>
#include "key_gen.h"
#include "decrypt.h"
using namespace std;

string constant[] = {
    "02", "03"," 01 "," 01 ",
    "01","02"," 03 "," 01 ",
    "01","01"," 02 "," 03 ",
    "03","01"," 01 "," 02 ",
};

vector<vector<string>> sub_byte(vector<vector<string>> state){
    map<char, int> c_in = {
        {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}, {'a', 10}, {'b', 11}, {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15}};
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            string a = bin2hex(state[i][j]);
            string word = sbox[(16 * c_in[a[0]]) + c_in[a[1]]];
            state[i][j] = hex2bin(word);
        }
    }
    return state;
}

vector<vector<string>> shift_rows(vector<vector<string>> state){
    for(int r=1;r<=3;r++){
        vector<string> temp;
        for(int c=0;c<=3;c++) temp.push_back(state[r][c]);
        for(int c=0;c<4;c++){
            int val = (c-r)>=0? (c-r) % 4: 4-(r-c);
            //cout<<"R: "<<r<<"C: "<<c<<" val: "<<val<<endl;;
            state[r][val] = temp[c];
        }
    }
    return state;
}

vector<vector<string>> mix_columns(vector<vector<string>> state){
    vector<vector<string>> trans_state = state;
    for(int col=0;col<4;col++){
        string a = state[0][col], b = state[1][col], c = state[2][col], d = state[3][col];
        trans_state[0][col] = num2bin((gf2n_multiply(bin2num(a), 0x02)) ^ (gf2n_multiply(bin2num(b), 0x03)) ^ (gf2n_multiply(bin2num(c), 0x01)) ^ (gf2n_multiply(bin2num(d), 0x01)));
        trans_state[1][col] = num2bin((gf2n_multiply(bin2num(a), 0x01)) ^ (gf2n_multiply(bin2num(b), 0x02)) ^ (gf2n_multiply(bin2num(c), 0x03)) ^ (gf2n_multiply(bin2num(d), 0x01)));
        trans_state[2][col] = num2bin((gf2n_multiply(bin2num(a), 0x01)) ^ (gf2n_multiply(bin2num(b), 0x01)) ^ (gf2n_multiply(bin2num(c), 0x02)) ^ (gf2n_multiply(bin2num(d), 0x03)));
        trans_state[3][col] = num2bin((gf2n_multiply(bin2num(a), 0x03)) ^ (gf2n_multiply(bin2num(b), 0x01)) ^ (gf2n_multiply(bin2num(c), 0x01)) ^ (gf2n_multiply(bin2num(d), 0x02)));
    }
    return trans_state;
}

vector<vector<string>> encrypt(vector<vector<string>> state, vector<string> roundKey){
    state = addRoundKey(state,generate_four(0,roundKey));
    for (int round = 1; round <= 10; round++){
        // cout<<"Round: "<<round<<endl;
        state = sub_byte(state);
        state = shift_rows(state);
        if (round != 10) state = mix_columns(state);
        state = addRoundKey(state, generate_four(round * 4, roundKey));
        // for (int i = 0; i < 4; i++){
        //     for (int j = 0; j < 4; j++)
        //         cout << bin2hex(state[i][j]) << " ";
        //     cout << endl;
        // }
    }
    return state;
}

int main(){
    string inp,k;
    cin>>inp>>k;
    vector<vector<string>> state(4,vector<string>(4));
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            int place = 8*i+2*j;
            state[j][i] = hex2bin(inp.substr(place,2));
        }
    }

    vector<string> keys = generate_key(hex2bin(k));
    //for(auto it: keys) cout<<bin2hex(it)<<" ";
    vector<vector<string>> encrypted_message = encrypt(state,keys);
    cout<<"Encrypted Message: ";
    for(int c=0;c<4;c++){
        for(int r=0;r<4;r++) cout<<bin2hex(encrypted_message[r][c])<<" ";
    }
    cout<<endl;

    reverse(keys.begin(),keys.end());
    vector<vector<string>> decrypted_message = decrypt(encrypted_message,keys);
    cout<<"Decrypted Message: ";
    for(int c=0;c<4;c++){
        for(int r=0;r<4;r++) cout<<bin2hex(decrypted_message[r][c])<<" ";
    }
    cout<<endl;  
    return 0;
}

//INput: 00041214120412000c00131108231919 2475a2b33475568831e2120013aa5487