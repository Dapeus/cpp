#include<bits/stdc++.h>

using namespace std;

vector<vector<int>> generate(int numRows) {
    vector<vector<int>> s;
    vector<int> v1;
    v1.push_back(1);
    s.push_back(v1);
    if(numRows>1) {
        vector<int> v2;
        v2.push_back(1);
        v2.push_back(1);
        s.push_back(v2);
    }
    for(int i = 2; i < numRows; i++) {
        vector<int> v;
        v.push_back(1);
        for(int j = 0; j < s[i-1].size()-1; j++) {
            v.push_back(s[i-1][j]+s[i-1][j+1]);
        }
        v.push_back(1);
        s.push_back(v);
    }
    return s;
}

int main()
{
    int n;
    cin>>n;
    generate(n);
    system("pause");
    exit(0);
}