///5
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

int n, m, k;
std::vector<int> pointer_map;
std::vector<std::vector<int>> memo;

int solve(int pointer_i, int current_m){
  if (current_m > n - pointer_i){
    return std::numeric_limits<int>::min();
  }
  if (memo[pointer_i][current_m] != -1){
    return memo[pointer_i][current_m];
  }
  int max_val = solve(pointer_i+1, current_m);
  int pointer_j = pointer_map[pointer_i];
  if(pointer_j != -1){
    max_val = std::max(max_val, pointer_j - pointer_i + solve(pointer_j, current_m-1));
  }
  
  memo[pointer_i][current_m] = max_val;
  return max_val;
}

void testcase() {
  // TODO:
  std::cin >> n >> m >> k;
  std::vector<int> power_required_cum_sum(n+1, 0);
  pointer_map = std::vector<int> (n+1, -1);
  memo = std::vector<std::vector<int>>(n+1,std::vector<int>(m+1, -1));
  int sum = 0;
  for(int i = 1; i <= n; i++){
    int val; std::cin >> val;
    sum += val;
    power_required_cum_sum[i] = sum;
  }
  int pointer_j = 1;
  for (int i = 0; i <= n; i++){
	while((power_required_cum_sum[pointer_j] - power_required_cum_sum[i] < k) 
      and pointer_j < n){
    	pointer_j++;
  	}
	if (power_required_cum_sum[pointer_j] - power_required_cum_sum[i] == k){
		pointer_map[i] = pointer_j;
	}
  }
  
  for (int i = 0; i < n+1; i++){
    memo[i][0] = 0;
  }
  for (int i = 1; i < m+1; i++){
    memo[n][i] = std::numeric_limits<int>::min();
  }
  
  int result = solve(0, m);
  if (result < 0){
    std::cout << "fail" << "\n";
  } else {
    std::cout << result << "\n";
  }
  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
