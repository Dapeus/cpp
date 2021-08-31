#include<bits/stdc++.h>

using namespace std;

vector<int> countBits(int n) {
    vector<int> ret;
    for(int i = 0; i <= n; i++) {
        int cnt = 0;
        if(i&1) {
            cnt++;
        }
        for(int j = 2; j <= i; j*=2) {
            if(i&j) {
                cnt++;
            }
        }
        ret.push_back(cnt);
    }
    return ret;
}
int main()
{
    int n;
    cin>>n;
    
    system("pause");
    exit(0);
}