#include <bits/stdc++.h>
using namespace std;

const int overflow = 0x100, mod = 0x11B; // AES GF(2^8) representation

int gf2n_multiply(int a, int b)
{
    int sum = 0;
    while (b > 0)
    {
        if (b & 1)
            sum = sum ^ a; // if last bit of b is 1, add a to the sum
        b = b >> 1;        // divide b by 2, discarding the last bit
        a = a << 1;        // multiply a by 2
        if (a & overflow)
            a = a ^ mod; // reduce a modulo the AES polynomial
    }
    return sum;
}

int bin2num(string a)
{
    int n = a.size(), f = 1, ans = 0;
    for (int i = n - 1; i >= 0; i--)
    {
        if (a[i] == '1')
            ans += f;
        f *= 2;
    }
    return ans;
}

string num2bin(int n)
{
    string res = "";
    int f = 128;
    for (int i = 0; i < 8; i++)
    {
        res += ((n / f) == 0 ? '0' : '1');
        n = n % f;
        f /= 2;
    }
    return res;
}
