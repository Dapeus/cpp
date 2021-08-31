#include<bits/stdc++.h>
using namespace std;
int w[10005],v[10005];
long long f[10000005];

int main()
{
    int t,m;
    cin>>t>>m;
    for(int i = 0; i < m; i++){
        cin>>w[i]>>v[i];
    }
    // for(int i = 0; i <= t; i++){
    //     for(int j = 0; j < m; j++){
    //         if(i-w[j]>=0)
    //             f[i] = max(f[i],f[i-w[j]]+v[j]);
    //     }
    // }
    for(int j = 0; j < m; j++)
    {
        for(int i = w[j]; i <= t; i++)
        {
            f[i] = max(f[i],f[i-w[j]]+v[j]);
        }
    }
    cout<<f[t]<<endl;
    // system("pause");
    return 0;
}