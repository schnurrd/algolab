#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

int n, m, k; 
std::vector<int> sovereigns_values;
std::vector<std::vector<int>> max_profit;

int solve(int pointer_i, int pointer_j){
	if (pointer_i > pointer_j){
		return 0;
	}
	if (max_profit[pointer_i][pointer_j] != -1){
		return max_profit[pointer_i][pointer_j];
	}
	int max_val;
	if ((pointer_i+n-pointer_j-1)%m == k){
		max_val = std::max(
			sovereigns_values[pointer_i] + solve(pointer_i+1, pointer_j),
			sovereigns_values[pointer_j] + solve(pointer_i, pointer_j-1));
	} else {
		max_val = std::min(solve(pointer_i, pointer_j-1), 
							solve(pointer_i+1, pointer_j));
	}
	max_profit[pointer_i][pointer_j] = max_val;
	return max_val;
}

void testcase() {
	// TODO:
	std::cin >> n >> m >> k; // number of sovereigns, passengers, index
	sovereigns_values = std::vector<int>(n);
	max_profit = std::vector<std::vector<int>> (n, std::vector<int>(n, -1));
	for (int i = 0; i < n; i++){
		std::cin >> sovereigns_values[i];
	}
	std::cout << solve(0, n-1) << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
