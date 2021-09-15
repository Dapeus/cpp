#include<bits/stdc++.h>

using namespace std;

int l(string s)
{
    int n = s.length(), ret = 0;
    bool dp[n][100] = { false };
    for(int i = n-1; i >= 0; i--) {
        for(int j = i; j < n; j++) {
            if(s[i] == s[j]) {
                if(j-i <= 1) {
                    ret++;
                    dp[i][j] = true;
                } else if(dp[i+1][j-1]) {
                    dp[i][j] = true;
                    ret++;
                }
            }
        }
    }
    return ret;
}

int main()
{
    string s;
    cin>>s;
    printf("%d\n",l(s));
    system("pause");
    return 0;
}