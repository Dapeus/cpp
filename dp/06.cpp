#include<bits/stdc++.h>

using namespace std;

int main()
{
    int n;
    cin>>n;
    vector<int> v,w;
    while(n--) {
        int t1,t2;
        cin>>t1>>t2;
        v.push_back(t2);
        w.push_back(t1);
    }
    int dp[3][5] = { 0 };
    for(int i = 0; i < 5; i++) {
        if(i > v[0])
            dp[0][i] = v[0];
    }
    for(int i = 1; i < 3; i++) {
        for(int j = 0; j < 5; j++) {
            dp[i][j] = max(dp[i-1][j], dp[i-1][j-w[i]]+v[i]);
        }
    }
    system("pause");
    exit(0);
}