#include <iostream>
#include <algorithm>
#include <limits.h>
#include <vector>
#include <utility>

using namespace std;

int n, m;
vector<vector<long>> dp;
vector<vector<pair<int,long>>> fwd_graph;
vector<vector<pair<int,long>>> bwd_graph;

long solve(int fwd, int bwd){
  if(fwd == n-1 && bwd == n-1)
  {
    return 0;
  }
  
  if(dp[fwd][bwd] != -2)
  {
    return dp[fwd][bwd];
  }
  
  vector<pair<int,long>> fwd_neighbors = fwd_graph[fwd];
  vector<pair<int,long>> bwd_neighbors = bwd_graph[bwd];
  if(fwd_neighbors.size() == 0 && fwd != n-1)
  {
    return -1;
  }
  if(bwd_neighbors.size() == 0 && bwd != n-1)
  {
    return -1;
  }
  
  long optimal = -1;
  if(fwd < bwd)
  {
    int size = fwd_neighbors.size();
    for(int i = 0; i < size; i++)
    {
      int neighbor = fwd_neighbors[i].first;
      if(neighbor != bwd || (neighbor == bwd && bwd == n-1))
      {
        long rats_collected = fwd_neighbors[i].second;
        long curr = solve(neighbor, bwd);
        if(curr < 0)
        {
          continue;
        }
        optimal = max(optimal, rats_collected + curr);
      }
    }
  }
  else
  {
    int size = bwd_neighbors.size();
    for(int i = 0; i < size; i++)
    {
      int neighbor = bwd_neighbors[i].first;
      if(neighbor != fwd || (neighbor == fwd && fwd == n-1))
      {
        long rats_collected = bwd_neighbors[i].second;
        int curr = solve(fwd, neighbor);
        if(curr < 0)
        {
          continue;
        }
        optimal = max(optimal, rats_collected + curr);
      }
    }
  }
  
  dp[fwd][bwd] = optimal;
  return optimal;
}

void testcase(){
  cin >> n >> m;
  
  dp = vector<vector<long>>(n, vector<long>(n, -2));
  fwd_graph = vector<vector<pair<int,long>>>(n);
  bwd_graph = vector<vector<pair<int,long>>>(n);
  
  for(int i = 0; i < m; i++)
  {
    int u, v, f;
    cin >> u >> v >> f;
    
    if(u < v)
    {
      fwd_graph[u].push_back({v, f});
    }
    else
    {
      bwd_graph[v].push_back({u, f});
    }
  }
  
  long total = solve(0, 0);
  cout << total << endl;
}

int main()
{
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  
  int t;
  cin >> t;
  
  for(int i = 0; i < t; i++)
  {
    testcase();
  }
}