///4
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

int n, m; 
std::vector<std::vector<int>> memo;
std::vector<std::vector<std::pair<int, int>>> forward_g;
std::vector<std::vector<std::pair<int, int>>> backward_g;

int solve(int forward, int backward){
  if(forward == n-1 && backward == n-1){
    return 0;
  }
  if(memo[forward][backward] != -2){
    return memo[forward][backward];
  }
  std::vector<std::pair<int,int>>& fwd_neighbors = forward_g[forward];
  std::vector<std::pair<int,int>>& bwd_neighbors = backward_g[backward];
  if((fwd_neighbors.size() == 0 && forward != n-1)
    or (bwd_neighbors.size() == 0 && backward != n-1))  {
    return -1;
  }
  int optimal = -1;
  if(forward < backward){
    int size = fwd_neighbors.size();
    for(int i = 0; i < size; i++){
      int neighbor = fwd_neighbors[i].first;
      if(neighbor != backward || (neighbor == backward && backward == n-1)){
        int rats_collected = fwd_neighbors[i].second;
        int curr = solve(neighbor, backward);
        if(curr < 0){
          continue;
        }
        optimal = std::max(optimal, rats_collected + curr);
      }
    }
  }
  else {
    int size = bwd_neighbors.size();
    for(int i = 0; i < size; i++){
      int neighbor = bwd_neighbors[i].first;
      if(neighbor != forward || (neighbor == forward && forward == n-1)){
        int rats_collected = bwd_neighbors[i].second;
        int curr = solve(forward, neighbor);
        if(curr < 0){
          continue;
        }
        optimal = std::max(optimal, rats_collected + curr);
      }
    }
  }
  memo[forward][backward] = optimal;
  return optimal;
}

void testcase() {
  // TODO:
  std::cin >> n >> m; // number of public squares, one-way streets

  memo = std::vector<std::vector<int>>(n, std::vector<int>(n, -2));
  forward_g = std::vector<std::vector<std::pair<int, int>>>(n);
  backward_g = std::vector<std::vector<std::pair<int, int>>> (n);

  for (int i = 0; i < m; i++){
    int u, v, f; std::cin >> u >> v >> f;
    if (u < v){
      forward_g[u].push_back({v, f});
    } else {
      backward_g[v].push_back({u, f});
    }
  }
  
  int total = solve(0, 0);
  std::cout << total << "\n";
  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
