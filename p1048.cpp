#include<bits/stdc++.h>
using namespace std;
long long df[10005];
int w[105],v[105];

int main()
{
    int t,m;
    cin>>t>>m;
    for(int i = 0; i < m; i++){
        cin>>w[i]>>v[i];
    }
    for(int i = 0; i < m; i++){
        for(int j = t; j >= w[i]; j--){
            df[j] = max(df[j],df[j-w[i]]+v[i]);
        }
    }
    cout<<df[t]<<endl;
    // system("pause");
    return 0;
}