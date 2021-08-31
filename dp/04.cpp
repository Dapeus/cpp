#include<bits/stdc++.h>

using namespace std;

int maxProfit(vector<int>& prices) {
    int len = prices.size();
    int dp[100005] = { 0 };
    int m = prices[0];
    for(int i = 1; i <= len; i++) {
        if(prices[i] < m)
            m = prices[i];
        dp[i] = max(dp[i-1],prices[i]-m);
    }
    return *max_element(dp,dp+len);
}

int main()
{
    int n;
    cin>>n;
    vector<int> prices;
    while(n--) {
        int t;
        cin>>t;
        prices.push_back(t);
    }
    printf("%d\n",maxProfit(prices));
    system("pause");
    exit(0);
}