///3
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

int n;
int max_val;
std::vector<int> vals_parcel_A;
std::vector<int> vals_parcel_B;
std::vector<std::vector<int>> dp;


int get_val(){
  for (int i = 0; i < n; i++){
	for (int j = 0; j < n; j++){
		dp[i+1][j+1] = std::min(dp[i][j+1], 
							std::min(dp[i+1][j], dp[i][j])
						) + (vals_parcel_A[i]-1) * (vals_parcel_B[j]-1);
	}
  }
  return dp[n][n];
}

void testcase() {
  // TODO:
  std::cin >> n; // number of parcels on each of the tracks
  vals_parcel_A = std::vector<int>(n);
  vals_parcel_B = std::vector<int>(n);
  dp = std::vector<std::vector<int>>(n+1, std::vector<int>(n+1, 10000000));
  dp[0][0] = 0;
  for (int i = 0; i < n; i++){
    std::cin >> vals_parcel_A[i];
  }
  for (int i = 0; i < n; i++){
    std::cin >> vals_parcel_B[i];
  }
  std::cout << get_val() << "\n";
  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
