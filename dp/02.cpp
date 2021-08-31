#include<bits/stdc++.h>

using namespace std;

int findNumberOfLIS(vector<int>& nums) {
    int len = nums.size();
    int dp[2000] = {0}, cnt[2000] = {0};
    for(int i = 0; i < len; i++) {
        cnt[i] = 1;
        dp[i] = 1;
    }
    for(int i = len-2; i >= 0; i--) {
        for(int j = i+1; j < len; j++) {
            if(nums[i] < nums[j]) {
                if(dp[i] < dp[j]+1) {
                    dp[i] = dp[j]+1;
                    cnt[i] = cnt[j];
                } else if(dp[i] == dp[j]+1) {
                    cnt[i] += cnt[j];
                }
            }
        }
    }
    // for(int i = 0; i < len; i++) {
    //     printf("%d ", dp[i]);
    // }
    // printf("\n");
    // for(int i = 0; i < len; i++) {
    //     printf("%d ", cnt[i]);
    // }
    // printf("\n");
    int m = *max_element(dp,dp+nums.size());
    int s = 0;
    for(int i = 0; i < len; i++) {
        if(dp[i] == m)
            s += cnt[i];
    }
    return s;
}

int main()
{
    vector<int> nums;
    int n;
    cin>>n;
    for(int i = 0; i < n; i++) {
        int t;
        cin>>t;
        nums.push_back(t);
    }
    printf("%d\n",findNumberOfLIS(nums));
    system("pause");
    exit(0);
}