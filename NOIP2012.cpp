#include<bits/stdc++.h>
using namespace std;
int l[1005],r[1005];
int main()
{
    long long ans;
    int n;
    cin>>n;
    for (int i=0;i<n+1;i++) { 
        cin>>l[i]>>r[i];
    }
    for(int i=2;i<n+1;i++){
        long long x=l[0];
        for(int j=1;j<i;j++){
            x *= l[j];
        }
        long long x1 = x/l[i-1];
        long long x2 = x;
        long long x3 = x;
        long long x4 = x1*l[i];

        long long y1 = x1/r[i-1];
        long long y2 = x2/r[i];
        long long y_m1 = max(y1,y2);
        long long y3 = x1/r[i];
        long long y4 = x2/r[i-1];
        long long y_m2 = max(y3,y4);
        ans=min(y_m1,y_m2);
        if(y_m1 > y_m2){
            int temp1 = l[i],temp2 = r[i];
            l[i]=l[i-1];
            r[i]=r[i-1];
            l[i-1]=temp1;
            r[i-1]=temp2;
        }
    }
    cout<<ans<<endl;
    system("pause");
    return 0;
}