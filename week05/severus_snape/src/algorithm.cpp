#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <boost/container_hash/hash.hpp>

int n, m, P, H, W; long a, b;
std::vector<std::pair<int, int>> power_and_happiness_of_potions_A;
std::vector<int> wit_of_potions_B, num_portions_b_needed_for_i_potions_of_a;
std::vector<std::vector<std::vector<long>>> memo;

// uses i potions from range 0, ...j with sum happiness >= h
long solve(int i, int j, int h){
	if (i <= 0 or j <= 0) return 0;
    if (memo[i][j][h] != -1) return memo[i][j][h];
	std::pair<int, int>& current_potion = power_and_happiness_of_potions_A[j-1];
	long result = solve(i, j-1, h);
	long val = solve(i-1, j-1, std::max(h - current_potion.second, 0));
	if (val > 0 or h <= current_potion.second){
		val += current_potion.first;
	}
	result = std::max(result, val);
	memo[i][j][h] = result;
	return result;
}


void testcase() {
	// TODO:
	std::cin >> n >> m; // potions of type A and B
	std::cin >> a >> b; 
	// a wit Harray loses with each potion of type A, 
	// b magical power lost with each potion of type B
	std::cin >> P >> H >> W; //needed powers
	power_and_happiness_of_potions_A = std::vector<std::pair<int, int>> (n);
	for (int i = 0; i < n; i++){
		int p_i, h_i; std::cin >> p_i >> h_i;
		power_and_happiness_of_potions_A[i] = std::make_pair(p_i, h_i);
		// magical power and happiness of poitnion i of type A
	}
	wit_of_potions_B = std::vector<int> (m);
	for (int i = 0; i < m; i++){
		std::cin >> wit_of_potions_B[i];
	}
	std::sort(wit_of_potions_B.begin(), wit_of_potions_B.end(), std::greater<int>());
	long sum = 0;
	num_portions_b_needed_for_i_potions_of_a = std::vector<int> (n+1, std::numeric_limits<int>::max());
	memo = std::vector<std::vector<std::vector<long>>> (n+1, std::vector<std::vector<long>>(n+1, std::vector<long>(H+1, -1)));
	for (int i = 0; i < m; i++){
		sum += wit_of_potions_B[i];
		for (int j = 0; j <= n; j++){
			if (W+j*a <= sum){
				num_portions_b_needed_for_i_potions_of_a[j] = 
					std::min(num_portions_b_needed_for_i_potions_of_a[j], i+1);
			} else {
				break;
			}
		}
	}
	//solve_table(); //memo[i][n][H]
	int min_potion = std::numeric_limits<int>::max();
	for (int i = 0; i <= n; i++){
		if((solve(i, n, H) - P - b * num_portions_b_needed_for_i_potions_of_a[i]) >= 0 and 
			num_portions_b_needed_for_i_potions_of_a[i] != std::numeric_limits<int>::max()){
			min_potion = i + num_portions_b_needed_for_i_potions_of_a[i];
			break;
		}
	}
	if (min_potion == std::numeric_limits<int>::max()){
		std::cout << -1 << "\n";
		return;
	}
	std::cout << min_potion << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
