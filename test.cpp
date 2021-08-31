#include<bits/stdc++.h>

using namespace std;

void dfs(vector<vector<int>>& graph,vector<int> road, int x) {
    if(road[x] == 4) {
        graph.push_back(road);
        return;
    }
    else {
        for(int i = 0; i < graph[road[x]].size(); i++) {
            road.push_back(graph[road[x]][i]);
            dfs(graph,road,x+1);
            road.pop_back();
        }
    }
}

vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
    vector<int> road;
    road.push_back(0);
    int x = 0;
    dfs(graph,road,x);
    return graph;
}


int main(void)
{
    vector<vector<int>> graph;
    for(int i = 0; i < 4; i++) {
        vector<int> t;
        int n;
        while(cin>>n && n != -1) {
            t.push_back(n);
        }
        graph.push_back(t);
    }
    vector<int> t;
    graph.push_back(t);
    allPathsSourceTarget(graph);

    system("pause");
    exit(0);    
}