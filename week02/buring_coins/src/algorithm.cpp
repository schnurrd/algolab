#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

int n;
std::vector<int> coins;
std::vector<std::vector<int>> memo;

int solve_max(int pointer_i, int pointer_j);

int solve_min(int pointer_i, int pointer_j){
	if(pointer_i > pointer_j){
		return 0;
	}
	return std::min(solve_max(pointer_i+1, pointer_j), solve_max(pointer_i, pointer_j-1));
}


int solve_max(int pointer_i, int pointer_j){
	if(pointer_i > pointer_j){
		return 0;
	}
	if (memo[pointer_i][pointer_j] != -1){
		return memo[pointer_i][pointer_j];
	}
	int val = std::max(coins[pointer_i] + solve_min(pointer_i+1, pointer_j),
				coins[pointer_j] + solve_min(pointer_i, pointer_j-1));
	memo[pointer_i][pointer_j] = val;
	return val;
}

void testcase() {
	// TODO:
	std::cin >> n;
	coins = std::vector<int>(n);
	memo = std::vector<std::vector<int>>(n, std::vector<int>(n, -1));
	for (int i=0; i < n; i++){
		std::cin >> coins[i];  // coin values
	}
	std::cout << solve_max(0, n-1) << "\n";

	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
